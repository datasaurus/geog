/*
   -	geog.h --
   -		Declarations of structures and functions
   -		that store and manipulate geographic data.
   -		See geog_lib (3).
   -	
   .	Copyright (c) 2008 Gordon D. Carrie
   .	All rights reserved.
   .
   .	Please send feedback to dev0@trekix.net
   .
   .	$Revision: 1.20 $ $Date: 2009/12/08 19:54:52 $
 */

#ifndef GEOG_LIB_H_
#define GEOG_LIB_H_

#ifndef M_PI
#define M_PI     3.141592653589793238462	/* pi */
#endif
#ifndef M_PI_2
#define M_PI_2   1.570796326794896619231	/* pi / 2 */
#endif
#ifndef RAD_DEG
#define RAD_DEG   0.01745329251994329576	/* radians / degree */
#endif
#ifndef DEG_RAD
#define DEG_RAD   57.29577951308232087680	/* degrees / radian */
#endif

double GeogLonR(const double, const double);
double GeogLatN(const double);
double GeogDist(const double, const double, const double, const double);
double GeogAz(const double, const double, const double, const double);
void GeogStep(const double, const double, const double, const double,
	double *, double *);

#endif
