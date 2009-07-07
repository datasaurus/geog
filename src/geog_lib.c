/*
 - geog_lib.c --
 - 	This file defines functions that do geography calculations and
 -	manage geographic data.
 -
   Copyright (c) 2008 Gordon D. Carrie
   Licensed under the Open Software License version 3.0

   Please send feedback to dev0@trekix.net

   $Revision: 1.12 $ $Date: 2009/07/07 17:52:36 $
 */

#include <math.h>
#include "geog.h"

double lonr(const double l, const double r)
{
    double l1 = fmod(l, 360.0);
    l1 = (l1 < r - 180.0) ? l1 + 360.0 : (l1 >= r + 180.0) ? l1 - 360.0 : l1;
    return (l1 == -0.0) ? 0.0 : l1;
}

double plat(const double l)
{
    double l1 = fmod(l, 360.0);
    l1 += (l1 < 0.0) ? 360.0 : 0.0;
    return (l1 > 270.0) ? l1 - 360.0
	: (l1 > 90.0 ) ? 180.0 - l1
	: l1;
}
