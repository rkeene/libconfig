AC_REVISION($Revision $)
AC_INIT(libconfig, 0.1.6)
AC_CONFIG_HEADER(config.h)

dnl Find out about the host OS
DC_CHK_OS_INFO

dnl Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_RANLIB
AC_AIX
AC_CHECK_TOOL(AR, ar, true)

AC_CHECK_HEADERS(ctype.h dirent.h pwd.h stdio.h stdlib.h string.h sys/stat.h sys/time.h sys/types.h time.h unistd.h)
AC_HEADER_TIME

DC_DO_TYPE(uint64_t, unsigned, 8)
DC_DO_TYPE(int64_t, signed, 8)
DC_DO_TYPE(uint32_t, unsigned, 4)
DC_DO_TYPE(int32_t, signed, 4)
DC_DO_TYPE(uint16_t, unsigned, 2)
DC_DO_TYPE(int16_t, signed, 2)

AC_REPLACE_FUNCS(strsep strtoll)
AC_CHECK_FUNCS(getpwuid)

DC_ASK_OPTLIB(opennet, fopen_net, opennet.h, [      Enable opennet support], libopennet, HAVE_LIBOPENNET, HAVE_OPENNET_H)

dnl Checks for Win32 specific things.
DC_DO_WIN32

dnl This must be last.
DC_GET_SHOBJFLAGS

AC_OUTPUT(Makefile lc_geterrno.3 lc_process.3 lc_register_var.3 lc_geterrstr.3 lc_register_callback.3 lc_cleanup.3 lc_process_file.3 libconfig.3)