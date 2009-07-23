/*
 - geog.h --
 - 	This header declares structures and functions that store and
 -	manipulate geographic data.
 -
   Copyright (c) 2008 Gordon D. Carrie
   Licensed under the Open Software License version 3.0
  
   Please send feedback to dev0@trekix.net

   $Revision: 1.11 $ $Date: 2009/07/11 18:01:38 $
 */

#ifndef GEOG_LIB_H_
#define GEOG_LIB_H_

#define RADPERDEG 0.017453292519943294892
#define DEGPERRAD 57.29577951308232088

double lonr(const double, const double);
double plat(const double);
double gcdist(const double, const double, const double, const double);
double gcdistr(const double, const double, const double, const double);

#endif
