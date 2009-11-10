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
   .	$Revision: 1.16 $ $Date: 2009/11/10 20:01:55 $
 */

#ifndef GEOG_LIB_H_
#define GEOG_LIB_H_

#ifndef M_PI
#define M_PI     3.141592653589793238462	/* pi */
#endif
#ifndef M_PI_2
#define M_PI_2   1.570796326794896619231	/* pi / 2 */
#endif

double LonToRef(const double, const double);
double LatN(const double);
double GCDistR(const double, const double, const double, const double);
void GeogStep(double, double, double, double, double *, double *);

#endif
