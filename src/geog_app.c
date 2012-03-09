/*
   -	geog_app.c --
   -		This file defines an application that does geography calculations.
   -
   .	Copyright (c) 2011, Gordon D. Carrie. All rights reserved.
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
   .	$Revision: 1.44 $ $Date: 2012/02/24 22:42:36 $
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "alloc.h"
#include "err_msg.h"
#include "geog_lib.h"

/* Application name and subcommand name */
char *cmd, *cmd1;

/* Number of subcommands */
#define NCMD 11

/* Callback functions.  There should be one for each subcommand. */
typedef int (callback)(int , char **);
callback rearth_cb;
callback lonr_cb;
callback latn_cb;
callback dist_cb;
callback sum_dist_cb;
callback az_cb;
callback step_cb;
callback beam_ht_cb;
callback contain_pt_cb;
callback contain_pts_cb;
callback vproj_cb;

int main(int argc, char *argv[])
{
    int i;		/* Index for subcommand in argv[1] */
    int rslt;		/* Return code */

    /* Arrays of subcommand names and associated callbacks */
    char *cmd1v[NCMD] = {"rearth", "lonr", "latn", "dist", "sum_dist", "az",
	"step", "beam_ht", "contain_pt", "contain_pts", "vproj"};
    callback *cb1v[NCMD] = {rearth_cb, lonr_cb, latn_cb, dist_cb, sum_dist_cb,
	az_cb, step_cb, beam_ht_cb, contain_pt_cb, contain_pts_cb, vproj_cb};

    cmd = argv[0];
    if (argc < 2) {
	fprintf(stderr, "Usage: %s subcommand [subcommand_options ...]\n", cmd);
	exit(1);
    }
    cmd1 = argv[1];

    /* Search cmd1v for cmd1.  When match is found, evaluate the associated
     * callback from cb1v. */
    for (i = 0; i < NCMD; i++) {
	if (strcmp(cmd1v[i], cmd1) == 0) {
	    rslt = (cb1v[i])(argc, argv);
	    if ( !rslt ) {
		fprintf(stderr, "%s %s failed.\n", cmd, cmd1);
		fprintf(stderr, "%s\n", Err_Get());
		break;
	    } else {
		break;
	    }
	}
    }
    if (i == NCMD) {
	fprintf(stderr, "%s: No option or subcommand named %s\n", cmd, cmd1);
	fprintf(stderr, "Subcommand must be one of: ");
	for (i = 0; i < NCMD; i++) {
	    fprintf(stderr, "%s ", cmd1v[i]);
	}
	fprintf(stderr, "\n");
	rslt = 0;
    }
    return !rslt;
}

int rearth_cb(int argc, char *argv[])
{
    if (argc == 2) {
	printf("%f\n", GeogREarth(NULL));
    } else if (argc == 3) {
	char *r_s = argv[2];
	double r;

	if (sscanf(r_s, "%lf", &r) != 1) {
	    Err_Append("Expected float value for Earth radius, got ");
	    Err_Append(r_s);
	    Err_Append(".\n");
	    return 0;
	}
	printf("%f\n", GeogREarth(&r));
    } else {
	Err_Append("Usage: ");
	Err_Append(cmd);
	Err_Append(" ");
	Err_Append(cmd1);
	Err_Append(" [r]\n");
	return 0;
    }
    return 1;
}

int lonr_cb(int argc, char *argv[])
{
    char *l_s, *r_s;		/* Strings from command line */
    double l, r;		/* Values from command line */

    if (argc != 4) {
	Err_Append("Usage: ");
	Err_Append(cmd);
	Err_Append(" ");
	Err_Append(cmd1);
	Err_Append("lon reflon\n");
	return 0;
    }
    l_s = argv[2];
    if (sscanf(l_s, "%lf", &l) != 1) {
	Err_Append("Expected float value for longitude, got ");
	Err_Append(l_s);
	Err_Append(".\n");
	return 0;
    }
    r_s = argv[3];
    if (sscanf(r_s, "%lf", &r) != 1) {
	Err_Append("Expected float value for reference longitude, got ");
	Err_Append(r_s);
	Err_Append(".\n");
	return 0;
    }
    printf("%lf\n", GeogLonR(l * RAD_DEG, r * RAD_DEG) * DEG_RAD);
    return 1;
}

int latn_cb(int argc, char *argv[])
{
    char *l_s;			/* String from command line */
    double l;			/* Latitude value from command line */

    if (argc != 3) {
	Err_Append("Usage: ");
	Err_Append(cmd);
	Err_Append(" ");
	Err_Append(cmd1);
	Err_Append(" lat\n");
	return 0;
    }
    l_s = argv[2];
    if (sscanf(l_s, "%lf", &l) != 1) {
	Err_Append("Expected float value for latitude, got ");
	Err_Append(l_s);
	Err_Append(".\n");
	return 0;
    }
    printf("%f\n", GeogLatN(l * RAD_DEG) * DEG_RAD);
    return 1;
}

int dist_cb(int argc, char *argv[])
{
    char *lon1_s, *lat1_s, *lon2_s,  *lat2_s;
    double lon1, lat1, lon2, lat2;

    if (argc != 6) {
	Err_Append("Usage: ");
	Err_Append(cmd);
	Err_Append(" ");
	Err_Append(cmd1);
	Err_Append(" lon1 lat1 lon2 lat2\n");
	return 0;
    }
    lon1_s = argv[2];
    lat1_s = argv[3];
    lon2_s = argv[4];
    lat2_s = argv[5];

    /* Get coordinates from command line arguments */
    if (sscanf(lon1_s, "%lf", &lon1) != 1) {
	Err_Append("Expected float value for lon1, got ");
	Err_Append(lon1_s);
	Err_Append(".\n");
	return 0;
    }
    if (sscanf(lat1_s, "%lf", &lat1) != 1) {
	Err_Append("Expected float value for lat1, got ");
	Err_Append(lat1_s);
	Err_Append(".\n");
	return 0;
    }
    if (sscanf(lon2_s, "%lf", &lon2) != 1) {
	Err_Append("Expected float value for lon2, got ");
	Err_Append(lon2_s);
	Err_Append(".\n");
	return 0;
    }
    if (sscanf(lat2_s, "%lf", &lat2) != 1) {
	Err_Append("Expected float value for lat2, got ");
	Err_Append(lat2_s);
	Err_Append(".\n");
	return 0;
    }
    printf("%f\n", GeogDist(lon1 * RAD_DEG, lat1 * RAD_DEG,
		lon2 * RAD_DEG, lat2 * RAD_DEG) * DEG_RAD);
    return 1;
}

/*
   Compute length of track given as lon lat pairs in stdin.
   Input and output are in degrees.
 */

int sum_dist_cb(int argc, char *argv[])
{
    double lon0, lon, lat0, lat;	/* Longitude, latitude from input */
    double tot;				/* Total distance */

    if (argc != 2) {
	Err_Append("Usage: ");
	Err_Append(cmd);
	Err_Append(" ");
	Err_Append(cmd1);
	return 0;
    }

    if (scanf(" %lf %lf", &lon0, &lat0) != 2) {
	Err_Append("No input. ");
	return 0;
    }
    for (tot = 0.0; scanf(" %lf %lf", &lon, &lat) == 2 ; ) {
	tot += GeogDist(lon0 * RAD_DEG, lat0 * RAD_DEG,
		lon * RAD_DEG, lat * RAD_DEG);
	lon0 = lon;
	lat0 = lat;
    }
    printf("%lf\n", tot * DEG_RAD);
    return 1;
}

int az_cb(int argc, char *argv[])
{
    char *lon1_s, *lat1_s, *lon2_s,  *lat2_s;
    double lon1, lat1, lon2, lat2;

    if (argc != 6) {
	Err_Append("Usage: ");
	Err_Append(cmd);
	Err_Append(" ");
	Err_Append(cmd1);
	Err_Append("lat1 lon1 lat2 lon2\n");
	return 0;
    }
    lon1_s = argv[2];
    lat1_s = argv[3];
    lon2_s = argv[4];
    lat2_s = argv[5];

    /* Get coordinates from command line arguments */
    if (sscanf(lon1_s, "%lf", &lon1) != 1) {
	Err_Append("Expected float value for lon1, got ");
	Err_Append(lon1_s);
	Err_Append(".\n");
	return 0;
    }
    if (sscanf(lat1_s, "%lf", &lat1) != 1) {
	Err_Append("Expected float value for lat1, got ");
	Err_Append(lat1_s);
	Err_Append(".\n");
	return 0;
    }
    if (sscanf(lon2_s, "%lf", &lon2) != 1) {
	Err_Append("Expected float value for lon2, got ");
	Err_Append(lon2_s);
	Err_Append(".\n");
	return 0;
    }
    if (sscanf(lat2_s, "%lf", &lat2) != 1) {
	Err_Append("Expected float value for lat2, got ");
	Err_Append(lat2_s);
	Err_Append(".\n");
	return 0;
    }
    printf("%f\n", GeogAz(lon1 * RAD_DEG, lat1 * RAD_DEG,
		lon2 * RAD_DEG,  lat2 * RAD_DEG) * DEG_RAD);
    return 1;
}

int step_cb(int argc, char *argv[])
{
    double lon1, lat1, dirn, dist, lon2, lat2;

    if (argc == 2) {
	while (scanf("%lf %lf %lf %lf", &lon1, &lat1, &dirn, &dist) == 4) {
	    GeogStep(lon1 * RAD_DEG, lat1 * RAD_DEG,
		    dirn * RAD_DEG, dist * RAD_DEG, &lon2, &lat2);
	    printf("%f %f\n", lon2 * DEG_RAD, lat2 * DEG_RAD);
	}
    } else if (argc == 6) {
	char *lat1_s, *lon1_s, *dirn_s, *dist_s;

	lon1_s = argv[2];
	if (sscanf(lon1_s, "%lf", &lon1) != 1) {
	    Err_Append("Expected float value for lon1, got ");
	    Err_Append(lon1_s);
	    Err_Append(".\n");
	    return 0;
	}
	lat1_s = argv[3];
	if (sscanf(lat1_s, "%lf", &lat1) != 1) {
	    Err_Append("Expected float value for lat1, got ");
	    Err_Append(lat1_s);
	    Err_Append(".\n");
	    return 0;
	}
	dirn_s = argv[4];
	if (sscanf(dirn_s, "%lf", &dirn) != 1) {
	    Err_Append("Expected float value for azimuth, got ");
	    Err_Append(dirn_s);
	    Err_Append(".\n");
	    return 0;
	}
	dist_s = argv[5];
	if (sscanf(dist_s, "%lf", &dist) != 1) {
	    Err_Append("Expected float value for range, got ");
	    Err_Append(dist_s);
	    Err_Append(".\n");
	    return 0;
	}
	GeogStep(lon1 * RAD_DEG, lat1 * RAD_DEG,
		dirn * RAD_DEG, dist * RAD_DEG, &lon2, &lat2);
	printf("%f %f\n", lon2 * DEG_RAD, lat2 * DEG_RAD);
    } else {
	Err_Append("Usage: ");
	Err_Append(cmd);
	Err_Append(" ");
	Err_Append(cmd1);
	Err_Append(" [lon lat direction distance]\n");
	return 0;
    }
    return 1;
}

int beam_ht_cb(int argc, char *argv[])
{
    char *d_s, *tilt_s, *a0_s;
    double d, tilt, a0;

    if (argc != 5) {
	Err_Append("Usage: ");
	Err_Append(cmd);
	Err_Append(" ");
	Err_Append(cmd1);
	Err_Append(" distance tilt earth_radius ");
	return 0;
    }

    d_s = argv[2];
    tilt_s = argv[3];
    a0_s = argv[4];

    if (sscanf(d_s, "%lf", &d) != 1) {
	Err_Append("Expected float value for distance, got ");
	Err_Append(d_s);
	Err_Append(".\n");
	return 0;
    }
    if (sscanf(tilt_s, "%lf", &tilt) != 1) {
	Err_Append("Expected float value for distance, got ");
	Err_Append(tilt_s);
	Err_Append(".\n");
	return 0;
    }
    if (sscanf(a0_s, "%lf", &a0) != 1) {
	Err_Append("Expected float value for distance, got ");
	Err_Append(a0_s);
	Err_Append(".\n");
	return 0;
    }

    printf("%lf\n", GeogBeamHt(d, tilt * RAD_DEG, a0));
    return 1;
}

int contain_pt_cb(int argc, char *argv[])
{
    char *lon_s, *lat_s;
    char **lon_sp, **lat_sp;
    struct GeogPt pt, *pts, *pts_p;
    size_t n_pts;

    if ( argc < 10 || argc % 2 != 0 ) {
	Err_Append("Usage: ");
	Err_Append(cmd);
	Err_Append(" ");
	Err_Append(cmd1);
	Err_Append(" lon lat lon1 lat1 lon2 lat2 ...");
	return 0;
    }
    lon_s = argv[2];
    lat_s = argv[3];
    if ( sscanf(lon_s, "%lf", &pt.lon) != 1 ) {
	Err_Append("Expected double value for longitude, got ");
	Err_Append(lon_s);
	Err_Append(". ");
	return 0;
    }
    pt.lon *= RAD_DEG;
    if ( sscanf(lat_s, "%lf", &pt.lat) != 1 ) {
	Err_Append("Expected double value for latitude, got ");
	Err_Append(lat_s);
	Err_Append(". ");
	return 0;
    }
    pt.lat *= RAD_DEG;
    n_pts = (argc - 4) / 2;
    if ( !(pts = CALLOC(n_pts, sizeof(struct GeogPt))) ) {
	Err_Append("Could not allocate memory for polygon. ");
	return 0;
    }
    for (lon_sp = argv + 4, lat_sp = argv + 5, pts_p = pts;
	    lat_sp < argv + argc; lon_sp += 2, lat_sp += 2, pts_p++) {
	if ( sscanf(*lon_sp, "%lf", &pts_p->lon) != 1 ) {
	    Err_Append("Expected double value for longitude, got ");
	    Err_Append(*lon_sp);
	    Err_Append(". ");
	    return 0;
	}
	pts_p->lon *= RAD_DEG;
	if ( sscanf(*lat_sp, "%lf", &pts_p->lat) != 1 ) {
	    Err_Append("Expected double value for latitude, got ");
	    Err_Append(*lat_sp);
	    Err_Append(". ");
	    return 0;
	}
	pts_p->lat *= RAD_DEG;
    }
    printf("%s\n", GeogContainPt(pt, pts, n_pts) ? "in" : "out");
    return 1;
}

#define LEN 1024
int contain_pts_cb(int argc, char *argv[])
{
    char **lon_sp, **lat_sp;
    struct GeogPt pt, *pts, *pts_p;
    size_t n_pts;
    char buf[LEN];

    if ( argc < 8 || argc % 2 != 0 ) {
	Err_Append("Usage: ");
	Err_Append(cmd);
	Err_Append(" ");
	Err_Append(cmd1);
	Err_Append(" lon1 lat1 lon2 lat2 ...");
	return 0;
    }
    n_pts = (argc - 2) / 2;
    if ( !(pts = CALLOC(n_pts, sizeof(struct GeogPt))) ) {
	Err_Append("Could not allocate memory for polygon. ");
	return 0;
    }
    for (lon_sp = argv + 2, lat_sp = argv + 3, pts_p = pts;
	    lat_sp < argv + argc; lon_sp += 2, lat_sp += 2, pts_p++) {
	if ( sscanf(*lon_sp, "%lf", &pts_p->lon) != 1 ) {
	    Err_Append("Expected double value for longitude, got ");
	    Err_Append(*lon_sp);
	    Err_Append(". ");
	    return 0;
	}
	pts_p->lon *= RAD_DEG;
	if ( sscanf(*lat_sp, "%lf", &pts_p->lat) != 1 ) {
	    Err_Append("Expected double value for latitude, got ");
	    Err_Append(*lat_sp);
	    Err_Append(". ");
	    return 0;
	}
	pts_p->lat *= RAD_DEG;
    }
    while ( fgets(buf, LEN, stdin) ) {
	if ( sscanf(buf, " %lf %lf ", &pt.lon, &pt.lat) == 2 ) {
	    pt.lon *= RAD_DEG;
	    pt.lat *= RAD_DEG;
	    if ( GeogContainPt(pt, pts, n_pts) ) {
		fputs(buf, stdout);
	    }
	}
    }
    return 1;
}

/*
   This subcommand projects points onto a vertical plane.

   geog vproj lon1 lat1 lon2 lat2 dist

   The vertical plane rises above the great circle connecting points
   (lon1 lat1) to (lon2 lat2), with given separation dist, in some distance
   unit.  The process will then read from standard input line of form:

   lon lat z

   Giving the longitude, latitude, and altitude of points to project.
   Altitude must use the same unit as dist. For each line of input,
   standard output will receive a line of form:

   x z

   giving the horizontal and vertical coordinates on the vertical plane in a
   coordinate system with origin lon1 lat1 z=0.
 */

int vproj_cb(int argc, char *argv[])
{
    char *lon1_s, *lat1_s, *lon2_s, *lat2_s, *dist_s;
    double lon1, lat1, lon2, lat2, dist, z;
    double azg;			/* Azimuth from (lon1 lat1) to (lon2 lat2) */
    double lon, lat;
    double az;
    double a0;			/* Earth radius */
    double d;			/* Distance along ground from origin to point */
    double x;			/* Abscissa of event on vertical plane */

    if ( argc != 7 ) {
	Err_Append("Usage: ");
	Err_Append(cmd);
	Err_Append(" ");
	Err_Append(cmd1);
	Err_Append(" lon1 lat1 lon2 lat2 distance");
	return 0;
    }
    lon1_s = argv[2];
    lat1_s = argv[3];
    lon2_s = argv[4];
    lat2_s = argv[5];
    dist_s = argv[6];
    if ( sscanf(lon1_s, "%lf", &lon1) != 1 ) {
	Err_Append("Expected float value for longitude of first point, got ");
	Err_Append(lon1_s);
	return 0;
    }
    if ( sscanf(lat1_s, "%lf", &lat1) != 1 ) {
	Err_Append("Expected float value for latitude of first point, got ");
	Err_Append(lat1_s);
	return 0;
    }
    if ( sscanf(lon2_s, "%lf", &lon2) != 1 ) {
	Err_Append("Expected float value for longitude of second point, got ");
	Err_Append(lon2_s);
	return 0;
    }
    if ( sscanf(lat2_s, "%lf", &lat2) != 1 ) {
	Err_Append("Expected float value for latitude of second point, got ");
	Err_Append(lat1_s);
	return 0;
    }
    if ( sscanf(dist_s, "%lf", &dist) != 1 ) {
	Err_Append("Expected float value for distance between points, got ");
	Err_Append(dist_s);
	return 0;
    }
    lon1 *= RAD_DEG;
    lat1 *= RAD_DEG;
    lon2 *= RAD_DEG;
    lat2 *= RAD_DEG;
    a0 = dist / GeogDist(lon1, lat1, lon2, lat2);
    azg = GeogAz(lon1, lat1, lon2, lat2);
    while (scanf(" %lf %lf %lf", &lon, &lat, &z) == 3) {
	lat *= RAD_DEG;
	lon *= RAD_DEG;
	d = a0 * GeogDist(lon1, lat1, lon, lat);
	az = GeogAz(lon1, lat1, lon, lat) - azg;
	x = d * cos(az);
	printf("%.1lf %.1lf\n", x, z);
    }
    return 1;
}
