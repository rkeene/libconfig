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

AC_DEFUN(DC_GET_SHOBJFLAGS, [
  AC_SUBST(SHOBJFLAGS)
  AC_SUBST(SHOBJLDFLAGS)

  AC_MSG_CHECKING(how to create shared objects)
  OLD_LDFLAGS="$LDFLAGS"
  SHOBJFLAGS=""
  if test -z "$SHOBJFLAGS"; then
    LDFLAGS="$OLD_LDFLAGS -shared -Wl,-G,-z,textoff -rdynamic -fPIC"
    AC_TRY_LINK([#include <stdio.h>
int unrestst(void);], [ printf("okay\n"); unrestst(); return(0); ], [ SHOBJFLAGS="-shared -Wl,-G,-z,textoff -fPIC"; SHOBJLDFLAGS="-rdynamic" ], SHOBJFLAGS="", SHOBJFLAGS="");
  fi
  if test -z "$SHOBJFLAGS"; then
    LDFLAGS="$OLD_LDFLAGS -shared -Wl,-G,-z,textoff -rdynamic"
    AC_TRY_LINK([#include <stdio.h>
int unrestst(void);], [ printf("okay\n"); unrestst(); return(0); ], [ SHOBJFLAGS="-shared -Wl,-G,-z,textoff"; SHOBJLDFLAGS="-rdynamic" ], SHOBJFLAGS="", SHOBJFLAGS="");
  fi
  if test -z "$SHOBJFLAGS"; then
    LDFLAGS="$OLD_LDFLAGS -shared -Wl,-G,-z,textoff -fPIC"
    AC_TRY_LINK([#include <stdio.h>
int unrestst(void);], [ printf("okay\n"); unrestst(); return(0); ], [ SHOBJFLAGS="-shared -Wl,-G,-z,textoff -fPIC"; SHOBJLDFLAGS="" ], SHOBJFLAGS="", SHOBJFLAGS="");
  fi
  if test -z "$SHOBJFLAGS"; then
    LDFLAGS="$OLD_LDFLAGS -shared -Wl,-G,-z,textoff"
    AC_TRY_LINK([#include <stdio.h>
int unrestst(void);], [ printf("okay\n"); unrestst(); return(0); ], [ SHOBJFLAGS="-shared -Wl,-G,-z,textoff"; SHOBJLDFLAGS="" ], SHOBJFLAGS="", SHOBJFLAGS="");
  fi
  if test -z "$SHOBJFLAGS"; then
    LDFLAGS="$OLD_LDFLAGS -Wl,-G,-z,textoff -rdynamic -fPIC"
    AC_TRY_LINK([#include <stdio.h>
int unrestst(void);], [ printf("okay\n"); unrestst(); return(0); ], [ SHOBJFLAGS="-Wl,-G,-z,textoff -fPIC"; SHOBJLDFLAGS="-rdynamic" ], SHOBJFLAGS="", SHOBJFLAGS="");
  fi
  if test -z "$SHOBJFLAGS"; then
    LDFLAGS="$OLD_LDFLAGS -Wl,-G,-z,textoff -rdynamic"
    AC_TRY_LINK([#include <stdio.h>
int unrestst(void);], [ printf("okay\n"); unrestst(); return(0); ], [ SHOBJFLAGS="-Wl,-G,-z,textoff"; SHOBJLDFLAGS="-rdynamic" ], SHOBJFLAGS="", SHOBJFLAGS="");
  fi
  if test -z "$SHOBJFLAGS"; then
    LDFLAGS="$OLD_LDFLAGS -Wl,-G,-z,textoff -fPIC"
    AC_TRY_LINK([#include <stdio.h>
int unrestst(void);], [ printf("okay\n"); unrestst(); return(0); ], [ SHOBJFLAGS="-Wl,-G,-z,textoff -fPIC"; SHOBJLDFLAGS="" ], SHOBJFLAGS="", SHOBJFLAGS="");
  fi
  if test -z "$SHOBJFLAGS"; then
    LDFLAGS="$OLD_LDFLAGS -Wl,-G,-z,textoff"
    AC_TRY_LINK([#include <stdio.h>
int unrestst(void);], [ printf("okay\n"); unrestst(); return(0); ], [ SHOBJFLAGS="-Wl,-G,-z,textoff"; SHOBJLDFLAGS="" ], SHOBJFLAGS="", SHOBJFLAGS="");
  fi
  if test -z "$SHOBJFLAGS"; then
    LDFLAGS="$LDFLAGS -shared -rdynamic -fPIC"
    AC_TRY_LINK([#include <stdio.h>
int unrestst(void);], [ printf("okay\n"); unrestst(); return(0); ], [ SHOBJFLAGS="-shared -fPIC"; SHOBJLDFLAGS="-rdynamic" ], SHOBJFLAGS="", SHOBJFLAGS="");
  fi
  if test -z "$SHOBJFLAGS"; then
    LDFLAGS="$OLD_LDFLAGS -shared -fPIC"
    AC_TRY_LINK([#include <stdio.h>
int unrestst(void);], [ printf("okay\n"); unrestst(); return(0); ], [ SHOBJFLAGS="-shared -fPIC"; SHOBJLDFLAGS="" ], SHOBJFLAGS="", SHOBJFLAGS="");
  fi
  if test -z "$SHOBJFLAGS"; then
    LDFLAGS="$OLD_LDFLAGS -shared"
    AC_TRY_LINK([#include <stdio.h>
int unrestst(void);], [ printf("okay\n"); unrestst(); return(0); ], [ SHOBJFLAGS="-shared"; SHOBJLDFLAGS="" ], SHOBJFLAGS="", SHOBJFLAGS="");
  fi
  LDFLAGS="$OLD_LDFLAGS"
  AC_MSG_RESULT($SHOBJLDFLAGS $SHOBJFLAGS)
])
