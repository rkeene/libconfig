AC_DEFUN(DC_DO_WIN32, [
  AC_CHECK_HEADERS(windows.h windowsx.h)
])

AC_DEFUN(DC_DO_TYPE, [
	if test -z "$ac_cv_sizeof_long"; then
		AC_C_INLINE
		AC_CHECK_SIZEOF(long long, 8)
		AC_CHECK_SIZEOF(long, 4)
		AC_CHECK_SIZEOF(int, 4)
		AC_CHECK_SIZEOF(short, 2)
	fi
	FOUND=0
	for dc_cv_loop in \$ac_cv_sizeof_long_long \$ac_cv_sizeof_int \$ac_cv_sizeof_long \$ac_cv_sizeof_short; do
		dc_cv_size=`eval echo $dc_cv_loop`
		dc_cv_name=`echo $dc_cv_loop | sed s/\\\$ac_cv_sizeof_//`
		if test "$dc_cv_size" = "$3"; then
			if test "$dc_cv_name" = "int"; then 
				AC_CHECK_TYPE($1, $2 int)
			fi
			if test "$dc_cv_name" = "long"; then
				AC_CHECK_TYPE($1, $2 long)
			fi
			if test "$dc_cv_name" = "long_long"; then
				AC_CHECK_TYPE($1, $2 long long)
			fi
			if test "$dc_cv_name" = "short"; then
				AC_CHECK_TYPE($1, $2 short)
			fi
			FOUND=1
			break
		fi
	done
])

dnl Usage:
dnl    DC_TEST_SHOBJFLAGS(shobjflags, shobjldflags, action-if-not-found)
dnl
AC_DEFUN(DC_TEST_SHOBJFLAGS, [
  AC_SUBST(SHOBJFLAGS)
  AC_SUBST(SHOBJLDFLAGS)

  OLD_LDFLAGS="$LDFLAGS"
  SHOBJFLAGS=""

  LDFLAGS="$OLD_LDFLAGS $1 $2"

  AC_TRY_LINK([#include <stdio.h>
int unrestst(void);], [ printf("okay\n"); unrestst(); return(0); ], [ SHOBJFLAGS="$1"; SHOBJLDFLAGS="$2" ], [
  LDFLAGS="$OLD_LDFLAGS"
  $3
])

  LDFLAGS="$OLD_LDFLAGS"
])

AC_DEFUN(DC_GET_SHOBJFLAGS, [
  AC_SUBST(SHLIBOBJS)

  AC_MSG_CHECKING(how to create shared objects)

  DC_TEST_SHOBJFLAGS([-shared -fPIC], [-rdynamic], [
    DC_TEST_SHOBJFLAGS([-shared -fPIC], [], [
      DC_TEST_SHOBJFLAGS([-shared -fPIC], [-rdynamic -mimpure-text], [
        DC_TEST_SHOBJFLAGS([-shared -fPIC], [-mimpure-text], [
          DC_TEST_SHOBJFLAGS([-shared -fPIC], [-rdynamic -Wl,-G,-z,textoff], [
            DC_TEST_SHOBJFLAGS([-shared -fPIC], [-Wl,-G,-z,textoff], [
              DC_TEST_SHOBJFLAGS([-shared -fPIC], [-dynamiclib -flat_namespace -undefined suppress -bind_at_load], [
                DC_TEST_SHOBJFLAGS([-fPIC], [-dynamiclib -flat_namespace -undefined suppress -bind_at_load], [
                  AC_MSG_ERROR(cant)
                ])
              ])
            ])
          ])
        ])
      ])
    ])
  ])

  for obj in $LIB@&t@OBJS; do
    SHLIBOBJS="$SHLIBOBJS `echo $obj | sed 's/\.o$/_shr.o/g'`"
  done

  AC_MSG_RESULT($SHOBJLDFLAGS $SHOBJFLAGS)
])


AC_DEFUN(DC_CHK_OS_INFO, [
	AC_CANONICAL_HOST
	AC_SUBST(SHOBJEXT)
	AC_SUBST(AREXT)

        AC_MSG_CHECKING(host operating system)
        AC_MSG_RESULT($host_os)

	SHOBJEXT="so"
	AREXT="a"

        case $host_os in
                darwin*)
			SHOBJEXT="dylib"
                        ;;
		hpux*)
			SHOBJEXT="sl"
			;;
	esac
])

