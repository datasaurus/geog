/*
   -	geog_proj.h --
   -
   -		This header file defines constants and declares structures and
   -		functions that manage and apply cartographic projections.
   .
   .	Copyright (c) 2012, Gordon D. Carrie. All rights reserved.
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

#ifndef _GEOGPROJ_H_
#define _GEOGPROJ_H_

#include "geog_lib.h"

/*
   The following constant identifies the currently recognized projection types.
 */

enum ProjType {
    CylEqDist, CylEqArea, Mercator, LambertConfConic,
    LambertEqArea, Orthographic, Stereographic
};

/*
   Structures of this store parameters and functions that making conversions
   between geographic and map coordinates. Clients should not refer to fields
   in this structure directly.
 */

struct GeogProj {
    enum ProjType type;			/* Projection type */
    union {
	double RefLon;			/* Reference longitude */
	struct {
	    double refLon, refLat;	/* Reference point */
	    double cosRLat;		/* Cosine of reference latitude */
	    double sinRLat;		/* Sine of reference latitude */
	} RefPt;
	struct {
	    double refLat;		/* Reference latitude */
	    double refLon;		/* Longitude of vertical meridian */
	    double n;			/* See Snyder, p. 105 */
	    double RF;			/* See Snyder, p. 105 */
	    double rho0;		/* See Snyder, p. 105 */
	} LambertConfConic;
    } params;
    double rotation;			/* Rotation angle (clockwise degrees).
					   This specifies the angle between
					   geographic north and north on the
					   map. It can also be regarded as the
					   orientation of the surface onto which
					   the Earth's surface is projected */
    double cosr, sinr;			/* Cosine and sine of rotation */
};

/*
   Global function declarations.
 */

int GeogProjXYToLonLat(double, double, double *, double *, struct GeogProj *);
int GeogProjLonLatToXY(double, double, double *, double *, struct GeogProj *);
struct GeogProj GeogProjSetCylEqDist(double, double);
struct GeogProj GeogProjSetCylEqArea(double);
struct GeogProj GeogProjSetMercator(double);
struct GeogProj GeogProjSetLambertConfConic(double, double);
struct GeogProj GeogProjSetLambertEqArea(double, double);
struct GeogProj GeogProjSetStereographic(double, double);
struct GeogProj GeogProjSetOrthographic(double, double);
void GeogProjSetRotation(struct GeogProj *, double);
int GeogProjSetFmStr(char *, struct GeogProj *);

#endif
