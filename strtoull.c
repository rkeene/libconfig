#include <sys/types.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdio.h>

/* We only handle base 10. */
unsigned long long int strtoull(char *nptr, char **endptr, int base) {
	unsigned long long int retval = 0;
	char *idx = NULL;

	for (idx = nptr; *idx != '\0' && isdigit(*idx); idx++) {
		retval *= 10;
		retval += (*idx - '0');
	}

	if (endptr != NULL) {
		*endptr = idx;
	}

	return(retval);
}
