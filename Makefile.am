ACLOCAL_AMFLAGS = -I m4

h2oxxdir = $(includedir)/h2o/cxx
pkgconfigdir = $(libdir)/pkgconfig

lib_LTLIBRARIES = libh2oxx.la

libh2oxx_la_SOURCES = src/h2o.cxx
libh2oxx_la_CPPFLAGS = $(LIBH2O_CFLAGS) -I$(top_srcdir)/include
libh2oxx_la_LIBADD = $(LIBH2O_LIBS)
libh2oxx_la_LDFLAGS = -version-info 0:0:0 -no-undefined

h2oxx_HEADERS = include/h2o

pkgconfig_DATA = libh2oxx.pc

TESTS = if97-test-values
check_PROGRAMS = $(TESTS)

if97_test_values_SOURCES = tests/if97-test-values.cxx
if97_test_values_CPPFLAGS = -I$(top_srcdir)/include
if97_test_values_LDADD = libh2oxx.la
