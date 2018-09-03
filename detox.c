/*
 * Copyright (c) 2004, Doug Harple.  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of author nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * $Id: detox.c,v 1.6 2004/02/16 03:17:15 purgedhalo Exp $
 * 
 */

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>

#include "detox.h"
#include "file.h"
#include "config.h"

enum {
	LONG_OPTION_DRY_RUN = 1,
	LONG_OPTION_REMOVE_TRAILING,
	LONG_OPTION_SPECIAL
};

/* expect this to be overwritten! */
int long_option = 0;

static struct option longopts[] = {
	/* long options with equivalents */
	{"help", no_argument, 0, 'h'},

	/* long options without */
	{"dry-run", no_argument, &long_option, LONG_OPTION_DRY_RUN},
	{"remove-trailing", no_argument, &long_option, LONG_OPTION_REMOVE_TRAILING},
	{"special", no_argument, &long_option, LONG_OPTION_SPECIAL},

	/* done */
	{0, 0, 0, 0}
};

char usage_message[] = "usage: detox [-hrv] [--dry-run] [--remove-trailing] [--special] [file ...]\n";

char help_message[] = {
	"	-h --help	this message\n"
	"	-r 		be recursive (descend into subdirectories)\n"
	"	-v 		be verbose\n"
	"	--dry-run	do a dry run (don't actually do anything)\n"
	"	--remove-trailing\n"
	"			remove trailing _ and - before a period\n"
	"	--special	work on links and special files\n"
};

struct detox_options main_options;

int main(int argc, char **argv)
{
	struct stat stat_info;
	int optcode, err;

	memset(&main_options, 0, sizeof(struct detox_options));

	while ((optcode = getopt_long(argc, argv, "hrvV?", longopts, NULL)) != -1) {
		switch (optcode) {
			case 'v':
				main_options.verbose++;
				break;

			case 'r':
				main_options.recurse = 1;
				break;

			case 'V':
				printf("%s\n", PACKAGE_STRING);
				return 0;

			case 'h':
				printf(usage_message);
				printf(help_message);
				return 0;

			case '?':
				printf(usage_message);
				return 0;

			case 0:
				switch (long_option) {
					case LONG_OPTION_DRY_RUN:
						main_options.dry_run = 1;
						break;

					case LONG_OPTION_REMOVE_TRAILING:
						main_options.remove_trailing = 1;
						break;

					case LONG_OPTION_SPECIAL:
						main_options.special = 1;
						break;

					default:
						/*
						 * getopt_long shouldn't let us get here...
						 * verify?
						 */
						printf("unknown option: %s\n", optarg);
						break;
				}
				long_option = 0;	/* clean up! */
				break;

			default:
				printf("unknown option: %c\n", optcode);
				return -1;
		}
	}

	if (optind < argc) {
		while (optind < argc) {
			if (main_options.verbose) {
				printf("Scanning: %s\n", argv[optind]);
			}

			err = lstat(argv[optind], &stat_info);
			if (err == -1) {
				fprintf(stderr, "%s: %s\n", argv[optind], strerror(errno));
			}
			else {
				if (S_ISDIR(stat_info.st_mode)) {
					parse_dir(argv[optind], &main_options);
				} 
				else if (S_ISREG(stat_info.st_mode)) {
					parse_file(argv[optind], &main_options);
				} 
				else if (main_options.special) {
					parse_special(argv[optind], &main_options);
				}
			}

			optind++;
		}
	} 
	else {
		printf(usage_message);
		return -1;
	}

	return 0;
}
