/*
 - geog_app.c --
 - 	This file defines an application that does geography calculations.
 -
   Copyright (c) 2008 Gordon D. Carrie
   Licensed under the Open Software License version 3.0
  
   Please send feedback to dev0@trekix.net

   $Revision: 1.6 $ $Date: 2008/12/17 22:53:56 $
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "alloc.h"
#include "err_msg.h"
#include "geog.h"

/* Application name and subcommand name */
char *cmd;
char *cmd1;

/* Callback functions.  There should be one for each subcommand. */
typedef int (callback)(int , char **);
callback rlon_cb;
callback plat_cb;

/* Number of subcommands */
#define NCMD 2

/* Array of subcommand names */
char *cmd1v[NCMD] = {"rlon", "plat"};

/* Array of subcomand callbacks. cb1v[i] is the callback for cmd1v[i] */
callback *cb1v[NCMD] = {rlon_cb, plat_cb};

int main(int argc, char *argv[])
{
    int i;
    int rslt;

    /* Ensure minimum command line */
    cmd = argv[0];
    if (argc < 2) {
	fprintf(stderr, "Usage: %s subcommand [options ...]\n", cmd);
	exit(1);
    }
    cmd1 = argv[1];

    /* Search cmd1v for cmd1.  When match is found, evaluate the associated
     * callback from cb1v. */
    for (i = 0; i < NCMD; i++) {
	if (strcmp(cmd1v[i], cmd1) == 0) {
	    rslt = (cb1v[i])(argc - 2, argv + 2);
	    if ( !rslt ) {
		fprintf(stderr, "%s %s failed.\n", cmd, cmd1);
		fprintf(stderr, "%s\n", err_get());
		exit(1);
	    } else {
		exit(0);
	    }
	}
    }
    fprintf(stderr, "%s: No subcommand named %s\n", cmd, cmd1);
    return 1;
}

int rlon_cb(int argc, char *argv[])
{
    char *lon_s, *rlon_s;	/* Strings from command line */
    double l, r;		/* Values from command line */

    /* Ensure minimum command line */
    if (argc != 2) {
	err_append("Usage: ");
	err_append(cmd);
	err_append(cmd1);
	err_append(" lon reflon\n");
	return 0;
    }
    lon_s = argv[0];
    rlon_s = argv[1];

    /* Get values from command line arguments */
    if (sscanf(lon_s, "%lf", &l) != 1) {
	err_append("Expected float value for longitude, got ");
	err_append(lon_s);
	return 0;
    }
    if (sscanf(rlon_s, "%lf", &r) != 1) {
	err_append("Expected float value for reference longitude, got ");
	err_append(rlon_s);
	return 0;
    }

    /* Send result */
    printf("%lf\n", rlon(l, r));
    return 1;
}

int plat_cb(int argc, char *argv[])
{
    char *lat_s;		/* String from command line */
    double l;			/* Latitude value from command line */

    /* Ensure minimum command line */
    if (argc != 1) {
	err_append("Usage: ");
	err_append(cmd);
	err_append(cmd1);
	err_append(" lat\n");
	return 0;
    }
    lat_s = argv[0];

    /* Get latitude value from command line argument */
    if (sscanf(lat_s, "%lf", &l) != 1) {
	err_append("Expected float value for latitude, got ");
	err_append(lat_s);
	return 0;
    }

    /* Send result */
    printf("%lf\n", plat(l));
    return 1;
}
