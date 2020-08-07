%{
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
 * $Id: config_file.y,v 1.15 2005/03/06 06:42:20 purgedhalo Exp $
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "detox.h"
#include "clean_string.h"
#include "config_file.h"

/*
 * I must apologize in advance for the cryptic, global variable names.
 */

static struct detox_sequence_list *cf_sl_ret, *cf_sl_current;
static struct detox_sequence *cf_seq_ret, *cf_seq_current;
static struct clean_string_options *csopts;
static char *current_name = NULL;
static char *current_filename = NULL;
static struct detox_options *current_options;

void cf_append_to_sequence_list(void);
void cf_append_to_sequence(void *ptr, void *opts);

void yyerror (char *s);

%}

%union {
    char	*string;	/* string buffer */
    int		cmd;		/* command value */
    struct detox_sequence *seq;	/* sequence */
    int		nvalue;		/* nvalue */
}

%token <string> QSTRING ID
%token <cmd> SEQUENCE UNCGI ISO8859_1 UTF_8 SAFE WIPEUP MAX_LENGTH LOWER
%token <cmd> FILENAME REMOVE_TRAILING LENGTH
%token <cmd> OPEN CLOSE EOL
%token <nvalue> NVALUE 

%type <string> string
 
%%

sequence: sequence_open methodlist sequence_close 
	|
	sequence_open methodlist sequence_close sequence
	;

methodlist: method |
	methodlist method
	;

sequence_open: SEQUENCE string OPEN { current_name = $2; }
	;

sequence_close: CLOSE EOL { cf_append_to_sequence_list(); }
	;

method: UNCGI EOL	{ cf_append_to_sequence(&clean_uncgi, NULL); }
	| 
	LOWER EOL	{ cf_append_to_sequence(&clean_lower, NULL); }
	| 
	wipeup EOL
	| 
	iso8859_1 EOL
	|
	utf_8 EOL
	|
	safe EOL
	|
	max_length EOL
	;

iso8859_1: ISO8859_1 { cf_append_to_sequence(&clean_iso8859_1, NULL); }
	|
	ISO8859_1 OPEN CLOSE { cf_append_to_sequence(&clean_iso8859_1, NULL); }
	|
	ISO8859_1 OPEN FILENAME string EOL CLOSE { 
		csopts = malloc(sizeof(struct clean_string_options));
		memset(csopts, 0, sizeof(struct clean_string_options));
		csopts->filename = $4;

		cf_append_to_sequence(&clean_iso8859_1, csopts);
	}
	;

utf_8: UTF_8 { cf_append_to_sequence(&clean_utf_8, NULL); }
	|
	UTF_8 OPEN CLOSE { cf_append_to_sequence(&clean_utf_8, NULL); }
	|
	UTF_8 OPEN FILENAME string EOL CLOSE { 
		csopts = malloc(sizeof(struct clean_string_options));
		memset(csopts, 0, sizeof(struct clean_string_options));
		csopts->filename = $4;

		cf_append_to_sequence(&clean_utf_8, csopts);
	}
	;

safe: SAFE { cf_append_to_sequence(&clean_safe, NULL); }
	|
	SAFE OPEN CLOSE { cf_append_to_sequence(&clean_safe, NULL); }
	|
	SAFE OPEN FILENAME string EOL CLOSE { 
		csopts = malloc(sizeof(struct clean_string_options));
		memset(csopts, 0, sizeof(struct clean_string_options));
		csopts->filename = $4;

		cf_append_to_sequence(&clean_safe, csopts);
	}
	;

wipeup:	WIPEUP { 
		if (current_options->remove_trailing) {
			csopts = malloc(sizeof(struct clean_string_options));
			memset(csopts, 0, sizeof(struct clean_string_options));
			csopts->remove_trailing = 1;
		}
		else {
			csopts = NULL;
		}

		cf_append_to_sequence(&clean_wipeup, csopts);
	}
	|
	WIPEUP OPEN CLOSE { 
		if (current_options->remove_trailing) {
			csopts = malloc(sizeof(struct clean_string_options));
			memset(csopts, 0, sizeof(struct clean_string_options));
			csopts->remove_trailing = 1;
		}
		else {
			csopts = NULL;
		}

		cf_append_to_sequence(&clean_wipeup, csopts);
	}
	|
	WIPEUP OPEN REMOVE_TRAILING EOL CLOSE {
		csopts = malloc(sizeof(struct clean_string_options));
		memset(csopts, 0, sizeof(struct clean_string_options));
		csopts->remove_trailing = 1;

		cf_append_to_sequence(&clean_wipeup, csopts);
	}
	;

max_length: MAX_LENGTH	{ cf_append_to_sequence(&clean_max_length, NULL); }
	|
	MAX_LENGTH OPEN CLOSE { cf_append_to_sequence(&clean_max_length, NULL); }
	|
	MAX_LENGTH OPEN LENGTH NVALUE EOL CLOSE {
		csopts = malloc(sizeof(struct clean_string_options));
		memset(csopts, 0, sizeof(struct clean_string_options));
		csopts->max_length = $4;

		cf_append_to_sequence(&clean_max_length, csopts);
	}
	;


string: QSTRING 		{ $$ = $1; }
	|
	ID			{ $$ = $1; }
	;

%%

extern FILE *yyin;
extern FILE *yyout;

struct detox_sequence_list *parse_config_file(char *filename, struct detox_sequence_list *previous_list, struct detox_options *main_options) {

	current_filename = filename;
	current_options = main_options;

	/* 
	 * XXX - Should we be closing the default yyin/yyout?  If so, should we
	 * be setting them to NULL at the end of this function?
	 */

	yyin = fopen(filename, "r");
	if (yyin == NULL) {
		return previous_list;
	}
	yyout = fopen("/dev/null", "w");

	cf_sl_ret = NULL;
	cf_sl_current = NULL;

	if (previous_list) {
		cf_sl_ret = previous_list;
		cf_sl_current = cf_sl_ret;
		while (cf_sl_current->next != NULL) {
			cf_sl_current = cf_sl_current->next;
		}
	}

	cf_seq_ret = NULL;
	cf_seq_current = NULL;

	do {
		yyparse();
	}
	while (!feof(yyin));

	fclose(yyin);
	fclose(yyout);

	return cf_sl_ret;
}

void yyerror (char *s) {
	/*
	 * XXX - Is extern valid here?  Does it do what I'm expecting?
	 */
	extern char *yytext;

	fprintf(stderr, "detox: error parsing config file %s: %s\n", current_filename, s);
	fprintf(stderr, "\tline %d", config_file_lineno);
	if (yytext != NULL) {
		fprintf(stderr, ": %s", yytext);
	}
	fprintf(stderr, "\n");
	exit(-1);
}


void cf_append_to_sequence_list(void) {
	struct detox_sequence_list *work;

	if (current_name == NULL) {
		current_name = strdup("default");
	}

	work = NULL;

	if (cf_sl_ret != NULL) {
		work = cf_sl_ret;

		while (work != NULL) {
			if (strcmp(work->name, current_name) == 0) {
				break;
			}

			work = work->next;
		}

	}

	if (work != NULL) {
		/*
		 * XXX - Free Old Tree
		 */
	}
	else {
		work = malloc(sizeof(struct detox_sequence_list));
		memset(work, 0, sizeof(struct detox_sequence_list));

		work->name = strdup(current_name);

		/*
		 * Append to the tree first.  If we don't, we could create a
		 * circular reference.
		 */
		if (cf_sl_ret == NULL) {
			cf_sl_ret = cf_sl_current = work;
		}
		else {
			cf_sl_current->next = work;
			cf_sl_current = work;
		}

	}

	work->head = cf_seq_ret;
	work->source_filename = strdup(current_filename);
	cf_seq_ret = cf_seq_current = NULL;

}


void cf_append_to_sequence(void *ptr, void *opts) {
	struct detox_sequence *work;

	work = malloc(sizeof(struct detox_sequence));
	memset(work, 0, sizeof(struct detox_sequence));

	work->cleaner = ptr;
	work->options = opts;

	if (cf_seq_ret == NULL) {
		cf_seq_ret = cf_seq_current = work;
	}
	else {
		cf_seq_current->next = work;
		cf_seq_current = work;
	}
}

