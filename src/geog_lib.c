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
   .	$Revision: 1.26 $ $Date: 2009/12/03 22:28:38 $
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
double GCDistR(const double a1, const double o1, const double a2, const double o2)
{
    double sin_do_2, sin_da_2, a;

    /*
       Reference -- R.W. Sinnott, "Virtues of the Haversine",
       Sky and Telescope, vol. 68, no. 2, 1984, p. 159
       cited in: http://www.census.gov/cgi-bin/geo/gisfaq?Q5.1
     */

    sin_do_2 = sin(0.5 * (o2 - o1));
    sin_da_2 = sin(0.5 * (a2 - a1));
    a = sqrt(sin_da_2 * sin_da_2 + cos(a1) * cos(a2) * sin_do_2 * sin_do_2);
    return (a > 1.0 ? M_PI : 2.0 * asin(a));
}

/* Azimuth from (longitude, latitude): (o1, a1) to (o2, a2) */
double Azimuth(const double a1, const double o1, const double a2, const double o2)
{
    double sin_da, sin_sa, y, x;

    sin_da = sin(a1 - a2);
    sin_sa = sin(a2 + a1);
    y = cos(a2) * sin(o2 - o1);
    x = 0.5 * (sin_sa - sin_da - (sin_sa + sin_da) * cos(o2 - o1));
    return atan2(y, x);
}
/*
   Compute destination point longitude *o2, latitude *a2 at given separation s
   and direction d from point at longitude = o1, latitude a1.
 */
void GeogStep(double o1, double a1, double d, double s, double *o2, double *a2)
{
    double sin_s, sin_d, cos_d, dlon, a, x, y;

    /*
       Reference -- Smart, W. M., "Textbook on Spherical Astronomy",
       Sixth edition revised by R. M. Green.
       Cambridge University Press, Cambridge, 1977.
     */

    sin_s = sin(s);
    sin_d = sin(d);
    cos_d = cos(d);
    a = 0.5 * (sin(a1 + s) * (1.0 + cos_d) + sin(a1 - s) * (1.0 - cos_d));
    *a2 = (a > 1.0) ? M_PI_2 : (a < -1.0) ? -M_PI_2 : asin(a);
    y = sin_s * sin_d;
    x = 0.5 * (cos(a1 + s) * (1 + cos_d) + cos(a1 - s) * (1 - cos_d));
    dlon = atan2(y, x);
    *o2 = LonToRef(o1 + dlon, 0.0);
}
