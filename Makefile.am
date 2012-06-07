ACLOCAL_AMFLAGS = -I m4

# ensure that assertions will be thrown in distcheck
DISTCHECK_CONFIGURE_FLAGS = --enable-debug

h2oxxdir = $(includedir)/h2o/cxx
pkgconfigdir = $(libdir)/pkgconfig

lib_LTLIBRARIES = libh2oxx.la

libh2oxx_la_SOURCES = src/h2o.cxx src/region.cxx
libh2oxx_la_CPPFLAGS = $(LIBH2O_CFLAGS) -I$(top_srcdir)/include
libh2oxx_la_LIBADD = $(LIBH2O_LIBS)
libh2oxx_la_LDFLAGS = -version-info 1:0:1 -no-undefined

h2oxx_HEADERS = include/h2o

pkgconfig_DATA = libh2oxx.pc

TESTS = tests/if97-test-values
check_PROGRAMS = $(TESTS)

tests_if97_test_values_SOURCES = tests/if97-test-values.cxx
tests_if97_test_values_CPPFLAGS = $(LIBH2O_CFLAGS) -I$(top_srcdir)/include
tests_if97_test_values_LDADD = libh2oxx.la

EXTRA_DIST = NEWS
NEWS: configure.ac Makefile.am
	git for-each-ref refs/tags --sort '-*committerdate' \
		--format '# %(tag) (%(*committerdate:short))%0a%(contents:body)' \
		> NEWS
