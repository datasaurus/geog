/*
   -	geog_lib.c --
   -		This file defines functions that do geography
   -		calculations and manage geographic data.
   -		See geog_lib (3).
   -
   .	Copyright (c) 2008 Gordon D. Carrie
   .	All rights reserved.
   .
   .	Please send feedback to dev0@trekix.net
   .
   .	$Revision: 1.20 $ $Date: 2009/10/07 17:06:47 $
 */

#include <math.h>
#include "geog_lib.h"

/* Put l into [r - PI, r + PI) */
double LonToRef(const double l, const double r)
{
    double l1 = fmod(l, PI2);
    l1 = (l1 < r - PI) ? l1 + PI2 : (l1 >= r + PI) ? l1 - PI2 : l1;
    return (l1 == -0.0) ? 0.0 : l1;
}

/* Go l radians north of equator */
double LatN(const double l)
{
    double l1 = fmod(l, PI2);
    l1 += (l1 < 0.0) ? PI2 : 0.0;
    return (l1 > PI3_2) ? l1 - PI2 : (l1 > PI_2 ) ? PI - l1 : l1;
}

/* Great circle distance in radians between two points */
double GCDistR(const double lat1, const double lon1,
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
    return (a > 1.0 ? PI : 2.0 * asin(a));
}
