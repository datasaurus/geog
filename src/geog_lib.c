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
   .	$Revision: 1.24 $ $Date: 2009/11/24 18:20:45 $
 */

#include <math.h>
#include "geog_lib.h"

/* Put l into [r - M_PI, r + M_PI) */
double LonToRef(const double l, const double r)
{
    double l1 = fmod(l, 2.0 * M_PI);
    l1 = (l1 < r - M_PI) ? l1 + 2.0 * M_PI
	: (l1 >= r + M_PI) ? l1 - 2.0 * M_PI : l1;
    return (l1 == -0.0) ? 0.0 : l1;
}

/* Go l radians north of equator */
double LatN(const double l)
{
    double l1 = fmod(l, 2.0 * M_PI);
    l1 += (l1 < 0.0) ? 2.0 * M_PI : 0.0;
    return (l1 > 1.5 * M_PI) ? l1 - 2.0 * M_PI : (l1 > M_PI_2 ) ? M_PI - l1 : l1;
}

/* Great circle distance in radians between two points */
double GCDistR(const double lat1, const double lon1,
	const double lat2, const double lon2)
{
    double sin_dlon_2, sin_dlat_2;
    double a;

    /*
       Reference -- R.W. Sinnott, "Virtues of the Haversine",
       Sky and Telescope, vol. 68, no. 2, 1984, p. 159
       cited in: http://www.census.gov/cgi-bin/geo/gisfaq?Q5.1
     */

    sin_dlon_2 = sin(0.5 * (lon2 - lon1));
    sin_dlat_2 = sin(0.5 * (lat2 - lat1));
    a = sqrt(sin_dlat_2 * sin_dlat_2
	+ cos(lat1) * cos(lat2) * sin_dlon_2 * sin_dlon_2);
    return (a > 1.0 ? M_PI : 2.0 * asin(a));
}

/* Compute lat-lon at given distance and direction from a point.  */
void GeogStep(double lon1, double lat1, double dirn, double dist,
	double *lon2, double *lat2)
{
    double sin_dist, sin_dirn, cos_dirn, dlon, a;

    /*
       Reference -- Smart, W. M., "Textbook on Spherical Astronomy",
       Sixth edition revised by R. M. Green.
       Cambridge University Press, Cambridge, 1977.
     */

    sin_dist = sin(dist);
    sin_dirn = sin(dirn);
    cos_dirn = cos(dirn);
    a = 0.5 * (sin(lat1 + dist) * (1.0 + cos_dirn)
	    + sin(lat1 - dist) * (1.0 - cos_dirn));
    *lat2 = (a > 1.0) ? M_PI_2 : (a < -1.0) ? -M_PI_2 : asin(a);
    dlon = atan2(sin_dist * sin_dirn, 0.5 * (cos(lat1 + dist) * (1 + cos_dirn)
		+ cos(lat1 - dist) * (1 - cos_dirn)));
    *lon2 = LonToRef(lon1 + dlon, 0.0);
}
