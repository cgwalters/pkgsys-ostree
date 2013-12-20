Using yum-ostree
================

This tool takes a set of packages, and commits them to an OSTree
repository.  At the moment, it is intended for use on build servers.
For example, this invocation:

# yum-ostree --repo=repo --enablerepo=fedora --os=fedora --os-version=20 create myserver @minimal lvm2 kernel ostree mysql wordpress

Will create a ref named "fedora/20/myserver", containing a complete
bootable rootfs of those pacakges.

If you export the repo via any plain HTTP server, clients using OSTree
can then replicate this tree, and boot it, tracking updates you make
over time.

Differences from the package model: Removals
============================================

If you run the above command again, but say remove "wordpress" from
the list, what will happen is that a new tree will be constructed
without the wordpress package.  When clients upgrade using ostree,
those files will be removed.

This is completely unlike using packaging systems on the client side;
removing packages is really difficult and unreliable.

Multiple trees
==============

One thing implicit in the above, but bears emphasizing strongly:
OSTree supports *multiple* bootable trees per repository.  Clients can
choose between them, and atomically switch between them.




