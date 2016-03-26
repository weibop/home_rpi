#include "hal_spi.h"
#include <stdlib.h>

void pabort(const char *s)
{
	perror(s);
	abort();// needs stdlib.h
}
