dnl Usage:
dnl    DC_TEST_SHOBJFLAGS(shobjflags, shobjldflags, action-if-not-found)
dnl
AC_DEFUN([DC_TEST_SHOBJFLAGS], [
  AC_SUBST(SHOBJFLAGS)
  AC_SUBST(SHOBJCPPFLAGS)
  AC_SUBST(SHOBJLDFLAGS)

  OLD_LDFLAGS="$LDFLAGS"
  OLD_CFLAGS="$CFLAGS"
  OLD_CPPFLAGS="$CPPFLAGS"

  SHOBJFLAGS=""
  SHOBJCPPFLAGS=""
  SHOBJLDFLAGS=""

  CFLAGS="$OLD_CFLAGS $1"
  CPPFLAGS="$OLD_CPPFLAGS $2"
  LDFLAGS="$OLD_LDFLAGS $3"

  AC_TRY_LINK([#include <stdio.h>
int unrestst(void);], [ printf("okay\n"); unrestst(); return(0); ], [ SHOBJFLAGS="$1"; SHOBJCPPFLAGS="$2"; SHOBJLDFLAGS="$3" ], [
    LDFLAGS="$OLD_LDFLAGS"
    CFLAGS="$OLD_CFLAGS"
    CPPFLAGS="$OLD_CPPFLAGS"
    $4
  ])

  LDFLAGS="$OLD_LDFLAGS"
  CFLAGS="$OLD_CFLAGS"
  CPPFLAGS="$OLD_CPPFLAGS"
])

AC_DEFUN([DC_GET_SHOBJFLAGS], [
  AC_SUBST(SHOBJFLAGS)
  AC_SUBST(SHOBJCPPFLAGS)
  AC_SUBST(SHOBJLDFLAGS)

  DC_CHK_OS_INFO

  AC_MSG_CHECKING(how to create shared objects)

  if test -z "$SHOBJFLAGS" -a -z "$SHOBJLDFLAGS" -a -z "$SHOBJCPPFLAGS"; then
    DC_TEST_SHOBJFLAGS([-fPIC], [-DPIC], [-shared -rdynamic], [
      DC_TEST_SHOBJFLAGS([-fPIC], [-DPIC], [-shared], [
	DC_TEST_SHOBJFLAGS([-fPIC], [-DPIC], [-shared -rdynamic -mimpure-text], [
	  DC_TEST_SHOBJFLAGS([-fPIC], [-DPIC], [-shared -mimpure-text], [
	    DC_TEST_SHOBJFLAGS([-fPIC], [-DPIC], [-shared -rdynamic -Wl,-G,-z,textoff], [
	      DC_TEST_SHOBJFLAGS([-fPIC], [-DPIC], [-shared -Wl,-G], [
		DC_TEST_SHOBJFLAGS([-fPIC], [-DPIC], [-shared -dynamiclib -flat_namespace -undefined suppress -bind_at_load], [
		  DC_TEST_SHOBJFLAGS([-fPIC], [-DPIC], [-dynamiclib -flat_namespace -undefined suppress -bind_at_load], [
		    DC_TEST_SHOBJFLAGS([-fPIC], [-DPIC], [-Wl,-dynamiclib -Wl,-flat_namespace -Wl,-undefined,suppress -Wl,-bind_at_load], [
		      DC_TEST_SHOBJFLAGS([-fPIC], [-DPIC], [-dynamiclib -flat_namespace -undefined suppress], [
		        DC_TEST_SHOBJFLAGS([-fPIC], [-DPIC], [-dynamiclib], [
		          AC_MSG_RESULT(cant)
		          AC_MSG_ERROR([We are unable to make shared objects.])
                        ])
		      ])
		    ])
		  ])
		])
	      ])
	    ])
	  ])
	])
      ])
    ])
  fi

  AC_MSG_RESULT($SHOBJCPPFLAGS $SHOBJFLAGS $SHOBJLDFLAGS)

  DC_SYNC_SHLIBOBJS
])

AC_DEFUN([DC_SYNC_SHLIBOBJS], [
  AC_SUBST(SHLIBOBJS)
  SHLIBOBJS=""
  for obj in $LIB@&t@OBJS; do
    SHLIBOBJS="$SHLIBOBJS `echo $obj | sed 's/\.o$/_shr.o/g'`"
  done
])

AC_DEFUN([DC_SYNC_RPATH], [
  OLD_LDFLAGS="$LDFLAGS"

  for tryrpath in "-Wl,-rpath" "-Wl,--rpath" "-Wl,-R"; do
    LDFLAGS="$OLD_LDFLAGS $tryrpath -Wl,/tmp"
    AC_LINK_IFELSE(AC_LANG_PROGRAM([], [ return(0); ]), [
      rpathldflags="$tryrpath"
      break
    ])
  done
  unset tryrpath

  LDFLAGS="$OLD_LDFLAGS"
  unset OLD_LDFLAGS

  ADDLDFLAGS=""
  for opt in $LDFLAGS; do
    if echo "$opt" | grep '^-L' >/dev/null; then
      rpathdir=`echo "$opt" | sed 's@^-L *@@'`
      ADDLDFLAGS="$ADDLDFLAGS $rpathldflags -Wl,$rpathdir"
    fi
  done
  unset opt rpathldflags

  LDFLAGS="$LDFLAGS $ADDLDFLAGS"

  unset ADDLDFLAGS
])

AC_DEFUN([DC_CHK_OS_INFO], [
	AC_CANONICAL_HOST
	AC_SUBST(SHOBJEXT)
	AC_SUBST(SHOBJFLAGS)
	AC_SUBST(SHOBJCPPFLAGS)
	AC_SUBST(SHOBJLDFLAGS)
	AC_SUBST(CFLAGS)
	AC_SUBST(CPPFLAGS)
	AC_SUBST(AREXT)

	if test "$dc_cv_dc_chk_os_info_called" != '1'; then
		dc_cv_dc_chk_os_info_called='1'

		AC_MSG_CHECKING(host operating system)
		AC_MSG_RESULT($host_os)

		SHOBJEXT="so"
		AREXT="a"

		case $host_os in
			darwin*)
				SHOBJEXT="dylib"
				;;
			hpux*)
				case "$host_cpu" in
					ia64)
						SHOBJEXT="so"
						;;
					*)
						SHOBJEXT="sl"
						;;
				esac
				;;
			aix[0-9].*)
				SHOBJEXT="a"
				;;
			mingw32|mingw32msvc*)
				SHOBJEXT="dll"
				CFLAGS="$CFLAGS -mms-bitfields"
				CPPFLAGS="$CPPFLAGS -mms-bitfields"
				SHOBJCPPFLAGS="-DPIC"
				SHOBJLDFLAGS='-shared -Wl,--dll -Wl,--enable-auto-image-base -Wl,--output-def,$[@].def,--out-implib,$[@].a'
				;;
			msvc)
				SHOBJEXT="dll"
				CFLAGS="$CFLAGS -nologo"
				SHOBJCPPFLAGS='-DPIC'
				SHOBJLDFLAGS='/LD /LINK /NODEFAULTLIB:MSVCRT'
				;;
			cygwin*)
				SHOBJEXT="dll"
				SHOBJFLAGS="-fPIC"
				SHOBJCPPFLAGS="-DPIC"
				CFLAGS="$CFLAGS -mms-bitfields"
				CPPFLAGS="$CPPFLAGS -mms-bitfields"
				SHOBJLDFLAGS='-shared -Wl,--enable-auto-image-base -Wl,--output-def,$[@].def,--out-implib,$[@].a'
				;;
		esac
	fi
])

AC_DEFUN(SHOBJ_SET_SONAME, [
	SAVE_LDFLAGS="$LDFLAGS"

	AC_MSG_CHECKING([how to specify soname])

	for try in "-Wl,--soname,$1" "Wl,-install_name,$1" '__fail__'; do
		LDFLAGS="$SAVE_LDFLAGS"

		if test "${try}" = '__fail__'; then
			AC_MSG_RESULT([can't])

			break
		fi

		LDFLAGS="${LDFLAGS} ${try}"
		AC_TRY_LINK([void TestTest(void) { return; }], [], [
			AC_MSG_RESULT([$try])

			break
		])
	done
])
