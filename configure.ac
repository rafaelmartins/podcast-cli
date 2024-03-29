AC_PREREQ(2.68)

AC_INIT([podcast-cli], [0.1], [rafael@rafaelmartins.eng.br], [podcast-cli],
  [http://labs.rafaelmartins.eng.br/wiki/podcast-cli])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign no-dist-gzip dist-bzip2 subdir-objects])
AC_CONFIG_HEADERS([config.h])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

LT_INIT

AC_MSG_CHECKING([for mercurial tip])
if test -d "${ac_top_srcdir:-.}/.hg"; then
  if test "x`hg id -t -R ${ac_top_srcdir:-.}`" = "xtip"; then
    HGTIP=`hg id -i -R ${ac_top_srcdir:-.}`
  fi
fi
if test -z "$HGTIP"; then
  AC_MSG_RESULT([no])
else
  AC_MSG_RESULT([$HGTIP])
  AC_DEFINE_UNQUOTED([HGTIP], ["$HGTIP"],
    [the changeset most recently added to the mercurial repository]
  )
fi

AC_PROG_CC_C99
AS_IF([test "x$ac_cv_prog_cc_c99" = "xno"], [
  AC_MSG_ERROR([no C99 compiler found, cgisysinfo requires a C99 compiler.])
])

PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([MRSS], [mrss])

AC_SYS_LARGEFILE

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT

if test -z "$HGTIP"; then
  MY_PACKAGE_STRING="$PACKAGE_STRING"
else
  MY_PACKAGE_STRING="$PACKAGE_STRING/$HGTIP"
fi

echo "
        ==== ${MY_PACKAGE_STRING} ====
        
        prefix:       ${prefix}
        exec_prefix:  ${exec_prefix}
        bindir:       ${bindir}
        
        compiler:     ${CC}
        cflags:       ${CFLAGS}
        ldflags:      ${LDFLAGS}
"
