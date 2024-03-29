AC_INIT(libconfig, 0.3.3)
AC_CONFIG_HEADER(config.h)

dnl Find out about the host OS
DC_CHK_OS_INFO

dnl Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_RANLIB
AC_AIX
AC_CHECK_TOOL(AR, ar, true)

DC_DO_TYPE(uint64_t, unsigned, 8)
DC_DO_TYPE(int64_t, signed, 8)
DC_DO_TYPE(uint32_t, unsigned, 4)
DC_DO_TYPE(int32_t, signed, 4)
DC_DO_TYPE(uint16_t, unsigned, 2)
DC_DO_TYPE(int16_t, signed, 2)

dnl This will cause optional libraries to be disabled even if they're
dnl available and specified.
DC_ASK_SMALL

dnl Checks for Win32 specific things.
DC_DO_WIN32

dnl Get shared objects flags, calls DC_SYNC_SHLIBOBJS but if libobjs is
dnl changed later (by AC_REPLACE_FUNCS, for example) you must call
dnl DC_SYNC_SHLIBOBJS again.
DC_GET_SHOBJFLAGS

dnl This stuff has to come after the shobjtest to verify that it is correct
AC_CHECK_HEADERS(ctype.h dirent.h pwd.h stdio.h stdlib.h string.h sys/stat.h sys/time.h sys/types.h time.h unistd.h netdb.h netinet/in.h)
AC_SEARCH_LIBS(gethostbyname, [nsl socket ws2_32], [], [
	AC_MSG_ERROR([Unable to find a library supplying gethostbyname.])
])

AC_HEADER_TIME

AC_REPLACE_FUNCS(strsep strtoll getuid)
AC_CHECK_FUNCS(getpwuid strtof)

dnl Setup stable API
DC_SETUP_STABLE_API([${srcdir}/libconfig.vers], libconfig.syms)

dnl Set a soname
ABI_VERSION='0'
SHOBJ_SET_SONAME([libconfig.$SHOBJEXT.$ABI_VERSION])
AC_SUBST(ABI_VERSION)

dnl This MUST be last.
DC_SYNC_SHLIBOBJS

AC_OUTPUT(Makefile lc_geterrno.3 lc_process.3 lc_register_var.3 lc_geterrstr.3 lc_seterrstr.3 lc_register_callback.3 lc_cleanup.3 lc_process_file.3 libconfig.3 lc_handle_type.3 libconfig.syms)
