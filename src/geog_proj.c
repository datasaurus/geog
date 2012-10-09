/*
   -	geog_proj.c --
   -		This file defines functions that convert back and forth between
   -		latitude longitude pairs and map points.  See the user
   -		documentation for more information.
   - 
   .	Copyright (c) 2012 Gordon D. Carrie.  All rights reserved.
   .	
   .	Redistribution and use in source and binary forms, with or without
   .	modification, are permitted provided that the following conditions
   .	are met:
   .	
   .	    * Redistributions of source code must retain the above copyright
   .	    notice, this list of conditions and the following disclaimer.
   .
   .	    * Redistributions in binary form must reproduce the above copyright
   .	    notice, this list of conditions and the following disclaimer in the
   .	    documentation and/or other materials provided with the distribution.
   .	
   .	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   .	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   .	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   .	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   .	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   .	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
   .	TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   .	PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   .	LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   .	NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   .	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
   .
   .	Please send feedback to dev0@trekix.net
   .
   .	$Revision: $ $Date: $
 */

/*

   Ref.
   Snyder, John P.
   Map Projections used by the U.S. Geological Survey.
   (Geological Survey bulletin ; 1532)
   United States Government Printing Office, Washington:  1982.

 */

#include <math.h>
#include <stdio.h>
#include "geog_lib.h"
#include "geog_proj.h"

static struct GeogProj refPtProj(double, double);

int GeogProjXYToLonLat(double x, double y, double *lon_p, double *lat_p,
	struct GeogProj *projPtr)
{
    if (projPtr->rotation != 0) {
	double x_;

	x_ =  x * projPtr->cosr - y * projPtr->sinr;
	y = x * projPtr->sinr + y * projPtr->cosr;
	x = x_;
    }
    switch (projPtr->type) {
	case CylEqDist:
	    {
		double r0 = GeogREarth(NULL);
		double refLon = projPtr->params.RefPt.refLon;
		double cosRLat = projPtr->params.RefPt.cosRLat;

		*lon_p = GeogLonR(x / cosRLat / r0, refLon);
		*lat_p = y / r0;
	    }
	    break;
	case CylEqArea:
	    {
		double r0 = GeogREarth(NULL);
		double refLon = projPtr->params.RefLon;
		double r;

		r = y / r0;
		if (fabs(r) > 1.0) {
		    return 0;
		}
		*lat_p = asin(r);
		*lon_p = GeogLonR(x / r0, refLon);
	    }
	    break;
	case Mercator:
	    {
		double r0 = GeogREarth(NULL);
		double refLon = projPtr->params.RefLon;

		*lon_p = GeogLonR(x / r0, refLon);
		*lat_p = 2.0 * (atan(exp(y / r0)) - M_PI_4);
	    }
	    break;
	case LambertConfConic:
	    {
		double refLon = projPtr->params.LambertConfConic.refLon;
		double RF = projPtr->params.LambertConfConic.RF;
		double n = projPtr->params.LambertConfConic.n;
		double rho0 = projPtr->params.LambertConfConic.rho0;
		double dy, rho, theta;

		dy = n > 0.0 ? rho0 - y : y - rho0;
		rho = n > 0.0 ? sqrt(x * x + dy * dy) : -sqrt(x * x + dy * dy);
		theta = atan2(x, dy);
		x = n > 0.0 ? x : -x;
		*lon_p = GeogLonR(refLon + theta / n, refLon);
		*lat_p = 2.0 * atan(pow(RF / rho, 1.0 / n)) - M_PI_2;
	    }
	    break;
	case LambertEqArea:
	    {
		double r0 = GeogREarth(NULL);
		double refLon = projPtr->params.RefPt.refLon;
		double refLat = projPtr->params.RefPt.refLat;
		double cosRLat = projPtr->params.RefPt.cosRLat;
		double sinRLat = projPtr->params.RefPt.sinRLat;
		double rho, c, cosC, sinC, ord, lon;

		rho = sqrt(x * x + y * y);
		if (rho == 0.0) {
		    *lon_p = refLon;
		    *lat_p = refLat;
		    return 1;
		}
		if (rho > 2.0 * r0) {
		    return 0;
		}
		c = 2.0 * asin(rho / (2.0 * r0));
		cosC = cos(c);
		sinC = sin(c);
		ord = cosC * sinRLat + (y * sinC * cosRLat / rho);
		if (ord > 1.0)  {
		    return 0;
		}
		lon = atan2(x * sinC,
			(rho * cosRLat * cosC - y * sinRLat * sinC));
		lon += refLon;
		*lon_p = GeogLonR(lon, refLon);
		*lat_p = asin(ord);
	    }
	    break;
	case Orthographic:
	    {
		double r0 = GeogREarth(NULL);
		double refLon = projPtr->params.RefPt.refLon;
		double refLat = projPtr->params.RefPt.refLat;
		double cosRLat = projPtr->params.RefPt.cosRLat;
		double sinRLat = projPtr->params.RefPt.sinRLat;
		double r;		/* Polar radius */
		double c;		/* Angular distance from point to
					   reference point */
		double cosC, sinC, ord;	/* Computational constants */
		double lon;

		r = sqrt(x * x + y * y);
		if (r == 0.0) {
		    *lon_p = refLon;
		    *lat_p = refLat;
		    return 1;
		}
		if (r / r0 > 1.0) {
		    return 0;
		}
		c = asin(r / r0);
		cosC = cos(c);
		sinC = sin(c);
		ord = cosC * sinRLat + (y * sinC * cosRLat / r);
		if (ord > 1.0) {
		    return 0;
		}
		lon = atan2(x * sinC,
			(r * cosRLat * cosC - y * sinRLat * sinC));
		lon += refLon;
		*lon_p = GeogLonR(lon, refLon);
		*lat_p = asin(ord);
	    }
	    break;
	case Stereographic:
	    {
		double r0 = GeogREarth(NULL);
		double refLon = projPtr->params.RefPt.refLon;
		double refLat = projPtr->params.RefPt.refLat;
		double cosRLat = projPtr->params.RefPt.cosRLat;
		double sinRLat = projPtr->params.RefPt.sinRLat;
		double rho, c, cosC, sinC, ord;
		double lon;

		rho = sqrt(x * x + y * y);
		if (rho == 0.0) {
		    *lon_p = refLon;
		    *lat_p = refLat;
		    return 1;
		}
		c = 2.0 * atan2(rho, 2.0 * r0);
		cosC = cos(c);
		sinC = sin(c);
		ord = cosC * sinRLat + (y * sinC * cosRLat / rho);
		if (ord > 1.0) {
		    return 0;
		}
		lon = atan2(x * sinC,
			rho * cosRLat * cosC - y * sinRLat * sinC);
		lon += refLon;
		*lon_p = GeogLonR(lon, refLon);
		*lat_p = asin(ord);
	    }
	    break;
    }
    return 1;
}

int GeogProjLonLatToXY(double lon, double lat, double *x_p, double *y_p,
	struct GeogProj *projPtr)
{
    switch (projPtr->type) {
	case CylEqDist:
	    {
		double r0 = GeogREarth(NULL);
		double refLon = projPtr->params.RefPt.refLon;
		double cosRLat = projPtr->params.RefPt.cosRLat;

		lon = GeogLonR(lon, refLon);
		*x_p = lon * cosRLat * r0;
		*y_p = lat * r0;
	    }
	    break;
	case CylEqArea:
	    {
		double r0 = GeogREarth(NULL);
		double refLon = projPtr->params.RefLon;

		lon = GeogLonR(lon, refLon);
		*x_p = r0 * lon;
		*y_p = r0 * sin(lat);
	    }
	    break;
	case Mercator:
	    {
		double r0 = GeogREarth(NULL);
		double refLon = projPtr->params.RefLon;
		double limit;

		limit = M_PI_2 * 8.0 / 9.0;	/* 80 degrees */
		if ( fabs(lat) > limit ) {
		    return 0;
		}
		lon = GeogLonR(lon, refLon);
		*x_p = r0 * lon;
		*y_p = r0 * log(tan(M_PI_4 + 0.5 * lat));

	    }
	    break;
	case LambertConfConic:
	    {
		double refLon = projPtr->params.LambertConfConic.refLon;
		double refLat = projPtr->params.LambertConfConic.refLat;
		double RF = projPtr->params.LambertConfConic.RF;
		double n = projPtr->params.LambertConfConic.n;
		double rho0 = projPtr->params.LambertConfConic.rho0;
		double rho, theta;

		lon = GeogLonR(lon, refLon);
		if (lat == M_PI_2 ) {
		    if ( refLat < 0.0 ) {
			return 0;
		    }
		    rho = 0.0;
		} else if ( lat == -M_PI_2 ) {
		    if ( refLat > 0.0 ) {
			return 0;
		    }
		    rho = 0.0;
		} else {
		    rho = RF / pow(tan(M_PI_4 + 0.5 * lat), n);
		}
		theta = n * (lon - refLon);
		*x_p = rho * sin(theta);
		*y_p = rho0 - rho * cos(theta);
	    }
	    break;
	case LambertEqArea:
	    {
		double r0 = GeogREarth(NULL);
		double refLon = projPtr->params.RefPt.refLon;
		double refLat = projPtr->params.RefPt.refLat;
		double cosRLat = projPtr->params.RefPt.cosRLat;
		double sinRLat = projPtr->params.RefPt.sinRLat;
		double k, dlon;
		double cosLat, sinLat, cosDLon;

		cosLat = cos(lat);
		sinLat = sin(lat);
		dlon = lon - refLon;
		cosDLon = cos(dlon);
		if ( GeogDist(refLon, refLat, lon, lat) > M_PI_2 ) {
		    return 0;
		}
		k = 1.0 + sinRLat * sinLat + cosRLat * cosLat * cosDLon;
		k = sqrt(2.0 / k);
		*x_p = r0 * k * cosLat * sin(dlon);
		*y_p = r0 * k * (cosRLat * sinLat - sinRLat * cosLat * cosDLon);
	    }
	    break;
	case Orthographic:
	    {
		double r0 = GeogREarth(NULL);
		double refLon = projPtr->params.RefPt.refLon;
		double refLat = projPtr->params.RefPt.refLat;
		double cosRLat = projPtr->params.RefPt.cosRLat;
		double sinRLat = projPtr->params.RefPt.sinRLat;
		double coslat, dlon;

		if ( GeogDist(refLon, refLat, lon, lat) > M_PI_2 ) {
		    return 0;
		}
		coslat = cos(lat);
		dlon = lon - refLon;
		*x_p = r0 * coslat * sin(dlon);
		*y_p = r0 * (cosRLat * sin(lat) - sinRLat * coslat * cos(dlon));
	    }
	    break;
	case Stereographic:
	    {
		double r0 = GeogREarth(NULL);
		double refLon = projPtr->params.RefPt.refLon;
		double refLat = projPtr->params.RefPt.refLat;
		double cosRLat = projPtr->params.RefPt.cosRLat;
		double sinRLat = projPtr->params.RefPt.sinRLat;
		double dlon, cosDLon, k, cosLat, sinLat;

		/*
		   Go to 90 degrees from reference point (i.e. follow convention
		   and treat as hemisphere projection)
		 */

		if ( GeogDist(refLon, refLat, lon, lat) > M_PI_2 ) {
		    return 0;
		}
		cosLat = cos(lat);
		sinLat = sin(lat);
		dlon = lon - refLon;
		cosDLon = cos(dlon);
		k = 2.0 / (1.0 + sinRLat * sinLat + cosRLat * cosLat * cosDLon);
		*x_p = r0 * k * cosLat * sin(dlon);
		*y_p = r0 * k * (cosRLat * sinLat - sinRLat * cosLat * cosDLon);
	    }
	    break;
    }
    if (projPtr->rotation != 0) {
	double x = *x_p, y = *y_p;
	double x_;

	x_ = x * projPtr->cosr + y * projPtr->sinr;
	*y_p = y * projPtr->cosr - x * projPtr->sinr;
	*x_p = x_;
    }
    return 1;
}

void GeogProjSetRotation(struct GeogProj *projPtr, double angle)
{
    projPtr->rotation = angle;
    projPtr->cosr = cos(angle);
    projPtr->sinr = sin(angle);
}

int GeogProjSetFmStr(char *l, struct GeogProj *projPtr)
{
    double refLon, refLat;

    if ( sscanf(l, "CylEqDist %lf %lf", &refLon, &refLat) == 2 ) {
	*projPtr = GeogProjSetCylEqDist(refLon, refLat);
	return 1;
    } else if ( sscanf(l, "CylEqArea %lf", &refLon) == 1 ) {
	*projPtr = GeogProjSetCylEqArea(refLon);
	return 1;
    } else if ( sscanf(l, "Mercator %lf", &refLon) == 1 ) {
	*projPtr = GeogProjSetMercator(refLon);
	return 1;
    } else if ( sscanf(l, "LambertConfConic %lf %lf", &refLon, &refLat) == 2 ) {
	*projPtr = GeogProjSetLambertConfConic(refLon, refLat);
	return 1;
    } else if ( sscanf(l, "LambertEqArea %lf %lf", &refLon, &refLat) == 2 ) {
	*projPtr = GeogProjSetLambertEqArea(refLon, refLat);
	return 1;
    } else if ( sscanf(l, "Stereographic %lf %lf", &refLon, &refLat) == 2 ) {
	*projPtr = GeogProjSetStereographic(refLon, refLat);
	return 1;
    } else if ( sscanf(l, "Orthographic %lf %lf", &refLon, &refLat) == 2 ) {
	*projPtr = GeogProjSetOrthographic(refLon, refLat);
	return 1;
    }
    return 0;
}

struct GeogProj GeogProjSetCylEqDist(double refLon, double refLat)
{
    struct GeogProj proj;

    proj = refPtProj(refLon, refLat);
    proj.type = CylEqDist;
    return proj;
}

struct GeogProj GeogProjSetMercator(double refLon)
{
    struct GeogProj proj;

    proj.type = Mercator;
    proj.params.RefLon = GeogLonR(refLon, 0.0);
    proj.rotation = 0.0;
    proj.cosr = 1.0;
    proj.sinr = 0.0;
    return proj;
}

struct GeogProj GeogProjSetCylEqArea(double refLon)
{
    struct GeogProj proj;

    proj.type = CylEqArea;
    proj.params.RefLon = GeogLonR(refLon, 0.0);
    proj.rotation = 0.0;
    proj.cosr = 1.0;
    proj.sinr = 0.0;
    return proj;
}

struct GeogProj GeogProjSetLambertConfConic(double refLon, double refLat)
{
    struct GeogProj proj;
    double phi0, n, tan_n, RF, rho0;
    double r0 = GeogREarth(NULL);

    proj.type = LambertConfConic;
    proj.params.LambertConfConic.refLat = GeogLatN(refLat);
    if ( proj.params.LambertConfConic.refLat == 0.0 ) {
	return GeogProjSetMercator(refLon);
    }
    proj.params.LambertConfConic.refLon = GeogLonR(refLon, 0.0);
    phi0 = proj.params.LambertConfConic.refLat;
    n = sin(phi0);
    tan_n = pow(tan(M_PI_4 + 0.5 * phi0), n);
    RF = r0 * cos(phi0) * tan_n / n;
    rho0 = r0 / tan(phi0);
    proj.params.LambertConfConic.n = n;
    proj.params.LambertConfConic.RF = RF;
    proj.params.LambertConfConic.rho0 = rho0;
    proj.rotation = 0.0;
    proj.cosr = 1.0;
    proj.sinr = 0.0;
    return proj;
}

struct GeogProj GeogProjSetLambertEqArea(double refLon, double refLat)
{
    struct GeogProj proj;

    proj = refPtProj(refLon, refLat);
    proj.type = LambertEqArea;
    return proj;
}

struct GeogProj GeogProjSetStereographic(double refLon, double refLat)
{
    struct GeogProj proj;

    proj = refPtProj(refLon, refLat);
    proj.type = Stereographic;
    return proj;
}

struct GeogProj GeogProjSetOrthographic(double refLon, double refLat)
{
    struct GeogProj proj;

    proj = refPtProj(refLon, refLat);
    proj.type = Orthographic;
    return proj;
}

/*
   Set members for a reference point based projection.
 */

static struct GeogProj refPtProj(double refLon, double refLat)
{
    struct GeogProj proj;

    refLat = GeogLatN(refLat);
    proj.params.RefPt.refLat = refLat;
    proj.params.RefPt.cosRLat = cos(refLat);
    proj.params.RefPt.sinRLat = sin(refLat);
    proj.params.RefPt.refLon = GeogLonR(refLon, 0.0);
    proj.rotation = 0.0;
    proj.cosr = 1.0;
    proj.sinr = 0.0;
    return proj;
}
