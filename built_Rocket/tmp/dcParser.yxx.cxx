
/*  A Bison parser, made from direct/src/dcparser/dcparser.yxx with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse dcyyparse
#define yylex dcyylex
#define yyerror dcyyerror
#define yylval dcyylval
#define yychar dcyychar
#define yydebug dcyydebug
#define yynerrs dcyynerrs
#define	UNSIGNED_INTEGER	258
#define	SIGNED_INTEGER	259
#define	REAL	260
#define	STRING	261
#define	HEX_STRING	262
#define	IDENTIFIER	263
#define	KEYWORD	264
#define	KW_DCLASS	265
#define	KW_STRUCT	266
#define	KW_FROM	267
#define	KW_IMPORT	268
#define	KW_TYPEDEF	269
#define	KW_KEYWORD	270
#define	KW_SWITCH	271
#define	KW_CASE	272
#define	KW_DEFAULT	273
#define	KW_BREAK	274
#define	KW_INT8	275
#define	KW_INT16	276
#define	KW_INT32	277
#define	KW_INT64	278
#define	KW_UINT8	279
#define	KW_UINT16	280
#define	KW_UINT32	281
#define	KW_UINT64	282
#define	KW_FLOAT64	283
#define	KW_STRING	284
#define	KW_BLOB	285
#define	KW_BLOB32	286
#define	KW_INT8ARRAY	287
#define	KW_INT16ARRAY	288
#define	KW_INT32ARRAY	289
#define	KW_UINT8ARRAY	290
#define	KW_UINT16ARRAY	291
#define	KW_UINT32ARRAY	292
#define	KW_UINT32UINT8ARRAY	293
#define	KW_CHAR	294
#define	START_DC	295
#define	START_PARAMETER_VALUE	296
#define	START_PARAMETER_DESCRIPTION	297

#line 6 "direct/src/dcparser/dcparser.yxx"

#include "dcLexerDefs.h"
#include "dcParserDefs.h"
#include "dcFile.h"
#include "dcClass.h"
#include "dcSwitch.h"
#include "dcAtomicField.h"
#include "dcMolecularField.h"
#include "dcClassParameter.h"
#include "dcSwitchParameter.h"
#include "dcArrayParameter.h"
#include "dcSimpleParameter.h"
#include "dcTypedef.h"
#include "dcKeyword.h"
#include "dcPacker.h"
#include "dcNumericRange.h"

// Because our token type contains objects of type string, which
// require correct copy construction (and not simply memcpying), we
// cannot use bison's built-in auto-stack-grow feature.  As an easy
// solution, we ensure here that we have enough yacc stack to start
// with, and that it doesn't ever try to grow.
#define YYINITDEPTH 1000
#define YYMAXDEPTH 1000

DCFile *dc_file = (DCFile *)NULL;
static DCClass *current_class = (DCClass *)NULL;
static DCSwitch *current_switch = (DCSwitch *)NULL;
static DCAtomicField *current_atomic = (DCAtomicField *)NULL;
static DCMolecularField *current_molecular = (DCMolecularField *)NULL;
static DCParameter *current_parameter = (DCParameter *)NULL;
static DCKeywordList current_keyword_list;
static DCPacker default_packer;
static DCPacker *current_packer;
static DCDoubleRange double_range;
static DCUnsignedIntRange uint_range;
static DCField *parameter_description = (DCField *)NULL;

////////////////////////////////////////////////////////////////////
// Defining the interface to the parser.
////////////////////////////////////////////////////////////////////

void
dc_init_parser(istream &in, const string &filename, DCFile &file) {
  dc_file = &file;
  dc_init_lexer(in, filename);
}

void
dc_init_parser_parameter_value(istream &in, const string &filename, 
                               DCPacker &packer) {
  dc_file = NULL;
  current_packer = &packer;
  dc_init_lexer(in, filename);
  dc_start_parameter_value();
}

void
dc_init_parser_parameter_description(istream &in, const string &filename,
                                     DCFile *file) {
  dc_file = file;
  dc_init_lexer(in, filename);
  parameter_description = NULL;
  dc_start_parameter_description();
}

DCField *
dc_get_parameter_description() {
  return parameter_description;
}

void
dc_cleanup_parser() {
  dc_file = (DCFile *)NULL;
}


#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		282
#define	YYFLAG		-32768
#define	YYNTBASE	58

#define YYTRANSLATE(x) ((unsigned)(x) <= 297 ? yytranslate[x] : 139)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,    54,     2,     2,    51,
    52,    46,     2,    47,    55,    45,    44,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    50,    43,     2,
    53,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    56,     2,    57,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    48,     2,    49,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     3,     6,     9,    11,    14,    17,    20,    23,    26,
    29,    31,    35,    37,    41,    44,    45,    51,    53,    55,
    57,    61,    64,    67,    69,    72,    75,    77,    79,    80,
    88,    90,    92,    95,    97,   101,   103,   106,   110,   113,
   116,   119,   122,   123,   131,   133,   135,   138,   140,   144,
   146,   149,   153,   156,   159,   162,   165,   166,   172,   174,
   176,   178,   182,   184,   185,   189,   191,   193,   194,   199,
   201,   202,   207,   209,   211,   213,   215,   217,   219,   222,
   225,   228,   230,   235,   239,   243,   245,   247,   249,   251,
   253,   255,   259,   262,   266,   272,   277,   279,   281,   285,
   288,   292,   298,   303,   305,   310,   312,   316,   320,   325,
   327,   329,   331,   333,   335,   337,   339,   341,   343,   345,
   347,   349,   350,   355,   357,   359,   361,   363,   365,   366,
   371,   372,   377,   378,   383,   387,   391,   395,   399,   401,
   404,   406,   408,   410,   414,   416,   418,   420,   422,   424,
   426,   428,   430,   432,   434,   436,   438,   440,   442,   444,
   446,   448,   450,   452,   454,   456,   459,   461,   462,   467,
   469,   471,   475,   477,   479,   480,   490,   492,   495,   498,
   501,   505,   509,   510,   515,   518,   520,   522,   524
};

static const short yyrhs[] = {    40,
    59,     0,    41,   113,     0,    42,    99,     0,   138,     0,
    59,    43,     0,    59,    69,     0,    59,   130,     0,    59,
    62,     0,    59,    66,     0,    59,    67,     0,     8,     0,
    60,    44,     8,     0,    60,     0,    61,    45,    60,     0,
    13,    61,     0,     0,    12,    61,    13,    63,    64,     0,
    65,     0,    46,     0,    60,     0,    65,    47,    60,     0,
    14,    97,     0,    15,    68,     0,   138,     0,    68,     8,
     0,    68,     9,     0,    70,     0,    77,     0,     0,    10,
   129,    71,    73,    48,    75,    49,     0,     8,     0,   138,
     0,    50,    74,     0,    72,     0,    74,    47,    72,     0,
   138,     0,    75,    43,     0,    75,    76,    43,     0,    84,
   123,     0,   125,   124,     0,    94,   123,     0,    92,   123,
     0,     0,    11,   129,    78,    80,    48,    82,    49,     0,
     8,     0,   138,     0,    50,    81,     0,    79,     0,    81,
    47,    79,     0,   138,     0,    82,    43,     0,    82,    83,
    43,     0,    84,   124,     0,   125,   124,     0,    94,   124,
     0,    92,   124,     0,     0,   129,    51,    85,    86,    52,
     0,   138,     0,    87,     0,    88,     0,    87,    47,    88,
     0,    97,     0,     0,   104,    90,   105,     0,   104,     0,
    89,     0,     0,    89,    53,    93,   113,     0,    91,     0,
     0,    91,    53,    95,   113,     0,    89,     0,    91,     0,
    92,     0,    94,     0,    96,     0,    84,     0,    84,   124,
     0,    94,   124,     0,    92,   124,     0,   122,     0,   100,
    51,   102,    52,     0,   100,    44,   107,     0,   100,    54,
   111,     0,   100,     0,     8,     0,    77,     0,   130,     0,
   138,     0,   112,     0,   112,    55,   112,     0,   112,   111,
     0,   102,    47,   112,     0,   102,    47,   112,    55,   112,
     0,   102,    47,   112,   111,     0,   138,     0,   106,     0,
   106,    55,   106,     0,   106,   108,     0,   103,    47,   106,
     0,   103,    47,   106,    55,   106,     0,   103,    47,   106,
   108,     0,   101,     0,   104,    56,   103,    57,     0,     8,
     0,   105,    44,   107,     0,   105,    54,   111,     0,   105,
    56,   103,    57,     0,     6,     0,   107,     0,     3,     0,
     4,     0,     4,     0,     3,     0,   110,     0,   109,     0,
     5,     0,     6,     0,   111,     0,   115,     0,     0,     8,
    53,   114,   115,     0,   109,     0,   110,     0,     5,     0,
     6,     0,     7,     0,     0,    48,   116,   119,    49,     0,
     0,    56,   117,   119,    57,     0,     0,    51,   118,   119,
    52,     0,   109,    46,   107,     0,   110,    46,   107,     0,
     5,    46,   107,     0,     7,    46,   107,     0,   120,     0,
   121,   120,     0,   138,     0,    47,     0,   113,     0,   121,
    47,   113,     0,    20,     0,    21,     0,    22,     0,    23,
     0,    24,     0,    25,     0,    26,     0,    27,     0,    28,
     0,    29,     0,    30,     0,    31,     0,    32,     0,    33,
     0,    34,     0,    35,     0,    36,     0,    37,     0,    38,
     0,    39,     0,   138,     0,   123,     9,     0,   123,     0,
     0,     8,    50,   126,   128,     0,     8,     0,   127,     0,
   128,    47,   127,     0,   138,     0,     8,     0,     0,    16,
   129,    51,    98,    52,    48,   131,   132,    49,     0,   138,
     0,   132,    43,     0,   132,   133,     0,   132,   135,     0,
   132,   136,    43,     0,   132,   137,    43,     0,     0,    17,
   134,   113,    50,     0,    18,    50,     0,    19,     0,    94,
     0,    92,     0,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   167,   169,   170,   176,   178,   179,   190,   196,   197,   198,
   201,   203,   209,   211,   217,   222,   227,   229,   231,   237,
   242,   248,   266,   270,   272,   276,   286,   288,   291,   296,
   303,   326,   328,   331,   338,   351,   353,   354,   366,   377,
   378,   386,   395,   400,   407,   430,   432,   435,   442,   450,
   452,   453,   463,   471,   472,   476,   482,   493,   500,   502,
   505,   507,   510,   519,   525,   530,   534,   536,   545,   564,
   566,   575,   594,   596,   599,   601,   604,   609,   615,   620,
   624,   630,   635,   644,   660,   674,   676,   708,   724,   742,
   747,   754,   761,   770,   776,   782,   792,   797,   804,   811,
   818,   824,   830,   838,   840,   850,   856,   871,   886,   892,
   902,   905,   916,   930,   934,   938,   943,   947,   950,   960,
   964,   968,   978,   982,   987,   991,   995,   999,  1003,  1007,
  1011,  1015,  1019,  1023,  1027,  1033,  1039,  1045,  1053,  1055,
  1058,  1060,  1063,  1065,  1068,  1073,  1077,  1081,  1085,  1089,
  1093,  1097,  1101,  1105,  1109,  1113,  1117,  1121,  1125,  1129,
  1133,  1137,  1141,  1145,  1151,  1156,  1162,  1171,  1177,  1182,
  1209,  1216,  1229,  1234,  1237,  1242,  1250,  1252,  1253,  1254,
  1255,  1256,  1268,  1275,  1289,  1298,  1305,  1310,  1316
};

static const char * const yytname[] = {   "$","error","$undefined.","UNSIGNED_INTEGER",
"SIGNED_INTEGER","REAL","STRING","HEX_STRING","IDENTIFIER","KEYWORD","KW_DCLASS",
"KW_STRUCT","KW_FROM","KW_IMPORT","KW_TYPEDEF","KW_KEYWORD","KW_SWITCH","KW_CASE",
"KW_DEFAULT","KW_BREAK","KW_INT8","KW_INT16","KW_INT32","KW_INT64","KW_UINT8",
"KW_UINT16","KW_UINT32","KW_UINT64","KW_FLOAT64","KW_STRING","KW_BLOB","KW_BLOB32",
"KW_INT8ARRAY","KW_INT16ARRAY","KW_INT32ARRAY","KW_UINT8ARRAY","KW_UINT16ARRAY",
"KW_UINT32ARRAY","KW_UINT32UINT8ARRAY","KW_CHAR","START_DC","START_PARAMETER_VALUE",
"START_PARAMETER_DESCRIPTION","';'","'/'","'.'","'*'","','","'{'","'}'","':'",
"'('","')'","'='","'%'","'-'","'['","']'","grammar","dc","slash_identifier",
"import_identifier","import","@1","import_symbol_list_or_star","import_symbol_list",
"typedef_decl","keyword_decl","keyword_decl_list","dclass_or_struct","dclass",
"@2","dclass_name","dclass_derivation","dclass_base_list","dclass_fields","dclass_field",
"struct","@3","struct_name","struct_derivation","struct_base_list","struct_fields",
"struct_field","atomic_field","@4","parameter_list","nonempty_parameter_list",
"atomic_element","named_parameter","@5","unnamed_parameter","named_parameter_with_default",
"@6","unnamed_parameter_with_default","@7","parameter","parameter_with_default",
"parameter_or_atomic","parameter_description","simple_type_name","type_name",
"double_range","uint_range","type_definition","parameter_definition","char_or_uint",
"small_unsigned_integer","small_negative_integer","signed_integer","unsigned_integer",
"number","char_or_number","parameter_value","@8","parameter_actual_value","@9",
"@10","@11","array","maybe_comma","array_def","type_token","keyword_list","no_keyword_list",
"molecular_field","@12","atomic_name","molecular_atom_list","optional_name",
"switch","@13","switch_fields","switch_case","@14","switch_default","switch_break",
"switch_field","empty",""
};
#endif

static const short yyr1[] = {     0,
    58,    58,    58,    59,    59,    59,    59,    59,    59,    59,
    60,    60,    61,    61,    62,    63,    62,    64,    64,    65,
    65,    66,    67,    68,    68,    68,    69,    69,    71,    70,
    72,    73,    73,    74,    74,    75,    75,    75,    76,    76,
    76,    76,    78,    77,    79,    80,    80,    81,    81,    82,
    82,    82,    83,    83,    83,    83,    85,    84,    86,    86,
    87,    87,    88,    90,    89,    91,    92,    93,    92,    94,
    95,    94,    96,    96,    97,    97,    98,    98,    99,    99,
    99,   100,   100,   100,   100,   101,   101,   101,   101,   102,
   102,   102,   102,   102,   102,   102,   103,   103,   103,   103,
   103,   103,   103,   104,   104,   105,   105,   105,   105,   106,
   106,   107,   108,   109,   110,   111,   111,   111,   112,   112,
   113,   114,   113,   115,   115,   115,   115,   115,   116,   115,
   117,   115,   118,   115,   115,   115,   115,   115,   119,   119,
   120,   120,   121,   121,   122,   122,   122,   122,   122,   122,
   122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
   122,   122,   122,   122,   123,   123,   124,   126,   125,   127,
   128,   128,   129,   129,   131,   130,   132,   132,   132,   132,
   132,   132,   134,   133,   135,   136,   137,   137,   138
};

static const short yyr2[] = {     0,
     2,     2,     2,     1,     2,     2,     2,     2,     2,     2,
     1,     3,     1,     3,     2,     0,     5,     1,     1,     1,
     3,     2,     2,     1,     2,     2,     1,     1,     0,     7,
     1,     1,     2,     1,     3,     1,     2,     3,     2,     2,
     2,     2,     0,     7,     1,     1,     2,     1,     3,     1,
     2,     3,     2,     2,     2,     2,     0,     5,     1,     1,
     1,     3,     1,     0,     3,     1,     1,     0,     4,     1,
     0,     4,     1,     1,     1,     1,     1,     1,     2,     2,
     2,     1,     4,     3,     3,     1,     1,     1,     1,     1,
     1,     3,     2,     3,     5,     4,     1,     1,     3,     2,
     3,     5,     4,     1,     4,     1,     3,     3,     4,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     0,     4,     1,     1,     1,     1,     1,     0,     4,
     0,     4,     0,     4,     3,     3,     3,     3,     1,     2,
     1,     1,     1,     3,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     2,     1,     0,     4,     1,
     1,     3,     1,     1,     0,     9,     1,     2,     2,     2,
     3,     3,     0,     4,     2,     1,     1,     1,     0
};

static const short yydefact[] = {     0,
   189,     0,   189,     1,     4,   115,   114,   126,   127,   128,
     0,   129,   133,   131,   124,   125,     2,   121,    87,   189,
   189,   145,   146,   147,   148,   149,   150,   151,   152,   153,
   154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
   164,    88,   189,    67,    70,   189,   189,     3,    86,   104,
    66,    82,     0,    89,   173,   189,     0,     0,     0,   189,
     5,     8,     9,    10,     6,    27,    28,     7,     0,     0,
   122,   189,   189,   189,     0,     0,   174,    43,     0,   167,
    79,   165,    68,    71,    81,    80,     0,   189,     0,   189,
     0,    57,    29,    11,    13,     0,    15,    87,    75,    76,
    22,    23,    24,   112,   137,   138,     0,   142,   143,     0,
   139,   189,   141,     0,     0,   135,   136,   189,   189,   166,
     0,     0,    84,   118,   119,     0,   117,   116,   120,    91,
    90,    85,   110,     0,    98,   111,    97,   106,    65,   189,
   189,     0,    16,     0,    25,    26,   123,   130,   142,   140,
   134,   132,     0,     0,    46,    78,    73,    74,    77,     0,
    69,    72,     0,    83,     0,    93,     0,   105,   113,     0,
   100,     0,     0,   189,     0,    60,    61,    63,    59,     0,
     0,    32,    12,     0,    14,   144,    45,    48,    47,   189,
     0,    94,    92,   101,    99,   107,   108,     0,    58,     0,
    31,    34,    33,   189,    19,    20,    17,    18,     0,   189,
    50,   175,     0,    96,     0,   103,   109,    62,     0,   189,
    36,     0,    49,    87,    51,    44,     0,   189,   189,   189,
   189,   189,    95,   102,    35,    37,    30,     0,   189,   189,
   189,   189,    21,   168,    52,    53,    56,    55,    54,     0,
   177,    38,    39,    42,    41,    40,     0,   183,     0,   186,
   178,   176,   188,   187,   179,   180,     0,     0,   170,   171,
   169,     0,   185,   181,   182,     0,     0,   172,   184,     0,
     0,     0
};

static const short yydefgoto[] = {   280,
     4,    95,    96,    62,   184,   207,   208,    63,    64,   102,
    65,    66,   141,   202,   181,   203,   220,   238,    42,   118,
   188,   154,   189,   210,   227,    43,   140,   175,   176,   177,
    44,    91,    45,    99,   121,   100,   122,   159,   178,   160,
    48,    49,    50,   126,   134,    51,   139,   135,   136,   171,
    15,    16,   129,   130,   109,   107,    18,    72,    74,    73,
   110,   111,   112,    52,    80,    81,   231,   257,   270,   271,
    53,    54,   232,   250,   265,   272,   266,   267,   268,    82
};

static const short yypact[] = {   117,
-32768,    32,   357,   113,-32768,-32768,-32768,   -35,-32768,   -12,
    -8,-32768,-32768,-32768,    -5,     7,-32768,-32768,     5,    60,
    60,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,    19,    37,-32768,-32768,-32768,    48,-32768,
    10,-32768,    14,-32768,-32768,    60,    93,    93,   389,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   102,   102,
-32768,    22,    22,    22,   102,   102,-32768,-32768,    63,   109,
-32768,-32768,-32768,-32768,-32768,-32768,   102,   158,   162,    16,
   138,-32768,-32768,-32768,   105,    36,   103,-32768,-32768,-32768,
-32768,   124,-32768,-32768,-32768,-32768,   104,-32768,-32768,   122,
-32768,   106,-32768,    99,    97,-32768,-32768,   123,   357,-32768,
    32,    32,-32768,-32768,-32768,    98,-32768,-32768,-32768,    43,
-32768,-32768,-32768,    17,    12,-32768,-32768,-32768,    86,   389,
   125,   164,-32768,    93,-32768,-32768,-32768,-32768,    32,-32768,
-32768,-32768,   166,   128,-32768,-32768,-32768,-32768,-32768,   126,
-32768,-32768,   158,-32768,   158,-32768,    16,-32768,-32768,    16,
-32768,   102,   162,    16,   127,   133,-32768,-32768,-32768,   173,
   134,-32768,-32768,    25,   105,-32768,-32768,-32768,   136,-32768,
   137,    57,-32768,    27,-32768,-32768,-32768,    84,-32768,   389,
-32768,-32768,   139,-32768,-32768,   105,-32768,   140,   166,   289,
-32768,-32768,   158,-32768,    16,-32768,-32768,-32768,   173,   323,
-32768,    93,-32768,    85,-32768,-32768,   141,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,   145,-32768,-32768,
-32768,-32768,   105,-32768,-32768,-32768,-32768,-32768,-32768,   255,
-32768,-32768,   109,   109,   109,-32768,   186,-32768,   146,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,   152,   154,-32768,-32768,
   151,    32,-32768,-32768,-32768,   186,   149,-32768,-32768,   200,
   201,-32768
};

static const short yypgoto[] = {-32768,
-32768,  -127,   144,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,   -15,-32768,-32768,-32768,-32768,   202,-32768,
    -4,-32768,-32768,-32768,-32768,  -114,-32768,-32768,-32768,     8,
    92,-32768,    95,    -1,-32768,     0,-32768,-32768,   153,-32768,
-32768,-32768,-32768,-32768,    41,-32768,-32768,  -112,   -55,    24,
   -79,   -76,   -88,  -113,    -2,-32768,   110,-32768,-32768,-32768,
    65,   112,-32768,-32768,   -71,   -39,     1,-32768,   -60,-32768,
    23,   218,-32768,-32768,-32768,-32768,-32768,-32768,-32768,     3
};


#define	YYLAST		428


static const short yytable[] = {    17,
   132,    46,    47,     5,   156,    55,    85,    86,   127,   127,
    69,   128,   128,   105,   106,   169,   185,   -64,   104,   116,
   117,   133,    55,    55,     6,     7,     8,     9,    10,    11,
   169,   123,    94,    70,     6,     7,     8,     9,    10,    11,
    75,   166,    78,    79,    71,     6,     7,   124,   143,   192,
   127,   193,    76,   128,   194,  -174,   206,   195,    55,     6,
     7,   124,   103,   167,    92,    90,   170,    77,   108,    12,
   205,    83,    13,   168,   113,   113,   113,    14,    93,    12,
   144,   215,    13,   127,   197,   127,   128,    14,   128,    84,
   131,    87,   137,   127,   243,   228,   128,   165,    88,   233,
    94,    89,   234,   214,   104,   239,     6,     7,     8,     9,
    10,   213,   127,   119,   113,   128,   196,   120,   161,   162,
   155,    55,    56,    20,    57,    58,    59,    60,    21,   172,
   167,   145,   146,   127,   244,  -174,   128,   114,   115,   173,
   217,   174,   179,   182,   163,   138,   186,   144,   142,   164,
   151,    12,   149,   152,    13,    61,     1,     2,     3,    14,
     6,     7,   124,   125,     6,     7,   124,   253,   254,   255,
   148,   183,   153,   187,   180,   190,   137,   191,   199,   200,
   201,   204,   209,   245,   212,   219,   222,   252,   246,   247,
   248,   249,   211,   269,   274,   273,   275,   276,   279,   281,
   282,    97,   256,   235,   223,    67,   221,   218,   229,   230,
   157,   101,    55,   158,   198,   278,   147,   216,   240,   241,
   242,    68,    55,   150,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   251,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   263,   264,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    98,     0,     0,    20,     0,     0,     0,   277,
    21,   258,   259,   260,    22,    23,    24,    25,    26,    27,
    28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
    38,    39,    40,    41,     0,     0,   224,   261,     0,    20,
     0,     0,     0,   262,    21,     0,     0,     0,    22,    23,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    39,    40,    41,     0,     0,
   224,   225,     0,    20,     0,     0,     0,   226,    21,     0,
     0,     0,    22,    23,    24,    25,    26,    27,    28,    29,
    30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
    40,    41,     0,     0,    19,   236,     0,    20,     0,     0,
     0,   237,    21,     0,     0,     0,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    98,     0,     0,    20,
     0,     0,     0,     0,    21,     0,     0,     0,    22,    23,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    39,    40,    41
};

static const short yycheck[] = {     2,
    89,     3,     3,     1,   119,     3,    46,    47,    88,    89,
    46,    88,    89,    69,    70,     4,   144,     8,     3,    75,
    76,     6,    20,    21,     3,     4,     5,     6,     7,     8,
     4,    87,     8,    46,     3,     4,     5,     6,     7,     8,
    46,   130,    20,    21,    53,     3,     4,     5,    13,   163,
   130,   165,    46,   130,   167,    51,   184,   170,    56,     3,
     4,     5,    60,    47,    51,    56,    55,     8,    47,    48,
    46,    53,    51,    57,    72,    73,    74,    56,    56,    48,
    45,    55,    51,   163,   173,   165,   163,    56,   165,    53,
    88,    44,    90,   173,   222,   210,   173,    55,    51,   213,
     8,    54,   215,   192,     3,   220,     3,     4,     5,     6,
     7,    55,   192,    51,   112,   192,   172,     9,   121,   122,
   118,   119,    10,    11,    12,    13,    14,    15,    16,    44,
    47,     8,     9,   213,    50,    51,   213,    73,    74,    54,
    57,    56,   140,   141,    47,     8,   149,    45,    44,    52,
    52,    48,    47,    57,    51,    43,    40,    41,    42,    56,
     3,     4,     5,     6,     3,     4,     5,   239,   240,   241,
    49,     8,    50,     8,    50,    48,   174,    52,    52,    47,
     8,    48,    47,    43,    48,    47,    47,    43,   228,   229,
   230,   231,   190,     8,    43,    50,    43,    47,    50,     0,
     0,    58,   242,   219,   209,     4,   204,   200,   210,   210,
   119,    59,   210,   119,   174,   276,   107,   194,   220,   220,
   220,     4,   220,   112,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   232,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   250,   250,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,     8,    -1,    -1,    11,    -1,    -1,    -1,   272,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    -1,    -1,     8,    43,    -1,    11,
    -1,    -1,    -1,    49,    16,    -1,    -1,    -1,    20,    21,
    22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
    32,    33,    34,    35,    36,    37,    38,    39,    -1,    -1,
     8,    43,    -1,    11,    -1,    -1,    -1,    49,    16,    -1,
    -1,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
    28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
    38,    39,    -1,    -1,     8,    43,    -1,    11,    -1,    -1,
    -1,    49,    16,    -1,    -1,    -1,    20,    21,    22,    23,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    39,     8,    -1,    -1,    11,
    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    20,    21,
    22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
    32,    33,    34,    35,    36,    37,    38,    39
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

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
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(FROM,TO,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (from, to, count)
     char *from;
     char *to;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *from, char *to, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 192 "bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#else
#define YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#endif

int
yyparse(YYPARSE_PARAM)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 3:
#line 171 "direct/src/dcparser/dcparser.yxx"
{
  parameter_description = yyvsp[0].u.field;
;
    break;}
case 6:
#line 180 "direct/src/dcparser/dcparser.yxx"
{
  if (!dc_file->add_class(yyvsp[0].u.dclass)) {
    DCClass *old_class = dc_file->get_class_by_name(yyvsp[0].u.dclass->get_name());
    if (old_class != (DCClass *)NULL && old_class->is_bogus_class()) {
      yyerror("Base class defined after its first reference: " + yyvsp[0].u.dclass->get_name());
    } else {
      yyerror("Duplicate class name: " + yyvsp[0].u.dclass->get_name());
    }
  }
;
    break;}
case 7:
#line 191 "direct/src/dcparser/dcparser.yxx"
{
  if (!dc_file->add_switch(yyvsp[0].u.dswitch)) {
    yyerror("Duplicate class name: " + yyvsp[0].u.dswitch->get_name());
  }
;
    break;}
case 12:
#line 204 "direct/src/dcparser/dcparser.yxx"
{
  yyval.str = yyvsp[-2].str + string("/") + yyvsp[0].str;
;
    break;}
case 14:
#line 212 "direct/src/dcparser/dcparser.yxx"
{
  yyval.str = yyvsp[-2].str + string(".") + yyvsp[0].str;
;
    break;}
case 15:
#line 219 "direct/src/dcparser/dcparser.yxx"
{
  dc_file->add_import_module(yyvsp[0].str);
;
    break;}
case 16:
#line 223 "direct/src/dcparser/dcparser.yxx"
{
  dc_file->add_import_module(yyvsp[-1].str);
;
    break;}
case 19:
#line 232 "direct/src/dcparser/dcparser.yxx"
{
  dc_file->add_import_symbol("*");
;
    break;}
case 20:
#line 239 "direct/src/dcparser/dcparser.yxx"
{
  dc_file->add_import_symbol(yyvsp[0].str);
;
    break;}
case 21:
#line 243 "direct/src/dcparser/dcparser.yxx"
{
  dc_file->add_import_symbol(yyvsp[0].str);
;
    break;}
case 22:
#line 250 "direct/src/dcparser/dcparser.yxx"
{
  if (yyvsp[0].u.parameter != (DCParameter *)NULL) {
    DCTypedef *dtypedef = new DCTypedef(yyvsp[0].u.parameter);
    
    if (!dc_file->add_typedef(dtypedef)) {
      DCTypedef *old_typedef = dc_file->get_typedef_by_name(dtypedef->get_name());
      if (old_typedef->is_bogus_typedef()) {
        yyerror("typedef defined after its first reference: " + dtypedef->get_name());
      } else {
        yyerror("Duplicate typedef name: " + dtypedef->get_name());
      }
    }
  }
;
    break;}
case 25:
#line 273 "direct/src/dcparser/dcparser.yxx"
{
  dc_file->add_keyword(yyvsp[0].str);
;
    break;}
case 26:
#line 277 "direct/src/dcparser/dcparser.yxx"
{
  // This keyword has already been defined.  But since we are now
  // explicitly defining it, clear its bitmask, so that we will have a
  // new hash code--doing this will allow us to phase out the
  // historical hash code support later.
  ((DCKeyword *)yyvsp[0].u.keyword)->clear_historical_flag();
;
    break;}
case 29:
#line 293 "direct/src/dcparser/dcparser.yxx"
{
  current_class = new DCClass(dc_file, yyvsp[0].str, false, false);
;
    break;}
case 30:
#line 297 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.dclass = current_class;
  current_class = yyvsp[-4].u.dclass;
;
    break;}
case 31:
#line 305 "direct/src/dcparser/dcparser.yxx"
{
  if (dc_file == (DCFile *)NULL) {
    yyerror("No DCFile available, so no class names are predefined.");
    yyval.u.dclass = NULL;

  } else {
    DCClass *dclass = dc_file->get_class_by_name(yyvsp[0].str);
    if (dclass == (DCClass *)NULL) {
      // Create a bogus class as a forward reference.
      dclass = new DCClass(dc_file, yyvsp[0].str, false, true);
      dc_file->add_class(dclass);
    }
    if (dclass->is_struct()) {
      yyerror("struct name not allowed");
    }
  
    yyval.u.dclass = dclass;
  }
;
    break;}
case 34:
#line 333 "direct/src/dcparser/dcparser.yxx"
{
  if (yyvsp[0].u.dclass != (DCClass *)NULL) {
    current_class->add_parent(yyvsp[0].u.dclass);
  }
;
    break;}
case 35:
#line 339 "direct/src/dcparser/dcparser.yxx"
{
  if (!dc_multiple_inheritance) {
    yyerror("Multiple inheritance is not supported without \"dc-multiple-inheritance 1\" in your Config.prc file.");

  } else {
    if (yyvsp[0].u.dclass != (DCClass *)NULL) {
      current_class->add_parent(yyvsp[0].u.dclass);
    }
  }
;
    break;}
case 38:
#line 355 "direct/src/dcparser/dcparser.yxx"
{
  if (yyvsp[-1].u.field == (DCField *)NULL) {
    // Pass this error up.
  } else if (!current_class->add_field(yyvsp[-1].u.field)) {
    yyerror("Duplicate field name: " + yyvsp[-1].u.field->get_name());
  } else if (yyvsp[-1].u.field->get_number() < 0) {
    yyerror("A non-network field cannot be stored on a dclass");
  }
;
    break;}
case 39:
#line 368 "direct/src/dcparser/dcparser.yxx"
{
  if (yyvsp[-1].u.field != (DCField *)NULL) {
    if (yyvsp[-1].u.field->get_name().empty()) {
      yyerror("Field name required.");
    }
    yyvsp[-1].u.field->copy_keywords(current_keyword_list);
  }
  yyval.u.field = yyvsp[-1].u.field;
;
    break;}
case 41:
#line 379 "direct/src/dcparser/dcparser.yxx"
{
  yyerror("Unnamed parameters are not allowed on a dclass");
  if (yyvsp[-1].u.parameter != (DCField *)NULL) {
    yyvsp[-1].u.parameter->copy_keywords(current_keyword_list);
  }
  yyval.u.field = yyvsp[-1].u.parameter;
;
    break;}
case 42:
#line 387 "direct/src/dcparser/dcparser.yxx"
{
  if (yyvsp[-1].u.parameter != (DCField *)NULL) {
    yyvsp[-1].u.parameter->copy_keywords(current_keyword_list);
  }
  yyval.u.field = yyvsp[-1].u.parameter;
;
    break;}
case 43:
#line 397 "direct/src/dcparser/dcparser.yxx"
{
  current_class = new DCClass(dc_file, yyvsp[0].str, true, false);
;
    break;}
case 44:
#line 401 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.dclass = current_class;
  current_class = yyvsp[-4].u.dclass;
;
    break;}
case 45:
#line 409 "direct/src/dcparser/dcparser.yxx"
{
  if (dc_file == (DCFile *)NULL) {
    yyerror("No DCFile available, so no struct names are predefined.");
    yyval.u.dclass = NULL;

  } else {
    DCClass *dstruct = dc_file->get_class_by_name(yyvsp[0].str);
    if (dstruct == (DCClass *)NULL) {
      // Create a bogus class as a forward reference.
      dstruct = new DCClass(dc_file, yyvsp[0].str, false, true);
      dc_file->add_class(dstruct);
    }
    if (!dstruct->is_struct()) {
      yyerror("struct name required");
    }
  
    yyval.u.dclass = dstruct;
  }
;
    break;}
case 48:
#line 437 "direct/src/dcparser/dcparser.yxx"
{
  if (yyvsp[0].u.dclass != (DCClass *)NULL) {
    current_class->add_parent(yyvsp[0].u.dclass);
  }
;
    break;}
case 49:
#line 443 "direct/src/dcparser/dcparser.yxx"
{
  if (yyvsp[0].u.dclass != (DCClass *)NULL) {
    current_class->add_parent(yyvsp[0].u.dclass);
  }
;
    break;}
case 52:
#line 454 "direct/src/dcparser/dcparser.yxx"
{
  if (yyvsp[-1].u.field == (DCField *)NULL) {
    // Pass this error up.
  } else if (!current_class->add_field(yyvsp[-1].u.field)) {
    yyerror("Duplicate field name: " + yyvsp[-1].u.field->get_name());
  }
;
    break;}
case 53:
#line 465 "direct/src/dcparser/dcparser.yxx"
{
  if (yyvsp[-1].u.field->get_name().empty()) {
    yyerror("Field name required.");
  }
  yyval.u.field = yyvsp[-1].u.field;
;
    break;}
case 55:
#line 473 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.field = yyvsp[-1].u.parameter;
;
    break;}
case 56:
#line 477 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.field = yyvsp[-1].u.parameter;
;
    break;}
case 57:
#line 484 "direct/src/dcparser/dcparser.yxx"
{
  if (current_class == (DCClass *)NULL) {
    yyerror("Cannot define a method outside of a struct or class.");
    DCClass *temp_class = new DCClass(dc_file, "temp", false, false);  // memory leak.
    current_atomic = new DCAtomicField(yyvsp[-1].str, temp_class, false);
  } else {
    current_atomic = new DCAtomicField(yyvsp[-1].str, current_class, false);
  }
;
    break;}
case 58:
#line 494 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.field = current_atomic;
  current_atomic = yyvsp[-2].u.atomic;
;
    break;}
case 63:
#line 512 "direct/src/dcparser/dcparser.yxx"
{
  if (yyvsp[0].u.parameter != (DCParameter *)NULL) {
    current_atomic->add_element(yyvsp[0].u.parameter);
  }
;
    break;}
case 64:
#line 521 "direct/src/dcparser/dcparser.yxx"
{
  current_parameter = yyvsp[0].u.parameter;
;
    break;}
case 65:
#line 525 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.parameter = yyvsp[0].u.parameter;
;
    break;}
case 68:
#line 537 "direct/src/dcparser/dcparser.yxx"
{
  current_packer = &default_packer;
  current_packer->clear_data();
  if (yyvsp[-1].u.parameter != (DCField *)NULL) {
    current_packer->begin_pack(yyvsp[-1].u.parameter);
  }
;
    break;}
case 69:
#line 545 "direct/src/dcparser/dcparser.yxx"
{
  bool is_valid = false;
  if (yyvsp[-3].u.parameter != (DCField *)NULL) {
    is_valid = yyvsp[-3].u.parameter->is_valid();
  }
  if (current_packer->end_pack()) {
    yyvsp[-3].u.parameter->set_default_value(current_packer->get_string());

  } else {
    if (is_valid) {
      yyerror("Invalid default value for type");
    }
    // If the current parameter isn't valid, we don't mind a pack
    // error (there's no way for us to validate the syntax).  So we'll
    // just ignore the default value in this case.
  }
;
    break;}
case 71:
#line 567 "direct/src/dcparser/dcparser.yxx"
{
  current_packer = &default_packer;
  current_packer->clear_data();
  if (yyvsp[-1].u.parameter != (DCField *)NULL) {
    current_packer->begin_pack(yyvsp[-1].u.parameter);
  }
;
    break;}
case 72:
#line 575 "direct/src/dcparser/dcparser.yxx"
{
  bool is_valid = false;
  if (yyvsp[-3].u.parameter != (DCField *)NULL) {
    is_valid = yyvsp[-3].u.parameter->is_valid();
  }
  if (current_packer->end_pack()) {
    yyvsp[-3].u.parameter->set_default_value(current_packer->get_string());

  } else {
    if (is_valid) {
      yyerror("Invalid default value for type");
    }
    // If the current parameter isn't valid, we don't mind a pack
    // error (there's no way for us to validate the syntax).  So we'll
    // just ignore the default value in this case.
  }
;
    break;}
case 77:
#line 606 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.field = yyvsp[0].u.parameter;
;
    break;}
case 78:
#line 610 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.field = yyvsp[0].u.field;
;
    break;}
case 79:
#line 617 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.field = yyvsp[-1].u.field;
;
    break;}
case 80:
#line 621 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.field = yyvsp[-1].u.parameter;
;
    break;}
case 81:
#line 625 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.field = yyvsp[-1].u.parameter;
;
    break;}
case 82:
#line 632 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.parameter = new DCSimpleParameter(yyvsp[0].u.subatomic);
;
    break;}
case 83:
#line 636 "direct/src/dcparser/dcparser.yxx"
{
  DCSimpleParameter *simple_param = yyvsp[-3].u.parameter->as_simple_parameter();
  nassertr(simple_param != (DCSimpleParameter *)NULL, 0);
  if (!simple_param->set_range(double_range)) {
    yyerror("Inappropriate range for type");
  }
  yyval.u.parameter = simple_param;
;
    break;}
case 84:
#line 645 "direct/src/dcparser/dcparser.yxx"
{
  DCSimpleParameter *simple_param = yyvsp[-2].u.parameter->as_simple_parameter();
  nassertr(simple_param != (DCSimpleParameter *)NULL, 0);
  if (!simple_param->is_numeric_type()) {
    yyerror("A divisor is only valid on a numeric type.");

  } else if (!simple_param->set_divisor(yyvsp[0].u.s_uint)) {
    yyerror("Invalid divisor.");

  } else if (simple_param->has_modulus() && !simple_param->set_modulus(simple_param->get_modulus())) {
    // Changing the divisor may change the valid range for the modulus.
    yyerror("Invalid modulus.");
  }
  yyval.u.parameter = simple_param;
;
    break;}
case 85:
#line 661 "direct/src/dcparser/dcparser.yxx"
{ 
  DCSimpleParameter *simple_param = yyvsp[-2].u.parameter->as_simple_parameter();
  nassertr(simple_param != (DCSimpleParameter *)NULL, 0);
  if (!simple_param->is_numeric_type()) {
    yyerror("A divisor is only valid on a numeric type.");

  } else if (!simple_param->set_modulus(yyvsp[0].u.real)) {
    yyerror("Invalid modulus.");
  }
  yyval.u.parameter = simple_param;
;
    break;}
case 87:
#line 677 "direct/src/dcparser/dcparser.yxx"
{
  if (dc_file == (DCFile *)NULL) {
    yyerror("Invalid type.");
    yyval.u.parameter = NULL;

  } else {
    DCTypedef *dtypedef = dc_file->get_typedef_by_name(yyvsp[0].str);
    if (dtypedef == (DCTypedef *)NULL) {
      // Maybe it's a class name.
      DCClass *dclass = dc_file->get_class_by_name(yyvsp[0].str);
      if (dclass != (DCClass *)NULL) {
        // Create an implicit typedef for this.
        dtypedef = new DCTypedef(new DCClassParameter(dclass), true);
      } else {
        // Maybe it's a switch name.
        DCSwitch *dswitch = dc_file->get_switch_by_name(yyvsp[0].str);
        if (dswitch != (DCSwitch *)NULL) {
          // This also gets an implicit typedef.
          dtypedef = new DCTypedef(new DCSwitchParameter(dswitch), true);
        } else {
          // It's an undefined typedef.  Create a bogus forward reference.
          dtypedef = new DCTypedef(yyvsp[0].str);
        }
      }
      
      dc_file->add_typedef(dtypedef);
    }
    
    yyval.u.parameter = dtypedef->make_new_parameter();
  }
;
    break;}
case 88:
#line 709 "direct/src/dcparser/dcparser.yxx"
{
  // This is an inline struct definition.
  if (yyvsp[0].u.dclass == (DCClass *)NULL) {
    yyval.u.parameter = NULL;
  } else {
    if (dc_file != (DCFile *)NULL) {
      dc_file->add_thing_to_delete(yyvsp[0].u.dclass);
    } else {
      // This is a memory leak--this happens when we put an anonymous
      // struct reference within the string passed to
      // DCPackerInterface::check_match().  Maybe it doesn't really matter.
    }
    yyval.u.parameter = new DCClassParameter(yyvsp[0].u.dclass);
  }
;
    break;}
case 89:
#line 725 "direct/src/dcparser/dcparser.yxx"
{
  // This is an inline switch definition.
  if (yyvsp[0].u.dswitch == (DCSwitch *)NULL) {
    yyval.u.parameter = NULL;
  } else {
    if (dc_file != (DCFile *)NULL) {
      dc_file->add_thing_to_delete(yyvsp[0].u.dswitch);
    } else {
      // This is a memory leak--this happens when we put an anonymous
      // switch reference within the string passed to
      // DCPackerInterface::check_match().  Maybe it doesn't really matter.
    }
    yyval.u.parameter = new DCSwitchParameter(yyvsp[0].u.dswitch);
  }
;
    break;}
case 90:
#line 744 "direct/src/dcparser/dcparser.yxx"
{
  double_range.clear();
;
    break;}
case 91:
#line 748 "direct/src/dcparser/dcparser.yxx"
{
  double_range.clear();
  if (!double_range.add_range(yyvsp[0].u.real, yyvsp[0].u.real)) {
    yyerror("Overlapping range");
  }
;
    break;}
case 92:
#line 755 "direct/src/dcparser/dcparser.yxx"
{
  double_range.clear();
  if (!double_range.add_range(yyvsp[-2].u.real, yyvsp[0].u.real)) {
    yyerror("Overlapping range");
  }
;
    break;}
case 93:
#line 762 "direct/src/dcparser/dcparser.yxx"
{
  double_range.clear();
  if (yyvsp[0].u.real >= 0) {
    yyerror("Syntax error");
  } else if (!double_range.add_range(yyvsp[-1].u.real, -yyvsp[0].u.real)) {
    yyerror("Overlapping range");
  }
;
    break;}
case 94:
#line 771 "direct/src/dcparser/dcparser.yxx"
{
  if (!double_range.add_range(yyvsp[0].u.real, yyvsp[0].u.real)) {
    yyerror("Overlapping range");
  }
;
    break;}
case 95:
#line 777 "direct/src/dcparser/dcparser.yxx"
{
  if (!double_range.add_range(yyvsp[-2].u.real, yyvsp[0].u.real)) {
    yyerror("Overlapping range");
  }
;
    break;}
case 96:
#line 783 "direct/src/dcparser/dcparser.yxx"
{
  if (yyvsp[0].u.real >= 0) {
    yyerror("Syntax error");
  } else if (!double_range.add_range(yyvsp[-1].u.real, -yyvsp[0].u.real)) {
    yyerror("Overlapping range");
  }
;
    break;}
case 97:
#line 794 "direct/src/dcparser/dcparser.yxx"
{
  uint_range.clear();
;
    break;}
case 98:
#line 798 "direct/src/dcparser/dcparser.yxx"
{
  uint_range.clear();
  if (!uint_range.add_range(yyvsp[0].u.s_uint, yyvsp[0].u.s_uint)) {
    yyerror("Overlapping range");
  }
;
    break;}
case 99:
#line 805 "direct/src/dcparser/dcparser.yxx"
{
  uint_range.clear();
  if (!uint_range.add_range(yyvsp[-2].u.s_uint, yyvsp[0].u.s_uint)) {
    yyerror("Overlapping range");
  }
;
    break;}
case 100:
#line 812 "direct/src/dcparser/dcparser.yxx"
{
  uint_range.clear();
  if (!uint_range.add_range(yyvsp[-1].u.s_uint, yyvsp[0].u.s_uint)) {
    yyerror("Overlapping range");
  }
;
    break;}
case 101:
#line 819 "direct/src/dcparser/dcparser.yxx"
{
  if (!uint_range.add_range(yyvsp[0].u.s_uint, yyvsp[0].u.s_uint)) {
    yyerror("Overlapping range");
  }
;
    break;}
case 102:
#line 825 "direct/src/dcparser/dcparser.yxx"
{
  if (!uint_range.add_range(yyvsp[-2].u.s_uint, yyvsp[0].u.s_uint)) {
    yyerror("Overlapping range");
  }
;
    break;}
case 103:
#line 831 "direct/src/dcparser/dcparser.yxx"
{
  if (!uint_range.add_range(yyvsp[-1].u.s_uint, yyvsp[0].u.s_uint)) {
    yyerror("Overlapping range");
  }
;
    break;}
case 105:
#line 841 "direct/src/dcparser/dcparser.yxx"
{
  if (yyvsp[-3].u.parameter == (DCParameter *)NULL) {
    yyval.u.parameter = NULL;
  } else {
    yyval.u.parameter = yyvsp[-3].u.parameter->append_array_specification(uint_range);
  }
;
    break;}
case 106:
#line 852 "direct/src/dcparser/dcparser.yxx"
{
  current_parameter->set_name(yyvsp[0].str);
  yyval.u.parameter = current_parameter;
;
    break;}
case 107:
#line 857 "direct/src/dcparser/dcparser.yxx"
{
  DCSimpleParameter *simple_param = yyvsp[-2].u.parameter->as_simple_parameter();
  if (simple_param == NULL || simple_param->get_typedef() != (DCTypedef *)NULL) {
    yyerror("A divisor is only allowed on a primitive type.");

  } else if (!simple_param->is_numeric_type()) {
      yyerror("A divisor is only valid on a numeric type.");

  } else {
    if (!simple_param->set_divisor(yyvsp[0].u.s_uint)) {
      yyerror("Invalid divisor.");
    }
  }
;
    break;}
case 108:
#line 872 "direct/src/dcparser/dcparser.yxx"
{
  DCSimpleParameter *simple_param = yyvsp[-2].u.parameter->as_simple_parameter();
  if (simple_param == NULL || simple_param->get_typedef() != (DCTypedef *)NULL) {
    yyerror("A modulus is only allowed on a primitive type.");

  } else if (!simple_param->is_numeric_type()) {
      yyerror("A modulus is only valid on a numeric type.");

  } else {
    if (!simple_param->set_modulus(yyvsp[0].u.real)) {
      yyerror("Invalid modulus.");
    }
  }
;
    break;}
case 109:
#line 887 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.parameter = yyvsp[-3].u.parameter->append_array_specification(uint_range);
;
    break;}
case 110:
#line 894 "direct/src/dcparser/dcparser.yxx"
{
  if (yyvsp[0].str.length() != 1) {
    yyerror("Single character required.");
    yyval.u.s_uint = 0;
  } else {
    yyval.u.s_uint = (unsigned char)yyvsp[0].str[0];
  }
;
    break;}
case 112:
#line 907 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.s_uint = (unsigned int)yyvsp[0].u.uint64;
  if (yyval.u.s_uint != yyvsp[0].u.uint64) {
    yyerror("Number out of range.");
    yyval.u.s_uint = 1;
  }
;
    break;}
case 113:
#line 918 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.s_uint = (unsigned int)-yyvsp[0].u.int64;
  if (yyvsp[0].u.int64 >= 0) {
    yyerror("Syntax error.");

  } else if (yyval.u.s_uint != -yyvsp[0].u.int64) {
    yyerror("Number out of range.");
    yyval.u.s_uint = 1;
  }
;
    break;}
case 116:
#line 940 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.real = (double)yyvsp[0].u.uint64;
;
    break;}
case 117:
#line 944 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.real = (double)yyvsp[0].u.int64;
;
    break;}
case 119:
#line 952 "direct/src/dcparser/dcparser.yxx"
{
  if (yyvsp[0].str.length() != 1) {
    yyerror("Single character required.");
    yyval.u.real = 0;
  } else {
    yyval.u.real = (double)(unsigned char)yyvsp[0].str[0];
  }
;
    break;}
case 121:
#line 966 "direct/src/dcparser/dcparser.yxx"
{
;
    break;}
case 122:
#line 969 "direct/src/dcparser/dcparser.yxx"
{
  if (yyvsp[-1].str != current_packer->get_current_field_name()) {
    ostringstream strm;
    strm << "Got '" << yyvsp[-1].str << "', expected '" 
         << current_packer->get_current_field_name() << "'";
    yyerror(strm.str());
  }
;
    break;}
case 123:
#line 978 "direct/src/dcparser/dcparser.yxx"
{
;
    break;}
case 124:
#line 984 "direct/src/dcparser/dcparser.yxx"
{
  current_packer->pack_int64(yyvsp[0].u.int64);
;
    break;}
case 125:
#line 988 "direct/src/dcparser/dcparser.yxx"
{
  current_packer->pack_uint64(yyvsp[0].u.uint64);
;
    break;}
case 126:
#line 992 "direct/src/dcparser/dcparser.yxx"
{
  current_packer->pack_double(yyvsp[0].u.real);
;
    break;}
case 127:
#line 996 "direct/src/dcparser/dcparser.yxx"
{
  current_packer->pack_string(yyvsp[0].str);
;
    break;}
case 128:
#line 1000 "direct/src/dcparser/dcparser.yxx"
{
  current_packer->pack_literal_value(yyvsp[0].str);
;
    break;}
case 129:
#line 1004 "direct/src/dcparser/dcparser.yxx"
{
  current_packer->push();
;
    break;}
case 130:
#line 1008 "direct/src/dcparser/dcparser.yxx"
{
  current_packer->pop();
;
    break;}
case 131:
#line 1012 "direct/src/dcparser/dcparser.yxx"
{
  current_packer->push();
;
    break;}
case 132:
#line 1016 "direct/src/dcparser/dcparser.yxx"
{
  current_packer->pop();
;
    break;}
case 133:
#line 1020 "direct/src/dcparser/dcparser.yxx"
{
  current_packer->push();
;
    break;}
case 134:
#line 1024 "direct/src/dcparser/dcparser.yxx"
{
  current_packer->pop();
;
    break;}
case 135:
#line 1028 "direct/src/dcparser/dcparser.yxx"
{
  for (unsigned int i = 0; i < yyvsp[0].u.s_uint; i++) {
    current_packer->pack_int64(yyvsp[-2].u.int64);
  }
;
    break;}
case 136:
#line 1034 "direct/src/dcparser/dcparser.yxx"
{
  for (unsigned int i = 0; i < yyvsp[0].u.s_uint; i++) {
    current_packer->pack_uint64(yyvsp[-2].u.uint64);
  }
;
    break;}
case 137:
#line 1040 "direct/src/dcparser/dcparser.yxx"
{
  for (unsigned int i = 0; i < yyvsp[0].u.s_uint; i++) {
    current_packer->pack_double(yyvsp[-2].u.real);
  }
;
    break;}
case 138:
#line 1046 "direct/src/dcparser/dcparser.yxx"
{
  for (unsigned int i = 0; i < yyvsp[0].u.s_uint; i++) {
    current_packer->pack_literal_value(yyvsp[-2].str);
  }
;
    break;}
case 145:
#line 1070 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.subatomic = ST_int8;
;
    break;}
case 146:
#line 1074 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.subatomic = ST_int16;
;
    break;}
case 147:
#line 1078 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.subatomic = ST_int32;
;
    break;}
case 148:
#line 1082 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.subatomic = ST_int64;
;
    break;}
case 149:
#line 1086 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.subatomic = ST_uint8;
;
    break;}
case 150:
#line 1090 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.subatomic = ST_uint16;
;
    break;}
case 151:
#line 1094 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.subatomic = ST_uint32;
;
    break;}
case 152:
#line 1098 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.subatomic = ST_uint64;
;
    break;}
case 153:
#line 1102 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.subatomic = ST_float64;
;
    break;}
case 154:
#line 1106 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.subatomic = ST_string;
;
    break;}
case 155:
#line 1110 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.subatomic = ST_blob;
;
    break;}
case 156:
#line 1114 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.subatomic = ST_blob32;
;
    break;}
case 157:
#line 1118 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.subatomic = ST_int8array;
;
    break;}
case 158:
#line 1122 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.subatomic = ST_int16array;
;
    break;}
case 159:
#line 1126 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.subatomic = ST_int32array;
;
    break;}
case 160:
#line 1130 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.subatomic = ST_uint8array;
;
    break;}
case 161:
#line 1134 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.subatomic = ST_uint16array;
;
    break;}
case 162:
#line 1138 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.subatomic = ST_uint32array;
;
    break;}
case 163:
#line 1142 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.subatomic = ST_uint32uint8array;
;
    break;}
case 164:
#line 1146 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.subatomic = ST_char;
;
    break;}
case 165:
#line 1153 "direct/src/dcparser/dcparser.yxx"
{
  current_keyword_list.clear_keywords();
;
    break;}
case 166:
#line 1157 "direct/src/dcparser/dcparser.yxx"
{
  current_keyword_list.add_keyword(yyvsp[0].u.keyword);
;
    break;}
case 167:
#line 1164 "direct/src/dcparser/dcparser.yxx"
{
  if (current_keyword_list.get_num_keywords() != 0) {
    yyerror("Communication keywords are not allowed here.");
  }
;
    break;}
case 168:
#line 1173 "direct/src/dcparser/dcparser.yxx"
{
  current_molecular = new DCMolecularField(yyvsp[-1].str, current_class);
;
    break;}
case 169:
#line 1177 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.field = current_molecular;
;
    break;}
case 170:
#line 1184 "direct/src/dcparser/dcparser.yxx"
{
  DCField *field = current_class->get_field_by_name(yyvsp[0].str);
  yyval.u.atomic = (DCAtomicField *)NULL;
  if (field == (DCField *)NULL) {
    // Maybe the field is unknown because the class is partially
    // bogus.  In that case, allow it for now; create a bogus field as
    // a placeholder.
    if (current_class->inherits_from_bogus_class()) {
      yyval.u.atomic = new DCAtomicField(yyvsp[0].str, current_class, true);
      current_class->add_field(yyval.u.atomic);

    } else {
      // Nope, it's a fully-defined class, so this is a real error.
      yyerror("Unknown field: " + yyvsp[0].str);
    }

  } else {
    yyval.u.atomic = field->as_atomic_field();
    if (yyval.u.atomic == (DCAtomicField *)NULL) {
      yyerror("Not an atomic field: " + yyvsp[0].str);
    }
  }
;
    break;}
case 171:
#line 1211 "direct/src/dcparser/dcparser.yxx"
{
  if (yyvsp[0].u.atomic != (DCAtomicField *)NULL) {
    current_molecular->add_atomic(yyvsp[0].u.atomic);
  }
;
    break;}
case 172:
#line 1217 "direct/src/dcparser/dcparser.yxx"
{
  if (yyvsp[0].u.atomic != (DCAtomicField *)NULL) {
    current_molecular->add_atomic(yyvsp[0].u.atomic);
    if (!yyvsp[0].u.atomic->is_bogus_field() && !current_molecular->compare_keywords(*yyvsp[0].u.atomic)) {
      yyerror("Mismatched keywords in molecule between " + 
              current_molecular->get_atomic(0)->get_name() + " and " +
              yyvsp[0].u.atomic->get_name());
    }
  }
;
    break;}
case 173:
#line 1231 "direct/src/dcparser/dcparser.yxx"
{
  yyval.str = "";
;
    break;}
case 175:
#line 1239 "direct/src/dcparser/dcparser.yxx"
{
  current_switch = new DCSwitch(yyvsp[-4].str, yyvsp[-2].u.field);
;
    break;}
case 176:
#line 1243 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.dswitch = current_switch;
  current_switch = (DCSwitch *)yyvsp[-2].u.parameter;
;
    break;}
case 182:
#line 1257 "direct/src/dcparser/dcparser.yxx"
{
  if (!current_switch->is_field_valid()) {
    yyerror("case declaration required before first element");
  } else if (yyvsp[-1].u.field != (DCField *)NULL) {
    if (!current_switch->add_field(yyvsp[-1].u.field)) {
      yyerror("Duplicate field name: " + yyvsp[-1].u.field->get_name());
    }
  }
;
    break;}
case 183:
#line 1270 "direct/src/dcparser/dcparser.yxx"
{
  current_packer = &default_packer;
  current_packer->clear_data();
  current_packer->begin_pack(current_switch->get_key_parameter());
;
    break;}
case 184:
#line 1276 "direct/src/dcparser/dcparser.yxx"
{
  if (!current_packer->end_pack()) {
    yyerror("Invalid value for switch parameter");
    current_switch->add_invalid_case();
  } else {
    int case_index = current_switch->add_case(current_packer->get_string());
    if (case_index == -1) {
      yyerror("Duplicate case value");
    }
  }
;
    break;}
case 185:
#line 1291 "direct/src/dcparser/dcparser.yxx"
{
  if (!current_switch->add_default()) {
    yyerror("Default case already defined");
  }
;
    break;}
case 186:
#line 1300 "direct/src/dcparser/dcparser.yxx"
{
  current_switch->add_break();
;
    break;}
case 187:
#line 1307 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.field = yyvsp[0].u.parameter;
;
    break;}
case 188:
#line 1311 "direct/src/dcparser/dcparser.yxx"
{
  yyval.u.field = yyvsp[0].u.parameter;
;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 487 "bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#if YYDEBUG != 0
#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
#endif
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 1319 "direct/src/dcparser/dcparser.yxx"
