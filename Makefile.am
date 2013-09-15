# Copyright (C) 2011,2013 Colin Walters <walters@verbum.org>
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

ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
AM_CPPFLAGS = -DDATADIR='"$(datadir)"' \
	-DLIBEXECDIR='"$(libexecdir)"' \
	-DLOCALEDIR=\"$(datadir)/locale\" \
	-DGLIB_VERSION_MIN_REQUIRED=GLIB_VERSION_2_34 -DGLIB_VERSION_MAX_ALLOWED=GLIB_VERSION_2_34
AM_CFLAGS = $(WARN_CFLAGS)
DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc --disable-maintainer-mode

# Subdirectories
SUBDIRS = .

#if ENABLE_GTK_DOC
#SUBDIRS += doc
#endif

# Common variables
NULL = 
INSTALL_DATA_HOOKS =
BUILT_SOURCES =
MANPAGES =
CLEANFILES =
EXTRA_DIST =
bin_PROGRAMS =
sbin_PROGRAMS =
bin_SCRIPTS =
lib_LTLIBRARIES =
libexec_PROGRAMS =
noinst_LTLIBRARIES =
noinst_PROGRAMS =
privlibdir = $(pkglibdir)
privlib_LTLIBRARIES =
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA =
INTROSPECTION_GIRS = 
girdir = $(datadir)/gir-1.0
gir_DATA =
typelibdir = $(libdir)/girepository-1.0
typelib_DATA =

EXTRA_DIST += autogen.sh COPYING

libgsystem_srcpath := src/libgsystem
libgsystem_cflags = $(PKGDEP_GIO_UNIX_CFLAGS) -I$(srcdir)/src/libgsystem
libgsystem_libs = $(PKGDEP_GIO_UNIX_LIBS)
include src/libgsystem/Makefile-libgsystem.am
noinst_LTLIBRARIES += libgsystem.la

include Makefile-pkgsys-ostree.am
include Makefile-fedora.am

install-data-hook: $(INSTALL_DATA_HOOKS)

release-tag:
	git tag -m "Release $(VERSION)" v$(VERSION)

git_version_rpm = $$(git describe | sed -e 's,-,\.,g' -e 's,^v,,')

dist-snapshot:
	set -x; \
	GITVERSION=$(git_version_rpm); export GITVERSION; \
	TARFILE_TMP=$(PACKAGE)-$${GITVERSION}.tar.tmp; \
	REV=$$(git rev-parse HEAD); \
	echo "Archiving $(PACKAGE) at $${REV}"; \
	(cd $(srcdir); git archive --format=tar --prefix=$(PACKAGE)-$${GITVERSION}/ $${REV}) > $${TARFILE_TMP}; \
	(cd $$(git rev-parse --show-toplevel); git submodule status) | while read line; do \
	  rev=$$(echo $$line | cut -f 1 -d ' '); path=$$(echo $$line | cut -f 2 -d ' '); \
	  echo "Archiving $${path} at $${rev}"; \
	  (cd $(srcdir)/$$path; git archive --format=tar --prefix=$(PACKAGE)-$${GITVERSION}/$$path/ $${rev}) > submodule.tar; \
	  tar -A -f $${TARFILE_TMP} submodule.tar; \
	  rm submodule.tar; \
	done; \
	mv $(PACKAGE)-$${GITVERSION}.tar{.tmp,}; \
	rm -f $(PACKAGE)-$${GITVERSION}.tar.xz; \
	xz $(PACKAGE)-$${GITVERSION}.tar 

srpm: dist-snapshot
	GITVERSION=$(git_version_rpm); export GITVERSION; \
	sed -e "s,^Version:,Version: $${GITVERSION}," < $(srcdir)/packages/$(PACKAGE).spec.in > $(PACKAGE).spec
	$(srcdir)/packages/rpmbuild-cwd -bs $(PACKAGE).spec

rpm-local: srpm
	GITVERSION=$(git_version_rpm); export GITVERSION; \
	$(srcdir)/packages/rpmbuild-cwd --rebuild $(PACKAGE)-${GITVERSION}*.src.rpm
