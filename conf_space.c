#include "compat.h"
#include "libconfig.h"
#include "libconfig_private.h"
#include "conf_space.h"

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

int lc_process_conf_space(const char *appname, const char *configfile) {
	FILE *configfp = NULL;
	char linebuf[LC_LINEBUF_LEN] = {0}, *linebuf_ptr = NULL;
	char *cmd = NULL, *value = NULL, *sep = NULL;
	char *fgetsret = NULL;
	int lcpvret = -1;

	if (appname == NULL || configfile == NULL) {
		return(-1);
	}

	configfp = fopen(configfile, "r");

	if (configfp == NULL) {
		return(-1);
	}

	while (1) {
		fgetsret = fgets(linebuf, sizeof(linebuf) - 1, configfp);
		if (fgetsret == NULL) {
			break;
		}
		if (feof(configfp)) {
			break;
		}

		linebuf_ptr = &linebuf[strlen(linebuf) - 1];
		while (*linebuf_ptr < ' ' && linebuf_ptr >= linebuf) {
			*linebuf_ptr = '\0';
			*linebuf_ptr--;
		}

		linebuf_ptr = &linebuf[0];
		while (*linebuf_ptr == ' ') {
			*linebuf_ptr++;
		}

		if (*linebuf_ptr == '#' || *linebuf_ptr == '\0') {
			continue;
		}


		sep = strpbrk(linebuf_ptr, " \t");
		cmd = linebuf_ptr;
		if (sep != NULL) {
			*sep = '\0';
			*sep++;
			while (*sep == ' ' || *sep == '\t') {
				*sep++;
			}
			value = sep;
		} else {
			value = NULL;
		}

		lcpvret = lc_process_var(cmd, NULL, value, LC_FLAGS_VAR);
		if (lcpvret < 0) {
			PRINTERR_D("Invalid command: \"%s\"", cmd);
		}
	}

	fclose(configfp);

	return(0);
}
