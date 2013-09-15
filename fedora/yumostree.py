#!/usr/bin/env python
#
# Copyright (C) 2012,2013 Colin Walters <walters@verbum.org>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.

import os
import sys
import optparse
import time
import shutil
import subprocess

from gi.repository import GLib
from gi.repository import Gio
from gi.repository import OSTree

os_release_data = {}

def ensuredir(path):
    if not os.path.isdir(path):
        os.makedirs(path)

def rmrf(path):
    shutil.rmtree(path, ignore_errors=True)

def feed_checksum(checksum, stream):
    b = stream.read(8192)
    while b != '':
        checksum.update(b)
        b = stream.read(8192)

def _find_current_origin_refspec():
    dpath = '/ostree/deploy/%s/deploy' % (os_release_data['ID'], )
    for name in os.listdir(dpath):
        if name.endswith('.origin'):
            for line in open(os.path.join(dpath, name)):
                if line.startswith('refspec='):
                    return line[len('refspec='):]
    return None

def _find_current_root():
    return '/ostree/deploy/%s/current' % (os_release_data['ID'], )

def _initfs(targetroot):
    os.makedirs(targetroot)
    for d in ['dev', 'proc', 'run', 'sys', 'var']:
        os.mkdir(os.path.join(targetroot, d))

    # Special ostree mount
    os.mkdir(os.path.join(targetroot, 'sysroot'))

    # Some FHS targets; these all live in /var
    for (target, name) in [('var/opt', 'opt'),
                           ('var/srv', 'srv'),
                           ('var/mnt', 'mnt'),
                           ('var/roothome', 'root'),
                           ('var/home', 'home'),
                           ('run/media', 'media'),
                           ('sysroot/ostree', 'ostree'),
                           ('sysroot/tmp', 'tmp')]:
        os.symlink(target, os.path.join(targetroot, name))


def _clone_current_root_to_yumroot(current_root, yumroot):
    _initfs(yumroot)
    subprocess.check_call(['cp', '--reflink=auto', '-a',
                           os.path.join(current_root, 'usr'),
                           yumroot])
                           

def _create_rootfs_from_yumroot_content(targetroot, yumroot):
    """Prepare a root filesystem, taking mainly the contents of /usr from yumroot"""

    _initfs(targetroot)

    # We take /usr from the yum content
    os.rename(os.path.join(yumroot, 'usr'), os.path.join(targetroot, 'usr'))
    # Plus the RPM database goes in usr/share/rpm
    os.rename(os.path.join(yumroot, 'var/lib/rpm'), os.path.join(targetroot, 'usr/share/rpm'))

    # Except /usr/local -> ../var/usrlocal
    rmrf(os.path.join(targetroot, 'usr/local'))
    os.symlink('../var/usrlocal', os.path.join(targetroot, 'usr/local'))
    target_usretc = os.path.join(targetroot, 'usr/etc')
    rmrf(target_usretc)
    os.rename(os.path.join(yumroot, 'etc'), target_usretc)

    # Move boot, but rename the kernel/initramfs to have a checksum
    targetboot = os.path.join(targetroot, 'boot')
    os.rename(os.path.join(yumroot, 'boot'), targetboot)
    kernel = None
    initramfs = None
    for name in os.listdir(targetboot):
        if name.startswith('vmlinuz-'):
            kernel = os.path.join(targetboot, name)
        elif name.startswith('initramfs-'):
            initramfs = os.path.join(targetboot, name)

    assert (kernel is not None and initramfs is not None)
    
    checksum = GLib.Checksum.new(GLib.ChecksumType.SHA256)
    f = open(kernel)
    feed_checksum(checksum, f)
    f.close()
    f = open(initramfs)
    feed_checksum(checksum, f)
    f.close()

    bootcsum = checksum.get_string()
    
    os.rename(kernel, kernel + '-' + bootcsum)
    os.rename(initramfs, initramfs + '-' + bootcsum)

    # Also carry along toplevel compat links
    for name in ['lib', 'lib64', 'bin', 'sbin']:
        src = os.path.join(yumroot, name)
        if os.path.islink(src):
            os.rename(src, os.path.join(targetroot, name))

    target_tmpfilesd = os.path.join(targetroot, 'usr/lib/tmpfiles.d')
    ensuredir(target_tmpfilesd)
    shutil.copy(os.path.join(PKGLIBDIR, 'tmpfiles-gnome-ostree.conf'), target_tmpfilesd)

def main():
    parser = optparse.OptionParser('%prog ACTION PACKAGE1 [PACKAGE2...]')
    parser.add_option('', "--repo",
                      action='store', dest='repo_path',
                      default=None,
                      help="Path to OSTree repository (default=/ostree)")
    parser.add_option('', "--local-ostree-package",
                      action='store', dest='local_ostree_package',
                      default='ostree',
                      help="Path to local OSTree RPM")

    (opts, args) = parser.parse_args(sys.argv[1:])

    f = open('/etc/os-release')
    for line in f.readlines():
        if line == '': continue
        (k,v) = line.split('=', 1)
        os_release_data[k.strip()] = v.strip()
    f.close()

    action = args[0]
    if action == 'create':
        branchname = args[1]
        ref = '%s/%s/%s' % (os_release_data['ID'], os_release_data['VERSION_ID'], branchname)
        packages = args[2:]
    elif action == 'install':
        packages = args[1:]
        ref = _find_current_origin_refspec()
    elif action == 'upgrade':
        packages = []
        ref = _find_current_origin_refspec()
    else:
        print >>sys.stderr, "Unknown action %s" % (action, )
        sys.exit(1)

    # Hardcoded, yes.
    packages.append('kernel')
    packages.append(opts.local_ostree_package)

    if opts.repo_path is not None:
        repo = OSTree.Repo.new(Gio.File.new_for_path(opts.repo_path))
    else:
        repo = OSTree.Repo.new_default()

    repo.open(None)

    cachedir = '/var/cache/yum-ostree/work'
    ensuredir(cachedir)

    yumroot = os.path.join(cachedir, 'yum')
    targetroot = os.path.join(cachedir, 'rootfs')
    yumcachedir = os.path.join(yumroot, 'var/cache/yum')
    yumcache_lookaside = os.path.join(cachedir, 'yum-cache')
    logs_lookaside = os.path.join(cachedir, 'logs')

    print "Will create commit %s using packages %r" % (ref, packages)

    shutil.rmtree(yumroot, ignore_errors=True)
    if action == 'create':
        if os.path.isdir(yumcache_lookaside):
            yumroot_varcache = os.path.join(yumroot, 'var/cache')
            print "Reusing cache: " + yumroot_varcache
            ensuredir(yumroot_varcache)
            subprocess.check_call(['cp', '-a', yumcache_lookaside, yumcachedir])
        else:
            print "No cache found at: " + yumroot_varcache
    else:
        current_root = _find_current_root()
        print "Cloning %s" % (current_root, )
        _clone_current_root_to_yumroot(current_root, yumroot)

    if action == 'create':
        yumargs = ['yum', '-y', '--releasever=%s' % (os_release_data['VERSION_ID'], ), '--nogpg', '--setopt=keepcache=1',
                '--installroot=' + yumroot, '--disablerepo=*', '--enablerepo=fedora', 'install']
    elif action == 'upgrade':
        yumargs = ['yum', '--installroot=' + yumroot, 'upgrade']
    elif action == 'install':
        yumargs = ['yum', '--installroot=' + yumroot, 'install']
    else:
        assert False
    print "Running: %s" % (subprocess.list2cmdline(yumargs), )
    yumargs.extend(packages)
    subprocess.check_call(yumargs)

    if action == 'create':
        # Attempt to cache stuff between runs
        rmrf(yumcache_lookaside)
        print "Saving yum cache " + yumcache_lookaside
        os.rename(yumcachedir, yumcache_lookaside)

    yumroot_rpmlibdir = os.path.join(yumroot, 'var/lib/rpm')
    rpmtextlist = os.path.join(cachedir, 'rpm-manifest.txt')
    manifest = subprocess.check_call(['rpm', '-qa', '--dbpath=' + yumroot_rpmlibdir],
                                     stdout=open(rpmtextlist, 'w'))

    rmrf(targetroot)
    _create_rootfs_from_yumroot_content(targetroot, yumroot)

    # Move the log files out
    rmrf(logs_lookaside)
    ensuredir(logs_lookaside)
    yumroot_varlog = os.path.join(yumroot, 'var/log')
    for name in os.listdir(yumroot_varlog):
        shutil.move(os.path.join(yumroot_varlog, name), logs_lookaside)

    # To make SELinux work, we need to do the labeling right before this.
    # This really needs some sort of API, so we can apply the xattrs as
    # we're committing into the repo, rather than having to label the
    # physical FS.
    # For now, no xattrs (and hence no SELinux =( )
    print "Committing " + targetroot + "..."
    repo.prepare_transaction(None)
    mtree = OSTree.MutableTree.new()
    modifier = OSTree.RepoCommitModifier.new(OSTree.RepoCommitModifierFlags.SKIP_XATTRS, None, None)
    repo.write_directory_to_mtree(Gio.File.new_for_path(targetroot),
                                  mtree, modifier, None)
    [success, parent] = repo.resolve_rev(ref, True)
    [success, tree] = repo.write_mtree(mtree, None)
    [success, commit] = repo.write_commit(parent, '', '', None, tree, None)
    repo.transaction_set_ref(None, ref, commit)
    repo.commit_transaction(None)

    print "%s => %s" % (ref, commit)

    rmrf(yumroot)
    rmrf(targetroot)

    subprocess.check_call(['ostree', 'admin', 'deploy', '--os=' + os_release_data['ID'], ref])

main()
