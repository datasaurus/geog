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
   .	$Revision: 1.21 $ $Date: 2009/11/10 20:01:55 $
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

    /* Reference: R.W. Sinnott, "Virtues of the Haversine", Sky and Telescope,
       vol. 68, no. 2, 1984, p. 159
       cited in: http://www.census.gov/cgi-bin/geo/gisfaq?Q5.1 */
    sin_dlon_2 = sin(0.5 * (lon2 - lon1));
    sin_dlat_2 = sin(0.5 * (lat2 - lat1));
    a = sqrt(sin_dlat_2 * sin_dlat_2
	+ cos(lat1) * cos(lat2) * sin_dlon_2 * sin_dlon_2);
    return (a > 1.0 ? M_PI : 2.0 * asin(a));
}

/* Compute lat-lon at given distance and direction from a point.  */
void GeogStep(double lon0, double lat0, double dirn, double dist,
	double *lon1, double *lat1)
{
    double cos_dist, sin_dist, cos_dir, sin_dir;
    double cos_lat, cos_lon, sin_lon, sin_lat;
    double x, y, z, h_1, h_2, dh;

    cos_dist = cos(dist);
    sin_dist = sin(dist);
    cos_dir = cos(dirn);
    sin_dir = sin(dirn);

    cos_lat = cos(lat0);
    cos_lon = cos(lon0);
    sin_lon = sin(lon0);
    sin_lat = sin(lat0);
    x = cos_dist * cos_lon * cos_lat - sin_dir * sin_dist * sin_lon 
	- cos_lon * cos_dir * sin_dist * sin_lat;
    y = sin_dir * cos_lon * sin_dist + cos_dist * cos_lat * sin_lon 
	- cos_dir * sin_dist * sin_lon * sin_lat;
    *lon1 = atan2(y, x);

    z = cos_lat * cos_dir * sin_dist + cos_dist * sin_lat;
    h_1 = cos_dist * cos_lat - cos_dir * sin_dist * sin_lat;
    h_2 = sin_dir * sin_dist;
    dh = h_1 * h_1 + h_2 * h_2;
    *lat1 = (dh == 0.0) ? (z > 0 ? M_PI_2 : -M_PI_2) : atan(z / sqrt(dh));
}
