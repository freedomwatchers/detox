/*
 * Copyright (c) 2004-2005, Doug Harple.  All rights reserved.
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
 * $Id: detox.c,v 1.22 2005/03/05 01:54:56 purgedhalo Exp $
 *
 */

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "detox.h"
#include "file.h"
#include "config.h"
#include "clean_string.h"
#include "detox_path.h"

#include "config_file.h"
#include "parse_table.h"

#ifndef HAVE_LIBPOPT
#include "parse_options_getopt.h"
#else
#include "parse_options_popt.h"
#endif

int main(int argc, char **argv)
{
	struct stat stat_info;
	int err;

	struct detox_sequence_list *sequences = NULL;
	struct detox_sequence_list *list_work = NULL;
	struct detox_sequence *which_sequence = NULL;
	struct detox_sequence *work = NULL;

	char *check_config_file = NULL;

	struct detox_options *main_options;

	char **file_walk;

#ifndef HAVE_LIBPOPT
	main_options = parse_options_getopt(argc, argv);
#else
	main_options = parse_options_popt(argc, argv);
#endif

	if (main_options == NULL) {
		return -1;
	}

	if (main_options->check_config_file) {
		check_config_file = strdup(main_options->check_config_file);
	}

	if (check_config_file != NULL) {
		sequences = parse_config_file(check_config_file, NULL, main_options);
		if (sequences == NULL) {
			fprintf(stderr, "detox: unable to open: %s\n", check_config_file);
			exit(-1);
		}
	}
	else {
		check_config_file = malloc(256);

		snprintf(check_config_file, 256, "%s/detoxrc", default_etc_dir);
		sequences = parse_config_file(check_config_file, NULL, main_options);

		if (sequences == NULL) {
			sequences = parse_config_file("/etc/detoxrc", NULL, main_options);
		}

		if (sequences == NULL) {
			sequences = parse_config_file("/usr/local/etc/detoxrc", NULL, main_options);
		}

		snprintf(check_config_file, 256, "%s/.detoxrc", getenv("HOME"));
		sequences = parse_config_file(check_config_file, sequences, main_options);

		if (sequences == NULL) {

			/*
			 * Head
			 */

			sequences = malloc(sizeof(struct detox_sequence_list));
			memset(sequences, 0, sizeof(struct detox_sequence_list));

			sequences->name = strdup("default");
			sequences->source_filename = strdup("inside the beast");

			/*
			 * Step 1 - ISO8859_1
			 */

			sequences->head = malloc(sizeof(struct detox_sequence));
			work = sequences->head;
			memset(work, 0, sizeof(struct detox_sequence));

			work->cleaner = &clean_iso8859_1;

			/*
			 * Step 2 - Safe
			 */

			work->next = malloc(sizeof(struct detox_sequence));
			work = work->next;
			memset(work, 0, sizeof(struct detox_sequence));

			work->cleaner = &clean_safe;

			/*
			 * Step 3 - Wipe Up
			 */

			work->next = malloc(sizeof(struct detox_sequence));
			work = work->next;
			memset(work, 0, sizeof(struct detox_sequence));

			work->cleaner = &clean_wipeup;

			/*
			 * Deprecated
			 */
			if (main_options->remove_trailing) {
				static struct clean_string_options *csopts;

				csopts = malloc(sizeof(struct clean_string_options));
				memset(csopts, 0, sizeof(struct clean_string_options));
				csopts->remove_trailing = 1;

				work->options = csopts;
			}
		}

		free(check_config_file);
	}


	/*
	 * Determine which sequence to use
	 */

	which_sequence = NULL;

	list_work = sequences;

	while (list_work != NULL) {
		if (strcmp(list_work->name, (main_options->sequence_name == NULL) ? "default" : main_options->sequence_name) == 0) {
			which_sequence = list_work->head;
			break;
		}

		list_work = list_work->next;
	}

	/*
	 * If no sequence was found, and the user didn't specify a sequence
	 * to use, just use the first sequence.
	 */

	if (which_sequence == NULL && main_options->sequence_name == NULL) {
		if (sequences != NULL) {
			which_sequence = sequences->head;
		}
	}

	main_options->sequence = which_sequence;

	/*
	 * List sequences
	 */
	if (main_options->list_sequences) {
		if (!main_options->verbose) {
			printf("available sequences:\n");
		}

		list_work = sequences;

		while (list_work != NULL) {
			if (main_options->verbose) {
				printf("sequence name: ");
			}
			else {
				printf("\t");
			}
			printf("%s%s\n", list_work->name, (main_options->sequence == list_work->head) ? " (*)" : "");
			if (main_options->verbose) {
				printf("\tsource file: %s\n", list_work->source_filename);

				work = list_work->head;
				while (work != NULL) {
					if (work->cleaner == &clean_uncgi) {
						printf("\tcleaner: uncgi\n");
					}
					else if (work->cleaner == &clean_safe) {
						printf("\tcleaner: safe\n");
					}
					else if (work->cleaner == &clean_wipeup) {
						printf("\tcleaner: wipeup\n");
						if (work->options != NULL) {
							struct clean_string_options *opts = work->options;
							printf("\t\tremove trailing: %s\n", opts->remove_trailing ? "yes" : "no");
						}
					}
					else if (work->cleaner == &clean_iso8859_1) {
						printf("\tcleaner: iso8859_1\n");
						if (work->options != NULL) {
							struct clean_string_options *opts = work->options;
							if (opts->filename != NULL) {
								printf("\t\ttranslation table: %s\n", opts->filename);
							}
						}
					}
					else if (work->cleaner == &clean_utf_8) {
						printf("\tcleaner: utf_8\n");
						if (work->options != NULL) {
							struct clean_string_options *opts = work->options;
							if (opts->filename != NULL) {
								printf("\t\ttranslation table: %s\n", opts->filename);
							}
						}
					}
					else if (work->cleaner == &clean_max_length) {
						printf("\tcleaner: max length\n");
						if (work->options != NULL) {
							struct clean_string_options *opts = work->options;
							printf("\t\tlength: %d\n", opts->max_length);
						}
					}
					if (work->cleaner == &clean_lower) {
						printf("\tcleaner: lower\n");
					}

					work = work->next;
				}
			}

			list_work = list_work->next;
		}

		exit(0);
	}

	/*
	 * Fail if no sequence is available
	 */
	if (main_options->sequence == NULL) {

		/*
		 * XXX - Explain this better
		 */
		fprintf(stderr, "detox: no sequence to work with\n");
		exit(-1);
	}

	/*
	 * Check translation tables
	 */

	work = main_options->sequence;
	while (work != NULL) {
		char *check_filename = NULL;
		int do_search = 0;

		struct translation_table *table = NULL;
		struct clean_string_options *opts;

		if (work->cleaner == &clean_iso8859_1) {
			if (work->options != NULL) {
				opts = work->options;
				if (opts->filename != NULL) {
					check_filename = opts->filename;
				}
			}

			if (!check_filename) {
				check_filename = "iso8859_1.tbl";
				do_search = 1;
			}
		}
		else if (work->cleaner == &clean_utf_8) {
			if (work->options != NULL) {
				opts = work->options;
				if (opts->filename != NULL) {
					check_filename = opts->filename;
				}
			}

			if (!check_filename) {
				check_filename = "unicode.tbl";
				do_search = 1;
			}
		}


		if (check_filename || do_search) {

			table = NULL;

			if (do_search) {
				check_config_file = malloc(256);

				snprintf(check_config_file, 256, "%s/%s", default_data_dir, check_filename);
				table = parse_table(check_config_file);

				if (table == NULL) {
					snprintf(check_config_file, 256, "/usr/share/detox/%s", check_filename);
					table = parse_table(check_config_file);
				}

				if (table == NULL) {
					snprintf(check_config_file, 256, "/usr/local/share/detox/%s", check_filename);
					table = parse_table(check_config_file);
				}

				if (table == NULL) {

					/*
					 * Fall back to the non-file based
					 * cleaner
					 */
					if (work->cleaner == &clean_iso8859_1) {
						work->cleaner = &clean_iso8859_1_basic;
					}
					else if (work->cleaner == &clean_utf_8) {
						work->cleaner = &clean_utf_8_basic;
					}
					else {
						fprintf(stderr, "detox: unable to locate translation table or fall back\n");
						exit(-1);
					}
				}
				else {

					/*
					 * Allocate an options
					 */
					opts = malloc(sizeof(struct clean_string_options));
					memset(opts, 0, sizeof(struct clean_string_options));

					opts->translation_table = table;
					work->options = opts;
				}

				free(check_config_file);
			}
			else {
				table = parse_table(check_filename);
				if (table == NULL) {
					fprintf(stderr, "detox: unable to parse file: %s\n", check_filename);
					exit(-1);
				}

				opts = work->options;
				opts->translation_table = table;
			}
		}


		work = work->next;
	}


	/*
	 * Do some actual work
	 */
	file_walk = main_options->files;
	while (*file_walk) {
		if (main_options->verbose) {
			printf("Scanning: %s\n", *file_walk);
		}

		err = lstat(*file_walk, &stat_info);
		if (err == -1) {
			fprintf(stderr, "%s: %s\n", *file_walk, strerror(errno));
		}
		else {
			if (S_ISDIR(stat_info.st_mode)) {
				parse_dir(*file_walk, main_options);
			}
			else if (S_ISREG(stat_info.st_mode)) {
				parse_file(*file_walk, main_options);
			}
			else if (main_options->special) {
				parse_special(*file_walk, main_options);
			}
		}

		file_walk++;
	}

	return 0;
}
