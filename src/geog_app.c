/*
   geog_app.c --

   	This file defines an application that does geography calculations.

	$Id: geog_app.c,v 1.2 2008/08/17 04:20:53 tkgeomap Exp $
 */

#include <stdlib.h>
#include <stdio.h>
#include "geog.h"

int main(void)
{
    double l, r;

    l = r = 0.0;
    while (scanf(" r=%lf", &r) == 1 || scanf(" l=%lf", &l) == 1) {
	printf("%g, r=%g => %g longitude\n", l, r, rlon(l, r));
	printf("Enter a number: ");
    }
    return 0;
}
