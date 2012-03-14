AC_PREREQ([2.60])
AC_INIT([libh2o++], [0])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.6 foreign dist-bzip2 no-dependencies])

PKG_CHECK_MODULES([LIBH2O], [libh2o])

AC_PROG_CXX

LT_INIT([disable-static])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile libh2oxx.pc])
AC_OUTPUT
