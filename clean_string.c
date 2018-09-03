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
 * $Id: clean_string.c,v 1.6 2004/02/16 03:17:15 purgedhalo Exp $
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "clean_string.h"

/* translation array for ISO8859.1 characters */
#include "iso8859_1.h"


/*
 * Translates ISO8859.1 characters (Latin-1) into lower ASCII characters.
 */
unsigned char *clean_iso8859_1(unsigned char *s)
{
	unsigned char *output, *input_walk, *output_walk, *replace_walk;
	int replace_pos;

	if (s == NULL) {
		return NULL;
	}

	output = malloc((strlen(s) * ISO8859_1_MAXLEN) + 1);
	if (output == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		return NULL;
	}

	input_walk = s;
	output_walk = output;

	while (*input_walk != '\0') {
		if (*input_walk >= ISO8859_1_OFFSET) {
			replace_pos = *input_walk - ISO8859_1_OFFSET;
			replace_walk = (unsigned char *)&iso8859_1_trans[replace_pos];

			while (*replace_walk != '\0') {
				*output_walk++ = *replace_walk++;
			}
			input_walk++;
		} 
		else {
			*output_walk++ = *input_walk++;
		}
	}

	*output_walk = 0;

	return output;
}

/*
 * Cleans up any unsafe characters.
 * 
 * The rules are: 
 *   Strip if at beginning of string, then leave alone: 
 *     -
 * 
 *   Replace with _ if at beginning of string, then leave alone: 
 *     #
 * 
 *   Leave alone: 
 *     ~ % ^ _ , . + =
 * 
 *   Translate: 
 *     &  into  _and_
 * 
 *   Replace with _: 
 *     ` ! @ $ * \ | : ; " ' < ? /
 * 
 *   Replace with - (or _, if at beginning of string): 
 *     ( ) [ ] { }
 * 
 */
unsigned char *clean_safe(unsigned char *s)
{
	unsigned char *output, *input_walk, *output_walk;

	if (s == NULL) {
		return NULL;
	}

	output = malloc((strlen(s) * 5) + 1);
	if (output == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		return NULL;
	}

	input_walk = s;
	output_walk = output;

	while (*input_walk != '\0') {
		if (isalnum(*input_walk)) {
			*output_walk++ = *input_walk++;
			continue;
		}

		switch (*input_walk) {
			case '-':
				if (output_walk == output) {
					break;
				}
			case '#':
				if (output_walk == output) {
					*output_walk++ = '_';
					break;
				}	/* else fall through */
			case '~':
			case '%':
			case '^':
			case '_':
			case ',':
			case '.':
			case '+':
			case '=':
				*output_walk++ = *input_walk;
				break;

			case '&':
				*output_walk++ = '_';
				*output_walk++ = 'a';
				*output_walk++ = 'n';
				*output_walk++ = 'd';
				*output_walk++ = '_';
				break;

			case ' ':
			case '`':
			case '!':
			case '@':
			case '$':
			case '*':
			case '\\':
			case '|':
			case ':':
			case ';':
			case '"':
			case '\'':
			case '<':
			case '>':
			case '?':
			case '/':
				*output_walk++ = '_';
				break;

			case '(':
			case ')':
			case '[':
			case ']':
			case '{':
			case '}':
				*output_walk = ((output_walk == output) ? '_' : '-');
				output_walk++;
				break;
		}

		input_walk++;
	}

	*output_walk = 0;

	return output;
}


/*
 * Cleans up any CGI encoded characters, in the form "%" followed by 2 hex 
 * digits.
 */
unsigned char *clean_uncgi(unsigned char *s)
{
	unsigned char *output, *input_walk, *output_walk;
	unsigned char conv[3];

	if (s == NULL) {
		return NULL;
	}

	output = malloc(strlen(s) + 1);
	if (output == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		return NULL;
	}

	input_walk = s;
	output_walk = output;

	while (*input_walk != '\0') {
		if (input_walk[0] == '%' && isxdigit(input_walk[1]) && isxdigit(input_walk[2])) {
			conv[0] = input_walk[1];
			conv[1] = input_walk[2];
			conv[2] = 0;
			*output_walk++ = (unsigned char)strtol(conv, NULL, 16);
			input_walk += 3;
		} 
		else {
			*output_walk++ = *input_walk++;
		}
	}

	*output_walk = 0;

	return output;
}


/*
 * Reduces any series of "_" and "-" to a single character.  "-" takes
 * precedence.
 *
 * If "remove_trailing" is set to non-zero, then "." is added to the
 * comparison, and takes precedence.  This has the effect of reducing "-." or
 * "._", etc, to ".".
 */ 
unsigned char *clean_wipeup(unsigned char *s, int remove_trailing)
{
	unsigned char *output, *input_walk, *output_walk;
	int matched;

	if (s == NULL) {
		return NULL;
	}

	/* remove any - or _ at beginning of string */
	while (*s == '-' || *s == '_') {
		s++;
	}

	output = malloc(strlen(s) + 1);
	if (output == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		return NULL;
	}

	input_walk = s;
	output_walk = output;
	matched = 0;

	while (*input_walk != '\0') {
		switch (*input_walk) {
			case '-':
				if (matched) {
					if (*output_walk == '_') {
						*output_walk = '-';
					}
				}
				else {
					*output_walk = '-';
				}

				matched = 1;
				break;

			case '_':
				if (!matched) {
					*output_walk = '_';
				}

				matched = 1;
				break;

			case '.':
				if (remove_trailing) {
					*output_walk = '.';
					matched = 1;
					break;
				} /* else fall through */
	
			default:
				if (matched) {
					output_walk++;
					matched = 0;
				}

				*output_walk++ = *input_walk;
		}
		input_walk++;
	}

	if (matched) {
		output_walk++;
	}

	*output_walk = 0;

	return output;
}
