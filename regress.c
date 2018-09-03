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
 * $Id: regress.c,v 1.3 2004/02/15 00:01:38 purgedhalo Exp $
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#include "clean_string.h"

char *test_iso8859_1[] = {
	"test\231.mp3",
	"test\251.mp3",
	"test\256.mp3",
	"test\306.mp3",
	"test\337.mp3",
	"test\243.mp3",
	"test\243\337\306\256\251\231.mp3",
	"test\200\201\376\377.mp3",
	NULL,
};

char *test_safe[] = {
	"test().mp3",
	"~test.mp3",
	"-test.mp3",
	"--test.mp3",
	"@test@.mp3",
	"~~~test~~~.mp3",
	"test&test%.mp3",
	"test&test$$$.mp3",
	NULL,
};

char *test_uncgi[] = {
	"test%20.mp3",
	"test%2f.mp3",
	"test%2g.mp3",
	"test%2g%%.mp3",
};

char *test_wipeup[] = {
	"__test__blah__.mp3",
	"_-_test_-_blah_-_.mp3",
	"_-test_-blah_-.mp3",
	"-_test-_blah-_.mp3",
	"_test_blah_.mp3",
	"-test-blah-.mp3",
	"_________test__blah__.mp3",
	"_________test-----__blah_----_--_-__.mp3",
	NULL
};

int main(void)
{
	char **t;
	char *work, *hold;
	int i;

	/* test iso8859-1 */
	printf("-----------------------------------\ntest: iso8859.1\n");
	t = test_iso8859_1;

	while (*t != NULL) {
		work = clean_iso8859_1(*t);
		printf("%s -> %s\n", *t, work);
		if (work != NULL) {
			free(work);
		}
		t++;
	}

	/* test safe */
	printf("-----------------------------------\ntest: safe\n");
	t = test_safe;

	while (*t != NULL) {
		work = clean_safe(*t);
		printf("%s -> %s\n", *t, work);
		if (work != NULL) {
			free(work);
		}
		t++;
	}

	/* test uncgi */
	printf("-----------------------------------\ntest: uncgi\n");
	t = test_uncgi;

	while (*t != NULL) {
		work = clean_uncgi(*t);
		printf("%s -> %s\n", *t, work);
		if (work != NULL) {
			free(work);
		}
		t++;
	}

	/* test wipeup */
	printf("-----------------------------------\ntest: wipeup\n");
	t = test_wipeup;

	while (*t != NULL) {
		work = clean_wipeup(*t, 0);
		printf("%s -> %s\n", *t, work);
		if (work != NULL) {
			free(work);
		}
		t++;
	}

	/* test wipeup, with remove trailing on */
	printf("-----------------------------------\ntest: wipeup, with remove trailing on\n");
	t = test_wipeup;

	while (*t != NULL) {
		work = clean_wipeup(*t, 1);
		printf("%s -> %s\n", *t, work);
		if (work != NULL) {
			free(work);
		}
		t++;
	}


	/* test total */
	printf("-----------------------------------\ntest: total\n");

	for (i = 0; i < 4; i++) {
		switch (i) {
		case 0:
			t = test_iso8859_1;
			break;
		case 1:
			t = test_safe;
			break;
		case 2:
			t = test_uncgi;
			break;
		case 3:
			t = test_wipeup;
			break;
		}

		while (*t != NULL) {
			work = clean_uncgi(*t);

			hold = clean_iso8859_1(work);
			if (work != NULL) {
				free(work);
			}
			work = hold;

			hold = clean_safe(work);
			if (work != NULL) {
				free(work);
			}
			work = hold;

			hold = clean_wipeup(work, 0);
			if (work != NULL) {
				free(work);
			}
			work = hold;

			printf("%s -> %s\n", *t, work);
			if (work != NULL) {
				free(work);
			}
			t++;
		}
	}


	/* test total, with remove trailing on */
	printf("-----------------------------------\ntest: total, with remove trailing on\n");

	for (i = 0; i < 4; i++) {
		switch (i) {
		case 0:
			t = test_iso8859_1;
			break;
		case 1:
			t = test_safe;
			break;
		case 2:
			t = test_uncgi;
			break;
		case 3:
			t = test_wipeup;
			break;
		}

		while (*t != NULL) {
			work = clean_uncgi(*t);

			hold = clean_iso8859_1(work);
			if (work != NULL) {
				free(work);
			}
			work = hold;

			hold = clean_safe(work);
			if (work != NULL) {
				free(work);
			}
			work = hold;

			hold = clean_wipeup(work, 1);
			if (work != NULL) {
				free(work);
			}
			work = hold;

			printf("%s -> %s\n", *t, work);
			if (work != NULL) {
				free(work);
			}
			t++;
		}
	}

	return 0;
}
