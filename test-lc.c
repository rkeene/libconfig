#include "compat.h"
#include "libconfig.h"

int help_cmd(const char *partarg, const char *arg, const char *argarg, const char *val, lc_flags_t flags, void *extra) {
	printf("Usage info goes here\n");
	printf("\n");

	exit(EXIT_FAILURE);
}
int sally_cmd(const char *partarg, const char *arg, const char *argarg, const char *val, lc_flags_t flags, void *extra) {
	PRINTERR("%s sets value: \"%s\" (flags=%i)", arg, val, flags);
	return(0);
}

int cmd_ifmodule(const char *partarg, const char *arg, const char *argarg, const char *val, lc_flags_t flags, void *extra) {
	if (flags == LC_FLAGS_SECTIONEND) {
		return(LC_CBRET_OKAY);
	}
	if (flags != LC_FLAGS_SECTIONSTART) {
		PRINTERR("IfModule can only be used as a section.");
		return(LC_CBRET_ERROR);
	}
	if (argarg == NULL) {
		PRINTERR("You must specify an argument to IfModule.");
		return(LC_CBRET_ERROR);
	}

	PRINTERR("IfModule (%s)", argarg);
	return(LC_CBRET_IGNORESECTION);
}

int main(int argc, char **argv) {
	char *joeval = NULL;
	long long xval = -1;
	int onoff = -1;
	int lcpret = -1;
	int i = 0;
	lc_err_t errs;

	lc_register_var("Section", LC_VAR_SECTION, NULL, 0);
	lc_register_var("Somesection", LC_VAR_SECTION, NULL, 0);
	lc_register_var("Section.Test", LC_VAR_STRING, &joeval, 'j');
	lc_register_var("bob", LC_VAR_SIZE_LONG_LONG, &xval, 's');
	lc_register_var("Somesection.Free", LC_VAR_BOOL, &onoff, 0);
	lc_register_callback("sally", 0, LC_VAR_STRING, sally_cmd, NULL);
	lc_register_callback("HELP", 'h', LC_VAR_NONE, help_cmd, NULL);
	lc_register_callback("*.ifmodule", 0, LC_VAR_NONE, cmd_ifmodule, NULL);
	lcpret = lc_process(argc, argv, "testapp", LC_CONF_APACHE, "test.cfg");
	if (lcpret < 0) {
		PRINTERR("Error processing config file: %s", lc_geterrstr());
		return(EXIT_FAILURE);
	}

	if (joeval != NULL) {
		PRINTERR("joeval = \"%s\"", joeval);
	} else {
		PRINTERR("joeval = \"(null)\"");
	}
	PRINTERR("xval = %lli", xval);
	PRINTERR("onoff = %i", onoff);
	for (i = lc_optind; i < argc; i++) {
		PRINTERR("argv[%i] = \"%s\"", i, argv[i]);
	}

	return(0);
}
