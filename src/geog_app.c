/*
 - geog_app.c --
 - 	This file defines an application that does geography calculations.
 -
   Copyright (c) 2008 Gordon D. Carrie
   Licensed under the Open Software License version 3.0
  
   Please send feedback to user0@tkgeomap.org

   $Id: geog_app.c,v 1.3 2008/08/20 03:09:39 tkgeomap Exp $
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
