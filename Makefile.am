ACLOCAL_AMFLAGS = -I m4

h2oxxdir = $(includedir)/h2o/cxx
pkgconfigdir = $(libdir)/pkgconfig

lib_LTLIBRARIES = libh2oxx.la

libh2oxx_la_SOURCES =
libh2oxx_la_CPPFLAGS = $(LIBH2O_CFLAGS)
libh2oxx_la_LIBADD = $(LIBH2O_LIBS)
libh2oxx_la_LDFLAGS = -version-info 0:0:0

h2oxx_HEADERS =

pkgconfig_DATA = libh2oxx.pc
