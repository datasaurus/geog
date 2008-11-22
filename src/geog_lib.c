/*
 - geog_lib.c --
 - 	This file defines functions that do geography calculations and
 -	manage geographic data.
 -
   Copyright (c) 2008 Gordon D. Carrie
   Licensed under the Open Software License version 3.0

   Please send feedback to user0@tkgeomap.org

   $Id: geog_lib.c,v 1.6 2008/08/30 16:02:06 tkgeomap Exp $
 */

#include <math.h>
#include "geog.h"

double rlon(const double l, const double r)
{
    double l1 = fmod(l, 360.0);
    return (l1 < r - 180.0) ? l1 + 360.0 : (l1 >= r + 180.0) ? l1 - 360.0 : l1;
}

double plat(const double l)
{
    double l1 = fmod(l, 360.0);
    return (l1 > 270.0) ? l1 - 360.0
	: (l1 > 180.0) ? l1 - 270.0
	: (l1 > 90.0 ) ? 180.0 - l1
	: l1;
}
