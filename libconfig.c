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

int lc_process(int argc, char **argv, const char *appname, lc_conf_type_t type, const char *extra) {
	int retval = -1;

	switch (type) {
		case LC_CONF_SECTION:
			retval = lc_process_conf_section(appname, extra);
			break;
		case LC_CONF_APACHE:
			retval = lc_process_conf_apache(appname, extra);
			break;
		case LC_CONF_COLON:
			retval = lc_process_conf_colon(appname, extra);
			break;
		case LC_CONF_EQUAL:
			retval = lc_process_conf_equal(appname, extra);
			break;
		case LC_CONF_SPACE:
			retval = lc_process_conf_space(appname, extra);
			break;
		case LC_CONF_XML:
			retval = lc_process_conf_xml(appname, extra);
			break;
		default:
			break;
	}

	return(retval);
}

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
	} else if (strcasecmp(value, "disable") == 0 ||
	    strcasecmp(value, "false") == 0 ||
	    strcasecmp(value, "off") == 0 ||
	    strcasecmp(value, "no") == 0 ||
            strcasecmp(value, "n") == 0 ||
	    strcasecmp(value, "0") == 0) {
		*dataval = 0;
	}

	return(0);
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

static int lc_handle(struct lc_varhandler_st *handler, const char *value, lc_flags_t flags) {
	switch (handler->mode) {
		case LC_MODE_CALLBACK:
			if (handler->callback != NULL) {
				return(handler->callback(handler->var, value, flags));
			}
			break;
		case LC_MODE_VAR:
			switch (handler->type) {
				case LC_VAR_STRING:
					return(lc_process_var_string(handler->data, value));
					break;
				case LC_VAR_LONG_LONG:
					return(lc_process_var_longlong(handler->data, value));
					break;
				case LC_VAR_LONG:
					return(lc_process_var_long(handler->data, value));
					break;
				case LC_VAR_INT:
					return(lc_process_var_int(handler->data, value));
					break;
				case LC_VAR_SHORT:
					return(lc_process_var_short(handler->data, value));
					break;
				case LC_VAR_BOOL:
					return(lc_process_var_bool(handler->data, value));
					break;
				case LC_VAR_SIZE_LONG_LONG:
					return(lc_process_var_sizelonglong(handler->data, value));
					break;
				case LC_VAR_SIZE_LONG:
					return(lc_process_var_sizelong(handler->data, value));
					break;
				case LC_VAR_SIZE_INT:
					return(lc_process_var_sizeint(handler->data, value));
					break;
				case LC_VAR_SIZE_SHORT:
					return(lc_process_var_sizeshort(handler->data, value));
					break;
				case LC_VAR_TIME:
				case LC_VAR_DATE:
				case LC_VAR_FILENAME:
				case LC_VAR_DIRECTORY:
					PRINTERR_D("Not implemented yet!");
					return(-1);
				case LC_VAR_NONE:
				case LC_VAR_UNKNOWN:
				case LC_VAR_SECTIONSTART:
				case LC_VAR_SECTIONEND:
					return(0);
					break;
			}
		break;
	}

	return(-1);
}

int lc_process_var(const char *var, const char *varargs, const char *value, lc_flags_t flags) {
	struct lc_varhandler_st *handler = NULL;

	for (handler = varhandlers; handler != NULL; handler = handler->_next) {
		if (handler->var != var && (handler->var == NULL || var == NULL)) {
			continue;
		}
		if (handler->var != NULL) {
			if (strcasecmp(handler->var, var) != 0) {
				continue;
			}
		}

		return(lc_handle(handler, value, flags));
	}
	return(-1);
}

int lc_register_callback(const char *var, int (*callback)(const char *, const char *, lc_flags_t)) {
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
	newhandler->callback = callback;
	newhandler->type = LC_VAR_UNKNOWN;
	newhandler->mode = LC_MODE_CALLBACK;
	newhandler->opt = '\0';
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
	newhandler->type = type;
	newhandler->mode = LC_MODE_VAR;
	newhandler->data = data;
	newhandler->opt = opt;
	newhandler->_next = varhandlers;

	varhandlers = newhandler;

	return(0);
}
