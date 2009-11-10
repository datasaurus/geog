/*
   -	geog_app.c --
   -		This file defines an application that does geography calculations.
   -
   .	Copyright (c) 2008 Gordon D. Carrie
   .	All rights reserved.
   .
   .	Please send feedback to dev0@trekix.net
   .
   .	$Revision: 1.21 $ $Date: 2009/10/07 17:06:47 $
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "err_msg.h"
#include "mstring.h"
#include "geog_lib.h"

/* Application name and subcommand name */
char *cmd, *cmd1;

/* Number of subcommands */
#define NCMD 3

/* Callback functions.  There should be one for each subcommand. */
typedef int (callback)(int , char **);
callback lonr_cb;
callback latn_cb;
callback gcdist_cb;

/* If true, use degrees instead of radians */
int use_deg = 0;

int main(int argc, char *argv[])
{
    char *ang_u;	/* Angle unit */
    int i;		/* Index for subcommand in argv[1] */
    int rslt;		/* Return code */

    /* Arrays of subcommand names and associated callbacks */
    char *cmd1v[NCMD] = {"lonr", "latn", "gcdist"};
    callback *cb1v[NCMD] = {lonr_cb, latn_cb, gcdist_cb};

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

int lonr_cb(int argc, char *argv[])
{
    char *l_s, *r_s;		/* Strings from command line */
    double l, r;		/* Values from command line */
    char *fmt;			/* Format for printing result */

    if (argc < 4) {
	Err_Append("Usage: ");
	Err_Append(cmd);
	Err_Append(" ");
	Err_Append(cmd1);
	Err_Append("[-f format] lon reflon\n");
	return 0;
    }
    fmt = "%lf\n";
    if (strcmp(argv[2], "-f") == 0) {
	fmt = Str_Esc(argv[3]);
	l_s = argv[4];
	r_s = argv[5];
    } else {
	l_s = argv[2];
	r_s = argv[3];
    }
    if (sscanf(l_s, "%lf", &l) != 1) {
	Err_Append("Expected float value for longitude, got ");
	Err_Append(l_s);
	return 0;
    }
    if (sscanf(r_s, "%lf", &r) != 1) {
	Err_Append("Expected float value for reference longitude, got ");
	Err_Append(r_s);
	return 0;
    }
    if (use_deg) {
	l *= PI / 180.0;
	r *= PI / 180.0;
    }
    printf(fmt, (use_deg ? 180 / PI : 1.0) * LonToRef(l, r));
    return 1;
}

int latn_cb(int argc, char *argv[])
{
    char *l_s;			/* String from command line */
    double l;			/* Latitude value from command line */
    char *fmt;

    if (argc < 3) {
	Err_Append("Usage: ");
	Err_Append(cmd);
	Err_Append(" ");
	Err_Append(cmd1);
	Err_Append("[-f format] lat\n");
	return 0;
    }
    fmt = "%lf\n";
    if (strcmp(argv[2], "-f") == 0) {
	fmt = Str_Esc(argv[3]);
	l_s = argv[4];
    } else {
	l_s = argv[2];
    }

    /* Get latitude value from command line argument */
    if (sscanf(l_s, "%lf", &l) != 1) {
	Err_Append("Expected float value for latitude, got ");
	Err_Append(l_s);
	return 0;
    }
    if (use_deg) {
	l *= PI / 180.0;
    }

    /* Send result */
    printf(fmt, (use_deg ? 180 / PI : 1.0) * LatN(l));
    return 1;
}

int gcdist_cb(int argc, char *argv[])
{
    char *lat1_s, *lon1_s, *lat2_s, *lon2_s;
    double lat1, lon1, lat2, lon2;
    char *fmt;

    if (argc < 6) {
	Err_Append("Usage: ");
	Err_Append(cmd);
	Err_Append(" ");
	Err_Append(cmd1);
	Err_Append("[-f format] lat1 lon1 lat2 lon2\n");
	return 0;
    }
    fmt = "%lf\n";
    if (strcmp(argv[2], "-f") == 0) {
	fmt = Str_Esc(argv[3]);
	lat1_s = argv[4];
	lon1_s = argv[5];
	lat2_s = argv[6];
	lon2_s = argv[7];
    } else {
	lat1_s = argv[2];
	lon1_s = argv[3];
	lat2_s = argv[4];
	lon2_s = argv[5];
    }

    /* Get coordinates from command line arguments */
    if (sscanf(lat1_s, "%lf", &lat1) != 1) {
	Err_Append("Expected float value for lat1, got ");
	Err_Append(lat1_s);
	return 0;
    }
    if (sscanf(lon1_s, "%lf", &lon1) != 1) {
	Err_Append("Expected float value for lon1, got ");
	Err_Append(lon1_s);
	return 0;
    }
    if (sscanf(lat2_s, "%lf", &lat2) != 1) {
	Err_Append("Expected float value for lat2, got ");
	Err_Append(lat2_s);
	return 0;
    }
    if (sscanf(lon2_s, "%lf", &lon2) != 1) {
	Err_Append("Expected float value for lon2, got ");
	Err_Append(lon2_s);
	return 0;
    }
    if (use_deg) {
	lat1 *= PI / 180.0;
	lon1 *= PI / 180.0;
	lat2 *= PI / 180.0;
	lon2 *= PI / 180.0;
    }

    /* Send result */
    printf(fmt, (use_deg ? 180 / PI : 1.0) * GCDistR(lat1, lon1, lat2, lon2));
    return 1;
}
