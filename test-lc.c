#include "compat.h"
#include "libconfig.h"

int sally_cmd(const char *arg, const char *val, lc_flags_t flags) {
	PRINTERR_D("%s sets value: \"%s\" (flags=%i)", arg, val, flags);
	return(0);
}

int main(int argc, char **argv) {
	char *joeval = NULL;
	long long xval = -1;
	int onoff = -1;

	lc_register_var("joe", LC_VAR_STRING, &joeval, 'j');
	lc_register_var("bob", LC_VAR_SIZE_LONG_LONG, &xval, '\0');
	lc_register_var("test", LC_VAR_BOOL, &onoff, '\0');
	lc_register_callback("sally", sally_cmd);
	lc_process(argc, argv, "testapp", LC_CONF_SECTION, "test.cfg");

	if (joeval != NULL) {
		SPOTVAR_S(joeval);
	}
	SPOTVAR_LLU(xval);
	SPOTVAR_I(onoff);

	return(0);
}
