/*
   geog_app.c --

   	This file defines an application that does geography calculations.

	$Id$
 */

#include <stdlib.h>
#include <stdio.h>
#include <geog.h>

int main(void)
{
    double l, r;

    l = r = 0.0;
    while (scanf(" r=%lf", &r) == 1 || scanf(" l=%lf", &l) == 1) {
	printf("%g, r=%g => %g longitude\n", l, r, lonr(l, r));
	printf("Enter a number: ");
    }
    return 0;
}
