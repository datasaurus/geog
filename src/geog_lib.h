/*
 - geog.h --
 - 	This header declares structures and functions that store and
 -	manipulate geographic data.
 -
   Copyright (c) 2008 Gordon D. Carrie
   Licensed under the Open Software License version 3.0
  
   Please send feedback to dev0@trekix.net

   $Revision: 1.10 $ $Date: 2009/07/07 20:24:48 $
 */

#ifndef GEOG_LIB_H_
#define GEOG_LIB_H_

double lonr(const double, const double);
double plat(const double);
double gcdist(const double, const double, const double, const double);

#endif
