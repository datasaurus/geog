/*
   -	geog_app.c --
   -		This file defines an application that does geography calculations.
   -
   .	Copyright (c) 2008 Gordon D. Carrie
   .	All rights reserved.
   .
   .	Please send feedback to dev0@trekix.net
   .
   .	$Revision: 1.35 $ $Date: 2010/10/08 19:37:28 $
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "err_msg.h"
#include "geog_lib.h"

/* Application name and subcommand name */
char *cmd, *cmd1;

/* Number of subcommands */
#define NCMD 7

/* Callback functions.  There should be one for each subcommand. */
typedef int (callback)(int , char **);
callback rearth_cb;
callback lonr_cb;
callback latn_cb;
callback dist_cb;
callback az_cb;
callback step_cb;
callback beam_ht_cb;

/* If true, use degrees instead of radians */
int use_deg = 0;

int main(int argc, char *argv[])
{
    char *ang_u;	/* Angle unit */
    int i;		/* Index for subcommand in argv[1] */
    int rslt;		/* Return code */

    /* Arrays of subcommand names and associated callbacks */
    char *cmd1v[NCMD] = {"rearth", "lonr", "latn", "dist", "az",
	"step", "beam_ht"};
    callback *cb1v[NCMD] = {rearth_cb, lonr_cb, latn_cb, dist_cb, az_cb,
	step_cb, beam_ht_cb};

    cmd = argv[0];
    if (argc < 2) {
	fprintf(stderr, "Usage: %s subcommand [subcommand_options ...]\n", cmd);
	exit(1);
    }
    cmd1 = argv[1];

    /* Check for angle unit */
    if ((ang_u = getenv("ANGLE_UNIT")) != NULL) {
	if (strcmp(ang_u, "DEGREE") == 0) {
	    use_deg = 1;
	} else if (strcmp(ang_u, "RADIAN") == 0) {
	    use_deg = 0;
	} else {
	    fprintf(stderr, "%s: Unknown angle unit %s.\n", cmd, ang_u);
	    exit(1);
	}
    }

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
    double c;

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
    c = (use_deg ? RAD_DEG : 1.0);
    printf("%lf\n", GeogLonR(l * c, r * c) / c);
    return 1;
}

int latn_cb(int argc, char *argv[])
{
    char *l_s;			/* String from command line */
    double l;			/* Latitude value from command line */
    double c;

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
    c = (use_deg ? RAD_DEG : 1.0);
    printf("%f\n", GeogLatN(l * c) / c);
    return 1;
}

int dist_cb(int argc, char *argv[])
{
    char *lon1_s, *lat1_s, *lon2_s,  *lat2_s;
    double lon1, lat1, lon2, lat2, c;

    if (argc != 6) {
	Err_Append("Usage: ");
	Err_Append(cmd);
	Err_Append(" ");
	Err_Append(cmd1);
	Err_Append(" lat1 lon1 lat2 lon2\n");
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
    c = (use_deg ? RAD_DEG : 1.0);
    printf("%f\n", GeogDist(lon1 * c, lat1 * c, lon2 * c, lat2 * c) / c);
    return 1;
}

int az_cb(int argc, char *argv[])
{
    char *lon1_s, *lat1_s, *lon2_s,  *lat2_s;
    double lon1, lat1, lon2, lat2, c;

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
    c = (use_deg ? RAD_DEG : 1.0);
    printf("%f\n", GeogAz(lon1 * c, lat1 * c, lon2 * c,  lat2 * c) / c);
    return 1;
}

int step_cb(int argc, char *argv[])
{
    double lon1, lat1, dirn, dist, lon2, lat2, c;

    if (argc == 2) {
	while (scanf("%lf %lf %lf %lf", &lon1, &lat1, &dirn, &dist) == 4) {
	    c = use_deg ? RAD_DEG : 1.0;
	    GeogStep(lon1 * c, lat1 * c, dirn * c, dist * c, &lon2, &lat2);
	    c = use_deg ? DEG_RAD : 1.0;
	    printf("%f %f\n", lon2 * c, lat2 * c);
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
	c = use_deg ? RAD_DEG : 1.0;
	GeogStep(lon1 * c, lat1 * c, dirn * c, dist * c, &lon2, &lat2);
	c = use_deg ? DEG_RAD : 1.0;
	printf("%f %f\n", lon2 * c, lat2 * c);
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
    double d, tilt, a0, c;

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

    c = use_deg ? RAD_DEG : 1.0;
    printf("%lf\n", GeogBeamHt(d, tilt * c, a0));
    return 1;
}
