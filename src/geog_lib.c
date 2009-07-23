/*
 - geog_lib.c --
 - 	This file defines functions that do geography calculations and
 -	manage geographic data.  Re. geog_lib (3).
 -
   Copyright (c) 2008 Gordon D. Carrie
   Licensed under the Open Software License version 3.0

   Please send feedback to dev0@trekix.net

   $Revision: 1.16 $ $Date: 2009/07/20 20:32:56 $
 */

#include <math.h>
#include "geog_lib.h"

/* Put l into [r - 180.0, r + 180.0) */
double lonr(const double l, const double r)
{
    double l1 = fmod(l, 360.0);
    l1 = (l1 < r - 180.0) ? l1 + 360.0 : (l1 >= r + 180.0) ? l1 - 360.0 : l1;
    return (l1 == -0.0) ? 0.0 : l1;
}

/* Go l degrees north of equator */
double plat(const double l)
{
    double l1 = fmod(l, 360.0);
    l1 += (l1 < 0.0) ? 360.0 : 0.0;
    return (l1 > 270.0) ? l1 - 360.0
	: (l1 > 90.0 ) ? 180.0 - l1
	: l1;
}

/* Great circle distance in degrees between two points */
double gcdist(const double lat1, const double lon1,
	const double lat2, const double lon2)
{
    double sin_dlon_2, sin_dlat_2;
    double a;

    /* Reference: R.W. Sinnott, "Virtues of the Haversine", Sky and Telescope,
       vol. 68, no. 2, 1984, p. 159
       cited in: http://www.census.gov/cgi-bin/geo/gisfaq?Q5.1 */
    sin_dlon_2 = sin(0.5 * RADPERDEG * (lon2 - lon1));
    sin_dlat_2 = sin(0.5 * RADPERDEG * (lat2 - lat1));
    a = sqrt(sin_dlat_2 * sin_dlat_2
	+ cos(lat1) * cos(lat2) * sin_dlon_2 * sin_dlon_2);
    return DEGPERRAD * (a > 1.0 ? M_PI : 2.0 * asin(a));
}

/* Great circle distance in radians between two points */
double gcdistr(const double lat1, const double lon1,
	const double lat2, const double lon2)
{
    double sin_dlon_2, sin_dlat_2;
    double a;

    /* Reference: R.W. Sinnott, "Virtues of the Haversine", Sky and Telescope,
       vol. 68, no. 2, 1984, p. 159
       cited in: http://www.census.gov/cgi-bin/geo/gisfaq?Q5.1 */
    sin_dlon_2 = sin(0.5 * (lon2 - lon1));
    sin_dlat_2 = sin(0.5 * (lat2 - lat1));
    a = sqrt(sin_dlat_2 * sin_dlat_2
	+ cos(lat1) * cos(lat2) * sin_dlon_2 * sin_dlon_2);
    return (a > 1.0 ? M_PI : 2.0 * asin(a));
}
