#include "compat.h"
#include "libconfig.h"
#include "libconfig_private.h"
#include "conf_section.h"
#include "conf_apache.h"
#include "conf_colon.h"
#include "conf_equal.h"
#include "conf_space.h"
#include "conf_xml.h"

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif

struct lc_varhandler_st *varhandlers = NULL;
lc_err_t lc_errno = LC_ERR_NONE;

static int lc_process_var_string(void *data, const char *value) {
	char **dataval;

	dataval = data;
	*dataval = strdup(value);

	return(0);
}

static int lc_process_var_longlong(void *data, const char *value) {
	long long *dataval;

	dataval = data;
	*dataval = strtoull(value, NULL, 10);

	return(0);
}

static int lc_process_var_long(void *data, const char *value) {
	long *dataval;

	dataval = data;
	*dataval = strtoull(value, NULL, 10);

	return(0);
}

static int lc_process_var_int(void *data, const char *value) {
	int *dataval;

	dataval = data;
	*dataval = strtoull(value, NULL, 10);

	return(0);
}

static int lc_process_var_short(void *data, const char *value) {
	short *dataval;

	dataval = data;
	*dataval = strtoull(value, NULL, 10);

	return(0);
}

static int lc_process_var_bool(void *data, const char *value) {
	int *dataval;

	dataval = data;

	*dataval = -1;

	if (strcasecmp(value, "enable") == 0 ||
	    strcasecmp(value, "true") == 0 ||
	    strcasecmp(value, "yes") == 0 ||
	    strcasecmp(value, "on") == 0 ||
            strcasecmp(value, "y") == 0 ||
	    strcasecmp(value, "1") == 0) {
		*dataval = 1;
		return(0);
	} else if (strcasecmp(value, "disable") == 0 ||
	    strcasecmp(value, "false") == 0 ||
	    strcasecmp(value, "off") == 0 ||
	    strcasecmp(value, "no") == 0 ||
            strcasecmp(value, "n") == 0 ||
	    strcasecmp(value, "0") == 0) {
		*dataval = 0;
		return(0);
	}

	lc_errno = LC_ERR_BADFORMAT;
	return(-1);
}

static long long lc_process_size(const char *value) {
	long long retval = -1;
	char *mult = NULL;

	retval = strtoull(value, &mult, 10);
	if (mult != NULL) {
		switch (tolower(mult[0])) {
			case 'p':
				retval *= 1125899906842624LLU;
				break;
			case 't':
				retval *= 1958505086976LLU;
				break;
			case 'g':
				retval *= 1073741824;
				break;
			case 'm':
				retval *= 1048576;
				break;
			case 'k':
				retval *= 1024;
				break;
			default:
				break;
		}
	}

	return(retval);
}

static int lc_process_var_sizelonglong(void *data, const char *value) {
	long long *dataval;

	dataval = data;
	*dataval = lc_process_size(value);

	return(0);
}

static int lc_process_var_sizelong(void *data, const char *value) {
	long *dataval;

	dataval = data;
	*dataval = lc_process_size(value);

	return(0);
}

static int lc_process_var_sizeint(void *data, const char *value) {
	int *dataval;

	dataval = data;
	*dataval = lc_process_size(value);

	return(0);
}

static int lc_process_var_sizeshort(void *data, const char *value) {
	short *dataval;

	dataval = data;
	*dataval = lc_process_size(value);

	return(0);
}

static int lc_handle_type(lc_var_type_t type, const char *value, void *data) {
	switch (type) {
		case LC_VAR_STRING:
			return(lc_process_var_string(data, value));
			break;
		case LC_VAR_LONG_LONG:
			return(lc_process_var_longlong(data, value));
			break;
		case LC_VAR_LONG:
			return(lc_process_var_long(data, value));
			break;
		case LC_VAR_INT:
			return(lc_process_var_int(data, value));
			break;
		case LC_VAR_SHORT:
			return(lc_process_var_short(data, value));
			break;
		case LC_VAR_BOOL:
			return(lc_process_var_bool(data, value));
			break;
		case LC_VAR_SIZE_LONG_LONG:
			return(lc_process_var_sizelonglong(data, value));
			break;
		case LC_VAR_SIZE_LONG:
			return(lc_process_var_sizelong(data, value));
			break;
		case LC_VAR_SIZE_INT:
			return(lc_process_var_sizeint(data, value));
			break;
		case LC_VAR_SIZE_SHORT:
			return(lc_process_var_sizeshort(data, value));
			break;
		case LC_VAR_TIME:
		case LC_VAR_DATE:
		case LC_VAR_FILENAME:
		case LC_VAR_DIRECTORY:
			PRINTERR_D("Not implemented yet!");
			return(-1);
		case LC_VAR_NONE:
		case LC_VAR_UNKNOWN:
		case LC_VAR_SECTION:
		case LC_VAR_SECTIONSTART:
		case LC_VAR_SECTIONEND:
			return(0);
			break;
	}

	return(-1);
}

static int lc_handle(struct lc_varhandler_st *handler, const char *var, const char *varargs, const char *value, lc_flags_t flags) {
	const char *localvar = NULL;
	int retval;

	if (var != NULL) {
		localvar = strrchr(var, '.');
		if (localvar == NULL) {
			localvar = var;
		} else {
			*localvar++;
		}
	} else {
		localvar = NULL;
	}

	switch (handler->mode) {
		case LC_MODE_CALLBACK:
			if (handler->callback != NULL) {
				retval = handler->callback(localvar, var, varargs, value, flags, handler->extra);
				if (retval < 0) {
					lc_errno = LC_ERR_CALLBACK;
				}
				return(retval);
			}
			break;
		case LC_MODE_VAR:
			return(lc_handle_type(handler->type, value, handler->data));
			break;
	}

	return(-1);
}

static int lc_process_environment(const char *appname) {
	/* XXX: write this */
	return(0);
}

static int lc_process_cmdline(int argc, char **argv) {
	struct lc_varhandler_st *handler = NULL;
	char *cmdarg = NULL, *cmdoptarg = NULL;
	char *lastcomponent_handler = NULL;
	int cmdargidx = 0;
	int retval = 0, chkretval = 0;
	int ch = 0;

	for (cmdargidx = 1; cmdargidx < argc; cmdargidx++) {
		cmdarg = argv[cmdargidx];

		/* Make sure we have an argument here. */
		if (cmdarg == NULL) {
			break;
		}

		/* If the argument isn't an option, abort. */
		if (cmdarg[0] != '-') {
			continue;
		}
		*cmdarg++;

		/* Handle long options. */
		if (cmdarg[0] == '-') {
			*cmdarg++;

			/* Don't process arguments after the '--' option. */
			if (cmdarg[0] == '\0') {
				break;
			}

			/* Look for a variable name that matches */
			for (handler = varhandlers; handler != NULL; handler = handler->_next) {
				/* Skip handlers with no variable name. */
				if (handler->var == NULL) {
					continue;
				}
				/* Skip handlers which don't agree with being
				   processed on the command line. */
				if (handler->type == LC_VAR_SECTION ||
				    handler->type == LC_VAR_SECTIONSTART ||
				    handler->type == LC_VAR_SECTIONEND ||
				    handler->type == LC_VAR_UNKNOWN) {
					continue;
				}

				/* Find the last part of the variable and compare it with 
				   the option being processed. */
				lastcomponent_handler = strrchr(handler->var, '.');
				if (lastcomponent_handler == NULL) {
					lastcomponent_handler = handler->var;
				} else {
					*lastcomponent_handler++;
				}

				/* Ignore this handler if they don't match. */
				if (strcasecmp(lastcomponent_handler, cmdarg) != 0) {
					continue;
				}

				if (handler->type == LC_VAR_NONE) {
					cmdoptarg = NULL;
				} else {
					cmdargidx++;
					if (cmdargidx >= argc) {
						PRINTERR("Argument required.");
						lc_errno = LC_ERR_BADFORMAT;
						return(-1);
					}
					cmdoptarg = argv[cmdargidx];
				}

				chkretval = lc_handle(handler, handler->var, NULL, cmdoptarg, LC_FLAGS_CMDLINE);
				if (chkretval < 0) {
					retval = -1;
				}

				break;
			}

			if (handler == NULL) {
				PRINTERR("Unknown option: --%s", cmdarg);
				lc_errno = LC_ERR_INVCMD;
				return(-1);
			}
		} else {
			for (; *cmdarg != '\0'; *cmdarg++) {
				ch = *cmdarg;

				for (handler = varhandlers; handler != NULL; handler = handler->_next) {
					if (handler->opt != ch || handler->opt == '\0') {
						continue;
					}
					/* Skip handlers which don't agree with being
					   processed on the command line. */
					if (handler->type == LC_VAR_SECTION ||
					    handler->type == LC_VAR_SECTIONSTART ||
					    handler->type == LC_VAR_SECTIONEND ||
					    handler->type == LC_VAR_UNKNOWN) {
						continue;
					}

					if (handler->type == LC_VAR_NONE) {
						cmdoptarg = NULL;
					} else {
						cmdargidx++;
						if (cmdargidx >= argc) {
							PRINTERR("Argument required.");
							lc_errno = LC_ERR_BADFORMAT;
							return(-1);
						}
						cmdoptarg = argv[cmdargidx];
					}

					chkretval = lc_handle(handler, handler->var, NULL, cmdoptarg, LC_FLAGS_CMDLINE);
					if (chkretval < 0) {
						retval = -1;
					}

					break;
				}

				if (handler == NULL) {
					PRINTERR("Unknown option: -%c", ch);
					lc_errno = LC_ERR_INVCMD;
					return(-1);
				}
			}
		}
	}

	return(retval);
}


int lc_process_var(const char *var, const char *varargs, const char *value, lc_flags_t flags) {
	struct lc_varhandler_st *handler = NULL;
	const char *lastcomponent_handler = NULL, *lastcomponent_var = NULL;

	lastcomponent_var = strrchr(var, '.');
	if (lastcomponent_var == NULL) {
		lastcomponent_var = var;
	} else {
		*lastcomponent_var++;
	}

	for (handler = varhandlers; handler != NULL; handler = handler->_next) {
		/* If either handler->var or var is NULL, skip, unless both are NULL. */
		if (handler->var != var && (handler->var == NULL || var == NULL)) {
			continue;
		}

		/* If both are not-NULL, compare them. */
		if (handler->var != NULL) {
			/* Wild-card-ish match. */
			if (handler->var[0] == '*' && handler->var[1] == '.') {
				/* Only compare the last components */

				lastcomponent_handler = strrchr(handler->var, '.') + 1; /* strrchr() won't return NULL, because we already checked it. */

				if (strcasecmp(lastcomponent_handler, lastcomponent_var) != 0) {
					continue;
				}
			} else if (strcasecmp(handler->var, var) != 0) {
				/* Exact (case-insensitive comparison) failed. */
				continue;
			}
		}

		return(lc_handle(handler, var, varargs, value, flags));
	}

	return(-1);
}

int lc_register_callback(const char *var, char opt, lc_var_type_t type, int (*callback)(const char *, const char *, const char *, const char *, lc_flags_t, void *), void *extra) {
	struct lc_varhandler_st *newhandler = NULL;

	newhandler = malloc(sizeof(*newhandler));

	if (newhandler == NULL) {
		return(-1);
	}

	if (var == NULL) {
		newhandler->var = NULL;
	} else {
		newhandler->var = strdup(var);
	}
	newhandler->mode = LC_MODE_CALLBACK;
	newhandler->type = type;
	newhandler->callback = callback;
	newhandler->opt = opt;
	newhandler->extra = extra;
	newhandler->_next = varhandlers;

	varhandlers = newhandler;

	return(0);
}

int lc_register_var(const char *var, lc_var_type_t type, void *data, char opt) {
	struct lc_varhandler_st *newhandler = NULL;

	newhandler = malloc(sizeof(*newhandler));

	if (newhandler == NULL) {
		return(-1);
	}

	if (var == NULL) {
		newhandler->var = NULL;
	} else {
		newhandler->var = strdup(var);
	}
	newhandler->mode = LC_MODE_VAR;
	newhandler->type = type;
	newhandler->data = data;
	newhandler->opt = opt;
	newhandler->extra = NULL;
	newhandler->_next = varhandlers;

	varhandlers = newhandler;

	return(0);
}

int lc_process(int argc, char **argv, const char *appname, lc_conf_type_t type, const char *extra) {
	int retval = 0, chkretval = 0;

	/* XXX Handle config files.  need to handle this in a loop of config files... */
	switch (type) {
		case LC_CONF_SECTION:
			chkretval = lc_process_conf_section(appname, extra);
			break;
		case LC_CONF_APACHE:
			chkretval = lc_process_conf_apache(appname, extra);
			break;
		case LC_CONF_COLON:
			chkretval = lc_process_conf_colon(appname, extra);
			break;
		case LC_CONF_EQUAL:
			chkretval = lc_process_conf_equal(appname, extra);
			break;
		case LC_CONF_SPACE:
			chkretval = lc_process_conf_space(appname, extra);
			break;
		case LC_CONF_XML:
			chkretval = lc_process_conf_xml(appname, extra);
			break;
		default:
			chkretval = -1;
			lc_errno = LC_ERR_INVDATA;
			break;
	}

	if (chkretval < 0) {
		retval = -1;
	}

	/* Handle environment variables.*/
	chkretval = lc_process_environment(appname);
	if (chkretval < 0) {
		retval = -1;
	}

	/* Handle command line arguments */
	chkretval = lc_process_cmdline(argc, argv);
	if (chkretval < 0) {
		retval = -1;
	}

	return(retval);
}


lc_err_t lc_geterrno(void) {
	lc_err_t retval;

	retval = lc_errno;

	lc_errno = LC_ERR_NONE;

	return(retval);
}

char *lc_geterrstr(void) {
	char *retval = NULL;

	switch (lc_errno) {
		case LC_ERR_NONE:
			retval = "Success";
			break;
		case LC_ERR_INVCMD:
			retval = "Invalid command";
			break;
		case LC_ERR_INVSECTION:
			retval = "Invalid section";
			break;
		case LC_ERR_INVDATA:
			retval = "Invalid application data (internal error)";
			break;
		case LC_ERR_BADFORMAT:
			retval = "Bad data specified or incorrect format.";
			break;
		case LC_ERR_CANTOPEN:
			retval = "Can't open file.";
			break;
		case LC_ERR_CALLBACK:
			retval = "Error return from application handler.";
			break;
	}

	lc_errno = LC_ERR_NONE;

	if (retval != NULL) {
		return(retval);
	}
	return("Unknown error");
}
