/*
 - geog_app.c --
 - 	This file defines an application that does geography calculations.
 -
   Copyright (c) 2008 Gordon D. Carrie
   Licensed under the Open Software License version 3.0
  
   Please send feedback to dev0@trekix.net

   $Revision: 1.11 $ $Date: 2009/07/07 20:24:19 $
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "err_msg.h"
#include "mstring.h"
#include "geog_lib.h"

/* Application name and subcommand name */
char *cmd, *cmd1;

/* Callback functions.  There should be one for each subcommand. */
typedef int (callback)(int , char **);
callback lonr_cb;
callback plat_cb;

/* Number of subcommands */
#define NCMD 2

/* Array of subcommand names */
char *cmd1v[NCMD] = {"lonr", "plat"};

/* Array of subcomand callbacks. cb1v[i] is the callback for cmd1v[i] */
callback *cb1v[NCMD] = {lonr_cb, plat_cb};

/* Output format */
char *fmt;

int main(int argc, char *argv[])
{
    int i, j;
    int rslt;

    /* Ensure minimum command line */
    cmd = argv[0];
    if (argc < 2) {
	fprintf(stderr,
		"Usage: %s [%s_options ...] subcommand [subcommand_options ...]\n",
		cmd, cmd);
	exit(1);
    }

    /* Parse application options */
    fmt = "%lf\n";
    for (i = 1; argv[i]; i++) {
	if (strcmp(argv[i], "-f") == 0) {
	    fmt = stresc(argv[++i]);
	} else {
	    cmd1 = argv[i];
	    break;
	}
    }

    /* Search cmd1v for cmd1.  When match is found, evaluate the associated
     * callback from cb1v. */
    for (j = 0; j < NCMD; j++) {
	if (strcmp(cmd1v[j], cmd1) == 0) {
	    rslt = (cb1v[j])(argc - i - 1, argv + i + 1);
	    if ( !rslt ) {
		fprintf(stderr, "%s %s failed.\n", cmd, cmd1);
		fprintf(stderr, "%s\n", err_get());
		break;
	    } else {
		break;
	    }
	}
    }
    if (j == NCMD) {
	fprintf(stderr, "%s: No option or subcommand named %s\n", cmd, cmd1);
	rslt = 0;
    }
    return !rslt;
}

int lonr_cb(int argc, char *argv[])
{
    char *cmd1 = argv[0];	/* Name of this subcommand */
    char *l_s, *r_s;		/* Strings from command line */
    double l, r;		/* Values from command line */

    /* Ensure minimum command line */
    if (argc != 2) {
	err_append("Usage: ");
	err_append(cmd);
	err_append(" ");
	err_append(cmd1);
	err_append(" lon reflon\n");
	return 0;
    }
    l_s = argv[0];
    r_s = argv[1];

    /* Get values from command line arguments */
    if (sscanf(l_s, "%lf", &l) != 1) {
	err_append("Expected float value for longitude, got ");
	err_append(l_s);
	return 0;
    }
    if (sscanf(r_s, "%lf", &r) != 1) {
	err_append("Expected float value for reference longitude, got ");
	err_append(r_s);
	return 0;
    }

    /* Send result */
    printf(fmt, lonr(l, r));
    return 1;
}

int plat_cb(int argc, char *argv[])
{
    char *cmd1 = argv[0];	/* Name of this subcommand */
    char *l_s;			/* String from command line */
    double l;			/* Latitude value from command line */

    /* Ensure minimum command line */
    if (argc != 1) {
	err_append("Usage: ");
	err_append(cmd);
	err_append(cmd1);
	err_append(" lat\n");
	return 0;
    }
    l_s = argv[0];

    /* Get latitude value from command line argument */
    if (sscanf(l_s, "%lf", &l) != 1) {
	err_append("Expected float value for latitude, got ");
	err_append(l_s);
	return 0;
    }

    /* Send result */
    printf(fmt, plat(l));
    return 1;
}
