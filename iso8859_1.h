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
 * $Id: iso8859_1.h,v 1.1 2004/02/15 00:01:37 purgedhalo Exp $
 * 
 *
 * 
 * Special thanks to: http://www.bbsinc.com/iso8859.html
 *
 */

#define ISO8859_1_OFFSET 0x80
#define ISO8859_1_COUNT 128
#define ISO8859_1_MAXLEN 8

/* starts at 80 */
char iso8859_1_trans[ISO8859_1_COUNT][ISO8859_1_MAXLEN] = {
	"_",			/* 0x80 */
	"_",			/* 0x81 */
	"_",			/* 0x82 */
	"_",			/* 0x83 */
	"_",			/* 0x84 */
	"_",			/* 0x85 */
	"_",			/* 0x86 */
	"_",			/* 0x87 */
	"_",			/* 0x88 */
	"_",			/* 0x89 */
	"S",			/* 0x8a */
	"_",			/* 0x8b */
	"OE",			/* 0x8c */
	"_",			/* 0x8d */
	"_",			/* 0x8e */
	"_",			/* 0x8f */
	"_",			/* 0x90 */
	"_",			/* 0x91 */
	"_",			/* 0x92 */
	"_",			/* 0x93 */
	"_",			/* 0x94 */
	"_",			/* 0x95 */
	"-",			/* 0x96 */
	"-",			/* 0x97 */
	"_",			/* 0x98 */
	"_tm_",			/* 0x99 */
	"s",			/* 0x9a */
	"_",			/* 0x9b */
	"oe",			/* 0x9c */
	"_",			/* 0x9d */
	"_",			/* 0x9e */
	"Y",			/* 0x9f */
	"_",			/* 0xa0 */
	"_",			/* 0xa1 */
	"_cent_",		/* 0xa2 */
	"_pound_",		/* 0xa3 */
	"_",			/* 0xa4 */
	"_yen_",		/* 0xa5 */
	"_",			/* 0xa6 */
	"_",			/* 0xa7 */
	"_",			/* 0xa8 */
	"_copy_",		/* 0xa9 */
	"_",			/* 0xaa */
	"_",			/* 0xab */
	"_",			/* 0xac */
	"_",			/* 0xad */
	"_reg_",		/* 0xae */
	"_",			/* 0xaf */
	"_",			/* 0xb0 */
	"_",			/* 0xb1 */
	"2",			/* 0xb2 */
	"3",			/* 0xb3 */
	"_",			/* 0xb4 */
	"b",			/* 0xb5 */
	"_pp_",			/* 0xb6 */
	"_",			/* 0xb7 */
	"_",			/* 0xb8 */
	"1",			/* 0xb9 */
	"_",			/* 0xba */
	"_",			/* 0xbb */
	"_",			/* 0xbc */
	"_",			/* 0xbd */
	"_",			/* 0xbe */
	"_",			/* 0xbf */
	"A",			/* 0xc0 */
	"A",			/* 0xc1 */
	"A",			/* 0xc2 */
	"A",			/* 0xc3 */
	"A",			/* 0xc4 */
	"A",			/* 0xc5 */
	"AE",			/* 0xc6 */
	"C",			/* 0xc7 */
	"E",			/* 0xc8 */
	"E",			/* 0xc9 */
	"E",			/* 0xca */
	"E",			/* 0xcb */
	"I",			/* 0xcc */
	"I",			/* 0xcd */
	"I",			/* 0xce */
	"I",			/* 0xcf */
	"D",			/* 0xd0 */
	"N",			/* 0xd1 */
	"O",			/* 0xd2 */
	"O",			/* 0xd3 */
	"O",			/* 0xd4 */
	"O",			/* 0xd5 */
	"O",			/* 0xd6 */
	"x",			/* 0xd7 */
	"O",			/* 0xd8 */
	"U",			/* 0xd9 */
	"U",			/* 0xda */
	"U",			/* 0xdb */
	"U",			/* 0xdc */
	"Y",			/* 0xdd */
	"_",			/* 0xde */
	"ss",			/* 0xdf -- b?  ss?  sz? */
	"a",			/* 0xe0 */
	"a",			/* 0xe1 */
	"a",			/* 0xe2 */
	"a",			/* 0xe3 */
	"a",			/* 0xe4 */
	"a",			/* 0xe5 */
	"ae",			/* 0xe6 */
	"c",			/* 0xe7 */
	"e",			/* 0xe8 */
	"e",			/* 0xe9 */
	"e",			/* 0xea */
	"e",			/* 0xeb */
	"i",			/* 0xec */
	"i",			/* 0xed */
	"i",			/* 0xee */
	"i",			/* 0xef */
	"o",			/* 0xf0 */
	"n",			/* 0xf1 */
	"o",			/* 0xf2 */
	"o",			/* 0xf3 */
	"o",			/* 0xf4 */
	"o",			/* 0xf5 */
	"o",			/* 0xf6 */
	"_",			/* 0xf7 */
	"u",			/* 0xf8 */
	"u",			/* 0xf9 */
	"u",			/* 0xfa */
	"u",			/* 0xfb */
	"y",			/* 0xfc */
	"_",			/* 0xfd */
	"y",			/* 0xfe */
	"_",			/* 0xff */
};
