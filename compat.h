#ifndef RSK_COMPAT_H
#define RSK_COMPAT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifdef __WIN32__
#include "win32.h"
#endif

#ifndef LC_LINEBUF_LEN
#define LC_LINEBUF_LEN 1024
#endif

#ifndef HAVE_GETOPT
#include "getopt.h"
#else
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif
#endif
#ifndef HAVE_STRSEP
#include "strsep.h"
#endif
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif
#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif

#ifdef DEBUG
#define PRINT_LINE fprintf(stderr, "%lu:%s:%i:%s(): ", (unsigned long) time(NULL), __FILE__, __LINE__, __func__); 
#define PRINTERR_D(x...) { PRINT_LINE; fprintf(stderr, x); fprintf(stderr, "\n"); fflush(stderr); }
#define PERROR(x) { PRINT_LINE; perror(x); fflush(stderr); }
#define DPERROR(x) PERROR(x)
#else
#define PRINT_LINE /**/
#define PRINTERR_D(x...) /**/
#define PERROR(x) perror(x);
#define DPERROR(x) /**/
#endif
#define CHECKPOINT PRINTERR_D("*** CHECKPOINT REACHED ***")
#define SPOTVAR_LLX(x) PRINTERR_D(#x " = %llx", x)
#define SPOTVAR_LLU(x) PRINTERR_D(#x " = %llu", x)
#define SPOTVAR_LU(x) PRINTERR_D(#x " = %lu", x)
#define SPOTVAR_D(x) PRINTERR_D(#x " = %lf", x)
#define SPOTVAR_X(x) PRINTERR_D(#x " = 0x%x", x)
#define SPOTVAR_I(x) PRINTERR_D(#x " = %i", x)
#define SPOTVAR_P(x) PRINTERR_D(#x " = %p", x)
#define SPOTVAR_S(x) PRINTERR_D(#x " = \"%s\"", x)
#define PRINTERR(x...) { PRINT_LINE; fprintf(stderr, x); fprintf(stderr, "\n"); fflush(stderr); }
#define LOG(x...) { PRINT_LINE; fprintf(stderr, "LOG: " x); fprintf(stderr, "\n"); }

#endif
