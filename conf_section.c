#include "compat.h"
#include "libconfig.h"
#include "libconfig_private.h"
#include "conf_section.h"

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

int lc_process_conf_section(const char *appname, const char *configfile) {
	FILE *configfp = NULL;
	char linebuf[LC_LINEBUF_LEN] = {0}, *linebuf_ptr = NULL;
	char qualifbuf[LC_LINEBUF_LEN] = {0};
	char *cmd = NULL, *value = NULL, *sep = NULL, *cmdend = NULL;
	char *currsection = NULL;
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

		/* Remove trailing crap (but not spaces). */
		linebuf_ptr = &linebuf[strlen(linebuf) - 1];
		while (*linebuf_ptr < ' ' && linebuf_ptr >= linebuf) {
			*linebuf_ptr = '\0';
			*linebuf_ptr--;
		}

		/* Handle section header. */
		if (linebuf[0] == '[' && linebuf[strlen(linebuf) - 1] == ']') {
			linebuf[strlen(linebuf) - 1] = '\0';
			linebuf_ptr = &linebuf[1];

			/* If a section was open, close it. */
			if (currsection != NULL) {
				lcpvret = lc_process_var(currsection, NULL, NULL, LC_FLAGS_SECTIONEND);
				if (lcpvret < 0) {
					PRINTERR_D("Invalid section terminating: \"%s\"", currsection);
				}
				free(currsection);
			}

			/* Open new section. */
			currsection = strdup(linebuf_ptr);
			lcpvret = lc_process_var(currsection, NULL, NULL, LC_FLAGS_SECTIONSTART);
			if (lcpvret < 0) {
				PRINTERR_D("Invalid section: \"%s\"", currsection);
			}
			continue;
		}

		/* Remove leading spaces. */
		linebuf_ptr = &linebuf[0];
		while (*linebuf_ptr == ' ') {
			*linebuf_ptr++;
		}

		/* Drop comments and blank lines. */
		if (*linebuf_ptr == ';' || *linebuf_ptr == '\0') {
			continue;
		}

		/* Find the command and the data in the line. */
		cmdend = sep = strpbrk(linebuf_ptr, "=");
		if (sep == NULL) {
			PRINTERR("Invalid line: \"%s\"", linebuf);
			continue;
		}

		/* Delete space at the end of the command. */
		*cmdend--; /* It currently derefs to the seperator.. */
		while (*cmdend <= ' ') {
			*cmdend = '\0';
			*cmdend--;
		}

		cmd = linebuf_ptr;

		/* Delete the seperator char and any leading space. */
		*sep = '\0';
		*sep++;
		while (*sep == ' ' || *sep == '\t') {
			*sep++;
		}
		value = sep;

		/* Create the fully qualified variable name. */
		if (currsection == NULL) {
			strncpy(qualifbuf, cmd, sizeof(qualifbuf) - 1);
		} else {
			snprintf(qualifbuf, sizeof(qualifbuf) - 1, "%s.%s", currsection, cmd);
		}

		/* Call the parent and tell them we have data. */
		lcpvret = lc_process_var(qualifbuf, NULL, value, LC_FLAGS_VAR);
		if (lcpvret < 0) {
			PRINTERR_D("Invalid command: \"%s\"", qualifbuf);
		}
	}

	/* Close any open section, and clean-up. */
	if (currsection != NULL) {
		lcpvret = lc_process_var(currsection, NULL, NULL, LC_FLAGS_SECTIONEND);
		if (lcpvret < 0) {
			PRINTERR_D("Invalid section terminating: \"%s\"", currsection);
		}
		free(currsection);
	}

	fclose(configfp);

	return(0);
}
