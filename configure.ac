AC_PREREQ([2.63])
AC_INIT([pkgsys-ostree], [2013.1], [walters@verbum.org])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([1.11 -Wno-portability foreign no-define tar-ustar no-dist-gzip dist-xz])
AM_MAINTAINER_MODE([enable])
AM_SILENT_RULES([yes])
AC_USE_SYSTEM_EXTENSIONS
AC_SYS_LARGEFILE

AC_PROG_CC
AM_PROG_CC_C_O

changequote(,)dnl
if test "x$GCC" = "xyes"; then
  WARN_CFLAGS="-Wall -Wstrict-prototypes -Werror=missing-prototypes \
		-Werror=implicit-function-declaration \
		-Werror=pointer-arith -Werror=init-self -Werror=format=2 \
		-Werror=format-security \
		-Werror=missing-include-dirs -Werror=aggregate-return \
		-Werror=declaration-after-statement"
fi
changequote([,])dnl
AC_SUBST(WARN_CFLAGS)

# Initialize libtool
LT_PREREQ([2.2.4])
LT_INIT([disable-static])

PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES(PKGDEP_GIO_UNIX, [gio-unix-2.0])
PKG_CHECK_MODULES(PKGDEP_OSTREE, [ostree-1])
GTK_DOC_CHECK([1.15], [--flavour no-tmpl])
AC_PATH_PROG([XSLTPROC], [xsltproc])

dnl AC_ARG_WITH([backend],
dnl             AS_HELP_STRING([--with-backend=BACKEND], [Choose BACKEND as package system (currently supported: yum)]),
dnl             [],
dnl             AC_MSG_ERROR([--with-backend=BACKEND must be provided]))
dnl AS_IF([test x$with_backend = xyum], [],
dnl   [ AC_MSG_ERROR([Unrecognized --with-backend=$with_backend])])
AM_CONDITIONAL(BUILDOPT_BACKEND_YUM, test x$with_backend = xyum)
AS_IF([test x$with_backend = xyum], [ AM_PATH_PYTHON(2.7)])

AC_ARG_WITH([systemdsystemunitdir],
            AS_HELP_STRING([--with-systemdsystemunitdir=DIR], [Directory for systemd service files]),
            [],
            [with_systemdsystemunitdir=$($PKG_CONFIG --variable=systemdsystemunitdir systemd)])
AS_IF([test "x$with_systemdsystemunitdir" != "xno"], [
  AC_SUBST([systemdsystemunitdir], [$with_systemdsystemunitdir])
])

AC_CONFIG_FILES([
Makefile
])
AC_OUTPUT

echo "
    fedora-ostree $VERSION
"
