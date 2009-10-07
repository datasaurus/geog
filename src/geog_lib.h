/*
   -	geog.h --
   -		This header declares structures and functions
   -		that store and manipulate geographic data.
   -		See geog_lib (3).
   -	
   .	Copyright (c) 2008 Gordon D. Carrie
   .	All rights reserved.
   .
   .	Please send feedback to dev0@trekix.net
   .
   .	$Revision: 1.14 $ $Date: 2009/10/01 22:15:22 $
 */

#ifndef GEOG_LIB_H_
#define GEOG_LIB_H_

#define RADPERDEG 0.017453292519943294892
#define DEGPERRAD 57.29577951308232088

double LonToRef(const double, const double);
double PrincLat(const double);
double GCDist(const double, const double, const double, const double);
double GCDistR(const double, const double, const double, const double);

#endif
