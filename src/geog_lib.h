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
   .	$Revision: 1.15 $ $Date: 2009/10/07 17:06:47 $
 */

#ifndef GEOG_LIB_H_
#define GEOG_LIB_H_

#define PI     3.141592653589793238462	/* pi */
#define PI_2   1.570796326794896619231	/* pi / 2 */
#define PI2    6.283185307179586476924	/* pi * 2 */
#define PI3_2  4.712388980384689857693	/* pi * 3 / 2 */

double LonToRef(const double, const double);
double LatN(const double);
double GCDistR(const double, const double, const double, const double);

#endif
