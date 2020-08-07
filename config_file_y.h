/* A Bison parser, made from config_file.y, by GNU bison 1.75.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef BISON_Y_TAB_H
# define BISON_Y_TAB_H

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     QSTRING = 258,
     ID = 259,
     SEQUENCE = 260,
     UNCGI = 261,
     ISO8859_1 = 262,
     UTF_8 = 263,
     SAFE = 264,
     WIPEUP = 265,
     MAX_LENGTH = 266,
     LOWER = 267,
     FILENAME = 268,
     REMOVE_TRAILING = 269,
     LENGTH = 270,
     OPEN = 271,
     CLOSE = 272,
     EOL = 273,
     NVALUE = 274
   };
#endif
#define QSTRING 258
#define ID 259
#define SEQUENCE 260
#define UNCGI 261
#define ISO8859_1 262
#define UTF_8 263
#define SAFE 264
#define WIPEUP 265
#define MAX_LENGTH 266
#define LOWER 267
#define FILENAME 268
#define REMOVE_TRAILING 269
#define LENGTH 270
#define OPEN 271
#define CLOSE 272
#define EOL 273
#define NVALUE 274




#ifndef YYSTYPE
#line 62 "config_file.y"
typedef union {
    char	*string;	/* string buffer */
    int		cmd;		/* command value */
    struct detox_sequence *seq;	/* sequence */
    int		nvalue;		/* nvalue */
} yystype;
/* Line 1281 of /usr/local/share/bison/yacc.c.  */
#line 85 "y.tab.h"
# define YYSTYPE yystype
#endif

extern YYSTYPE yylval;


#endif /* not BISON_Y_TAB_H */

