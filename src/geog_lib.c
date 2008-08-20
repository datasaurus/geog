/*
   geog_lib.c --

   	This file defines functions that do geography calculations and
	manage geographic data.

	$Id$
 */

#include <math.h>
#include "geog.h"

double rlon(double l, double r)
{
    l = fmod(l, 360.0);
    return (l < r - 180.0) ? l + 360.0 : (l >= r + 180.0) ? l - 360.0 : l;
}
