/*
   geog_lib.c --

   	This file defines functions that do geography calculations and
	manage geographic data.

	$Id: geog_lib.c,v 1.3 2008/08/20 03:31:09 tkgeomap Exp $
 */

#include <math.h>
#include "geog.h"

double rlon(const double l, const double r)
{
    l = fmod(l, 360.0);
    return (l < r - 180.0) ? l + 360.0 : (l >= r + 180.0) ? l - 360.0 : l;
}
