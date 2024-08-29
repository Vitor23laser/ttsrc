
/*  A Bison parser, made from dtool/src/cppparser/cppbison.yxx with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse cppyyparse
#define yylex cppyylex
#define yyerror cppyyerror
#define yylval cppyylval
#define yychar cppyychar
#define yydebug cppyydebug
#define yynerrs cppyynerrs
#define YYLSP_NEEDED

#define	REAL	258
#define	INTEGER	259
#define	CHAR_TOK	260
#define	STRING	261
#define	SIMPLE_IDENTIFIER	262
#define	IDENTIFIER	263
#define	TYPENAME_IDENTIFIER	264
#define	SCOPING	265
#define	TYPEDEFNAME	266
#define	ELLIPSIS	267
#define	OROR	268
#define	ANDAND	269
#define	EQCOMPARE	270
#define	NECOMPARE	271
#define	LECOMPARE	272
#define	GECOMPARE	273
#define	LSHIFT	274
#define	RSHIFT	275
#define	POINTSAT_STAR	276
#define	DOT_STAR	277
#define	UNARY	278
#define	UNARY_NOT	279
#define	UNARY_NEGATE	280
#define	UNARY_MINUS	281
#define	UNARY_STAR	282
#define	UNARY_REF	283
#define	POINTSAT	284
#define	SCOPE	285
#define	PLUSPLUS	286
#define	MINUSMINUS	287
#define	TIMESEQUAL	288
#define	DIVIDEEQUAL	289
#define	MODEQUAL	290
#define	PLUSEQUAL	291
#define	MINUSEQUAL	292
#define	OREQUAL	293
#define	ANDEQUAL	294
#define	XOREQUAL	295
#define	LSHIFTEQUAL	296
#define	RSHIFTEQUAL	297
#define	TOKENPASTE	298
#define	KW_BEGIN_PUBLISH	299
#define	KW_BLOCKING	300
#define	KW_BOOL	301
#define	KW_CATCH	302
#define	KW_CHAR	303
#define	KW_WCHAR_T	304
#define	KW_CLASS	305
#define	KW_CONST	306
#define	KW_DELETE	307
#define	KW_DOUBLE	308
#define	KW_DYNAMIC_CAST	309
#define	KW_ELSE	310
#define	KW_END_PUBLISH	311
#define	KW_ENUM	312
#define	KW_EXTERN	313
#define	KW_EXPLICIT	314
#define	KW_PUBLISHED	315
#define	KW_FALSE	316
#define	KW_FLOAT	317
#define	KW_FRIEND	318
#define	KW_FOR	319
#define	KW_GOTO	320
#define	KW_IF	321
#define	KW_INLINE	322
#define	KW_INT	323
#define	KW_LONG	324
#define	KW_LONGLONG	325
#define	KW_MAKE_SEQ	326
#define	KW_MUTABLE	327
#define	KW_NAMESPACE	328
#define	KW_NEW	329
#define	KW_OPERATOR	330
#define	KW_PRIVATE	331
#define	KW_PROTECTED	332
#define	KW_PUBLIC	333
#define	KW_REGISTER	334
#define	KW_RETURN	335
#define	KW_SHORT	336
#define	KW_SIGNED	337
#define	KW_SIZEOF	338
#define	KW_STATIC	339
#define	KW_STATIC_CAST	340
#define	KW_STRUCT	341
#define	KW_TEMPLATE	342
#define	KW_THROW	343
#define	KW_TRUE	344
#define	KW_TRY	345
#define	KW_TYPEDEF	346
#define	KW_TYPENAME	347
#define	KW_UNION	348
#define	KW_UNSIGNED	349
#define	KW_USING	350
#define	KW_VIRTUAL	351
#define	KW_VOID	352
#define	KW_VOLATILE	353
#define	KW_WHILE	354
#define	START_CPP	355
#define	START_CONST_EXPR	356
#define	START_TYPE	357

#line 6 "dtool/src/cppparser/cppbison.yxx"


#include "cppBisonDefs.h"
#include "cppParser.h"
#include "cppExpression.h"
#include "cppSimpleType.h"
#include "cppExtensionType.h"
#include "cppStructType.h"
#include "cppEnumType.h"
#include "cppFunctionType.h"
#include "cppTBDType.h"
#include "cppMakeSeq.h"
#include "cppParameterList.h"
#include "cppInstance.h"
#include "cppClassTemplateParameter.h"
#include "cppTemplateParameterList.h"
#include "cppInstanceIdentifier.h"
#include "cppTypedef.h"
#include "cppTypeDeclaration.h"
#include "cppVisibility.h"
#include "cppIdentifier.h"
#include "cppScope.h"
#include "cppTemplateScope.h"
#include "cppNamespace.h"
#include "cppUsing.h"

////////////////////////////////////////////////////////////////////
// Defining the interface to the parser.
////////////////////////////////////////////////////////////////////

CPPScope *current_scope = NULL;
CPPScope *global_scope = NULL;
CPPPreprocessor *current_lexer = NULL;

static CPPStructType *current_struct = NULL; 
static CPPEnumType *current_enum = NULL;
static int current_storage_class = 0;
static CPPType *current_type = NULL;
static CPPExpression *current_expr = NULL;
static int publish_nest_level = 0;
static CPPVisibility publish_previous;
static YYLTYPE publish_loc;

static vector<CPPScope *> last_scopes;
static vector<int> last_storage_classes;
static vector<CPPStructType *> last_structs;

int yyparse();

#define YYERROR_VERBOSE

static void
yyerror(const string &msg) {
  current_lexer->error(msg);
}

static void
yyerror(const string &msg, YYLTYPE &loc) {
  current_lexer->error(msg, loc.first_line, loc.first_column);
}

static void
yywarning(const string &msg, YYLTYPE &loc) {
  current_lexer->warning(msg, loc.first_line, loc.first_column);
}

static int
yylex(YYSTYPE *lval, YYLTYPE *lloc) {
  CPPToken token = current_lexer->get_next_token();
  *lval = token._lval;
  *lloc = token._lloc;
  return token._token;
}

void
parse_cpp(CPPParser *cp) {
  CPPScope *old_scope = current_scope;
  CPPScope *old_global_scope = global_scope;
  CPPPreprocessor *old_lexer = current_lexer;

  current_scope = cp;
  global_scope = cp;
  current_lexer = cp;
  publish_nest_level = 0;
  yyparse();

  if (publish_nest_level != 0) {
    yyerror("Unclosed __begin_publish", publish_loc);
    publish_nest_level = 0;
  }

  current_scope = old_scope;
  global_scope = old_global_scope;
  current_lexer = old_lexer;
    
}

CPPExpression *
parse_const_expr(CPPPreprocessor *pp, CPPScope *new_current_scope,
                 CPPScope *new_global_scope) {
  CPPScope *old_scope = current_scope;
  CPPScope *old_global_scope = global_scope;
  CPPPreprocessor *old_lexer = current_lexer;
  CPPExpression *old_expr = current_expr;

  current_scope = new_current_scope;
  global_scope = new_global_scope;
  current_expr = (CPPExpression *)NULL;
  current_lexer = pp;
  yyparse();

  CPPExpression *result = current_expr;

  current_scope = old_scope;
  global_scope = old_global_scope;
  current_lexer = old_lexer;
  current_expr = old_expr;

  return result;
}

CPPType *
parse_type(CPPPreprocessor *pp, CPPScope *new_current_scope,
           CPPScope *new_global_scope) {
  CPPScope *old_scope = current_scope;
  CPPScope *old_global_scope = global_scope;
  CPPPreprocessor *old_lexer = current_lexer;
  CPPType *old_type = current_type;

  current_scope = new_current_scope;
  global_scope = new_global_scope;
  current_type = (CPPType *)NULL;
  current_lexer = pp;
  yyparse();

  CPPType *result = current_type;
  
  current_scope = old_scope;
  global_scope = old_global_scope;
  current_lexer = old_lexer;
  current_type = old_type;

  return result;
}

static void
push_scope(CPPScope *new_scope) {
  last_scopes.push_back(current_scope);
  if (new_scope != NULL) {
    current_scope = new_scope;
  }
}

static void
pop_scope() {
  assert(!last_scopes.empty());
  current_scope = last_scopes.back();
  last_scopes.pop_back();
}

static void
push_storage_class(int new_storage_class) {
  last_storage_classes.push_back(current_storage_class);
  current_storage_class = new_storage_class;
}

static void
pop_storage_class() {
  assert(!last_storage_classes.empty());
  current_storage_class = last_storage_classes.back();
  last_storage_classes.pop_back();
}

static void
push_struct(CPPStructType *new_struct) {
  last_structs.push_back(current_struct);
  current_struct = new_struct;
}

static void
pop_struct() {
  assert(!last_structs.empty());
  current_struct = last_structs.back();
  last_structs.pop_back();
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



#define	YYFINAL		957
#define	YYFLAG		-32768
#define	YYNTBASE	127

#define YYTRANSLATE(x) ((unsigned)(x) <= 357 ? yytranslate[x] : 220)

static const short yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,   125,     2,     2,     2,   118,   111,     2,   121,
   123,   116,   114,   104,   115,   120,   117,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,   106,   105,   112,
   107,   113,   108,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
   122,     2,   126,   110,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,   103,   109,   124,   119,     2,     2,     2,     2,
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
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
    56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
    66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
    76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
    86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
    96,    97,    98,    99,   100,   101,   102
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     3,     6,     9,    11,    14,    17,    19,    23,    28,
    29,    35,    37,    39,    41,    43,    45,    47,    50,    52,
    54,    57,    60,    63,    66,    76,    77,    81,    83,    86,
    90,    93,    96,    99,   102,   105,   108,   111,   114,   116,
   120,   124,   125,   130,   131,   137,   140,   145,   148,   153,
   154,   159,   160,   166,   170,   173,   178,   181,   186,   187,
   194,   195,   202,   203,   211,   212,   223,   224,   236,   237,
   246,   247,   257,   259,   261,   263,   268,   274,   276,   278,
   280,   282,   284,   286,   288,   290,   292,   294,   296,   298,
   300,   302,   304,   306,   308,   310,   312,   314,   316,   318,
   320,   322,   324,   326,   328,   330,   332,   334,   336,   338,
   340,   342,   344,   347,   350,   352,   354,   356,   358,   359,
   366,   368,   370,   372,   376,   379,   384,   388,   393,   395,
   397,   399,   402,   404,   407,   410,   413,   416,   420,   425,
   429,   433,   434,   441,   443,   445,   447,   451,   454,   456,
   460,   462,   465,   467,   470,   472,   476,   482,   486,   491,
   493,   495,   498,   500,   504,   508,   514,   518,   522,   527,
   529,   531,   533,   535,   538,   541,   544,   548,   553,   555,
   557,   559,   562,   565,   568,   572,   577,   585,   589,   591,
   594,   597,   600,   604,   609,   617,   619,   621,   624,   626,
   628,   630,   632,   635,   638,   640,   642,   645,   647,   649,
   651,   653,   656,   659,   661,   663,   666,   669,   672,   675,
   679,   680,   686,   687,   695,   697,   699,   702,   706,   709,
   712,   715,   719,   723,   727,   731,   735,   739,   740,   746,
   747,   754,   756,   758,   761,   763,   767,   771,   777,   779,
   781,   783,   785,   786,   793,   798,   801,   805,   807,   809,
   811,   813,   815,   817,   819,   821,   823,   825,   827,   829,
   832,   835,   838,   841,   843,   846,   848,   852,   855,   857,
   858,   861,   863,   866,   868,   870,   872,   874,   876,   878,
   880,   882,   884,   886,   888,   890,   892,   894,   896,   898,
   900,   902,   904,   906,   908,   910,   912,   914,   916,   918,
   920,   922,   924,   926,   928,   930,   932,   934,   936,   938,
   940,   942,   944,   946,   948,   950,   952,   954,   956,   958,
   960,   962,   964,   966,   968,   970,   972,   974,   976,   978,
   980,   982,   984,   986,   988,   990,   992,   994,   996,   998,
  1000,  1002,  1004,  1006,  1008,  1010,  1012,  1014,  1016,  1018,
  1020,  1022,  1024,  1026,  1028,  1030,  1032,  1034,  1036,  1038,
  1040,  1042,  1044,  1046,  1048,  1050,  1052,  1054,  1056,  1058,
  1060,  1062,  1064,  1066,  1068,  1070,  1072,  1074,  1078,  1080,
  1082,  1084,  1086,  1088,  1092,  1094,  1099,  1107,  1115,  1120,
  1123,  1126,  1129,  1132,  1135,  1139,  1143,  1147,  1151,  1155,
  1159,  1163,  1167,  1171,  1175,  1179,  1183,  1187,  1191,  1195,
  1199,  1205,  1210,  1215,  1219,  1223,  1227,  1231,  1233,  1238,
  1246,  1254,  1259,  1264,  1269,  1274,  1279,  1284,  1289,  1294,
  1299,  1304,  1309,  1314,  1317,  1323,  1326,  1329,  1332,  1335,
  1338,  1342,  1346,  1350,  1354,  1358,  1362,  1366,  1370,  1374,
  1378,  1382,  1386,  1390,  1394,  1398,  1402,  1406,  1410,  1416,
  1421,  1426,  1430,  1434,  1438,  1442,  1444,  1446,  1448,  1450,
  1452,  1454,  1456,  1458,  1463,  1471,  1479,  1484,  1487,  1493,
  1496,  1499,  1502,  1505,  1509,  1513,  1517,  1521,  1525,  1529,
  1533,  1537,  1541,  1545,  1549,  1553,  1557,  1561,  1565,  1569,
  1573,  1577,  1583,  1588,  1593,  1597,  1601,  1605,  1609,  1611,
  1613,  1615,  1617,  1619,  1621,  1623,  1626,  1629,  1631,  1633,
  1635,  1638
};

static const short yyrhs[] = {   100,
   128,     0,   101,   212,     0,   102,   181,     0,   219,     0,
   128,   105,     0,   128,   133,     0,   130,     0,   129,   104,
   130,     0,   217,   121,   209,   123,     0,     0,   136,   103,
   132,   128,   124,     0,   137,     0,   159,     0,   131,     0,
   197,     0,   199,     0,   134,     0,    91,   143,     0,    44,
     0,    56,     0,    60,   106,     0,    78,   106,     0,    77,
   106,     0,    76,   106,     0,    71,   121,   217,   104,   217,
   104,   217,   123,   105,     0,     0,    63,   135,   133,     0,
   219,     0,   136,    58,     0,   136,    58,   218,     0,   136,
    84,     0,   136,    67,     0,   136,    96,     0,   136,    59,
     0,   136,    98,     0,   136,    72,     0,   136,    79,     0,
   136,    45,     0,   138,     0,   136,   179,   105,     0,   136,
   148,   171,     0,     0,   136,   179,   139,   141,     0,     0,
   136,    51,   178,   140,   142,     0,   165,   171,     0,   165,
   170,   104,   141,     0,   165,   171,     0,   165,   170,   104,
   142,     0,     0,   136,   179,   144,   146,     0,     0,   136,
    51,   178,   145,   147,     0,   136,   148,   171,     0,   165,
   171,     0,   165,   170,   104,   146,     0,   165,   171,     0,
   165,   170,   104,   147,     0,     0,     8,   121,   149,   167,
   123,   156,     0,     0,     9,   121,   150,   167,   123,   156,
     0,     0,   119,   217,   121,   151,   167,   123,   156,     0,
     0,     9,   121,   116,   165,   123,   121,   152,   167,   123,
   156,     0,     0,     9,   121,    10,   116,   165,   123,   121,
   153,   167,   123,   156,     0,     0,    75,   178,   175,   121,
   154,   167,   123,   156,     0,     0,    75,    51,   178,   175,
   121,   155,   167,   123,   156,     0,     8,     0,   219,     0,
    51,     0,   156,    88,   121,   123,     0,   156,    88,   121,
   217,   123,     0,   125,     0,   119,     0,   116,     0,   117,
     0,   118,     0,   114,     0,   115,     0,   109,     0,   111,
     0,   110,     0,    13,     0,    14,     0,    15,     0,    16,
     0,    17,     0,    18,     0,   112,     0,   113,     0,    19,
     0,    20,     0,   107,     0,   104,     0,    31,     0,    32,
     0,    33,     0,    34,     0,    35,     0,    36,     0,    37,
     0,    38,     0,    39,     0,    40,     0,    41,     0,    42,
     0,    29,     0,   122,   126,     0,   121,   123,     0,    74,
     0,    52,     0,   137,     0,   159,     0,     0,    87,   160,
   112,   161,   113,   158,     0,   219,     0,   162,     0,   163,
     0,   162,   104,   163,     0,    50,   217,     0,    50,   217,
   107,   181,     0,   164,   176,   169,     0,    51,   164,   176,
   169,     0,   200,     0,     8,     0,     9,     0,    92,   217,
     0,   217,     0,    75,   157,     0,    51,   165,     0,   116,
   165,     0,   111,   165,     0,    10,   116,   165,     0,   165,
   122,   208,   126,     0,   165,   106,     4,     0,   121,   165,
   123,     0,     0,   165,   121,   166,   167,   123,   156,     0,
   219,     0,    12,     0,   168,     0,   168,   104,    12,     0,
   168,    12,     0,   174,     0,   168,   104,   174,     0,   219,
     0,   107,   211,     0,   219,     0,   107,   212,     0,   105,
     0,   103,   204,   124,     0,   106,   129,   103,   204,   124,
     0,   107,   212,   105,     0,   107,   103,   172,   124,     0,
   219,     0,   173,     0,   173,   104,     0,   212,     0,   103,
   172,   124,     0,   173,   104,   212,     0,   173,   104,   103,
   172,   124,     0,   178,   176,   170,     0,     8,   176,   170,
     0,    51,   178,   176,   170,     0,   214,     0,   219,     0,
     8,     0,     9,     0,    51,   175,     0,   116,   175,     0,
   111,   175,     0,    10,   116,   175,     0,   175,   122,   208,
   126,     0,   219,     0,     8,     0,     9,     0,    51,   176,
     0,   116,   176,     0,   111,   176,     0,    10,   116,   176,
     0,   176,   122,   208,   126,     0,   121,   176,   123,   121,
   167,   123,   156,     0,   121,   176,   123,     0,   219,     0,
    51,   177,     0,   116,   177,     0,   111,   177,     0,    10,
   116,   177,     0,   177,   122,   208,   126,     0,   121,   177,
   123,   121,   167,   123,   156,     0,   200,     0,     9,     0,
    92,   217,     0,   182,     0,   184,     0,   189,     0,   191,
     0,   196,   217,     0,   195,   217,     0,   200,     0,     9,
     0,    92,   217,     0,   182,     0,   184,     0,   189,     0,
   191,     0,   196,   217,     0,   195,   217,     0,   200,     0,
     9,     0,    92,   217,     0,   196,   217,     0,   195,   217,
     0,   178,   177,     0,    51,   178,   177,     0,     0,   196,
   103,   183,   128,   124,     0,     0,   196,   217,   185,   186,
   103,   128,   124,     0,   219,     0,   187,     0,   106,   188,
     0,   187,   104,   188,     0,    78,   216,     0,    77,   216,
     0,    76,   216,     0,    96,    78,   216,     0,    96,    77,
   216,     0,    96,    76,   216,     0,    78,    96,   216,     0,
    77,    96,   216,     0,    76,    96,   216,     0,     0,   195,
   103,   190,   193,   124,     0,     0,   195,   217,   103,   192,
   193,   124,     0,   219,     0,   194,     0,   194,   104,     0,
   217,     0,   217,   107,   212,     0,   194,   104,   217,     0,
   194,   104,   217,   107,   212,     0,    57,     0,    50,     0,
    86,     0,    93,     0,     0,    73,   217,   103,   198,   128,
   124,     0,    73,   103,   128,   124,     0,    95,   217,     0,
    95,    73,   217,     0,   201,     0,   202,     0,   203,     0,
    46,     0,    48,     0,    49,     0,    81,     0,    69,     0,
    70,     0,    94,     0,    82,     0,    68,     0,    81,   201,
     0,    69,   201,     0,    94,   201,     0,    82,   201,     0,
    62,     0,    69,    62,     0,    53,     0,    69,    69,    62,
     0,    69,    53,     0,    97,     0,     0,   205,   206,     0,
   219,     0,   206,   207,     0,     3,     0,     4,     0,     6,
     0,     5,     0,     8,     0,     9,     0,    10,     0,     7,
     0,    12,     0,    13,     0,    14,     0,    15,     0,    16,
     0,    17,     0,    18,     0,    19,     0,    20,     0,    21,
     0,    22,     0,    29,     0,    30,     0,    31,     0,    32,
     0,    33,     0,    34,     0,    35,     0,    36,     0,    37,
     0,    38,     0,    39,     0,    40,     0,    41,     0,    42,
     0,    46,     0,    47,     0,    48,     0,    49,     0,    50,
     0,    51,     0,    52,     0,    53,     0,    54,     0,    55,
     0,    57,     0,    58,     0,    59,     0,    61,     0,    62,
     0,    63,     0,    64,     0,    65,     0,    66,     0,    67,
     0,    68,     0,    69,     0,    72,     0,    74,     0,    76,
     0,    77,     0,    78,     0,    60,     0,    79,     0,    80,
     0,    81,     0,    82,     0,    83,     0,    84,     0,    85,
     0,    86,     0,    88,     0,    89,     0,    90,     0,    91,
     0,    92,     0,    93,     0,    94,     0,    96,     0,    97,
     0,    98,     0,    99,     0,    43,     0,    75,     0,   114,
     0,   115,     0,   116,     0,   117,     0,   111,     0,   109,
     0,   110,     0,   125,     0,   119,     0,   107,     0,   118,
     0,   112,     0,   113,     0,   121,     0,   123,     0,   120,
     0,   104,     0,   105,     0,   106,     0,   122,     0,   126,
     0,   108,     0,   103,   206,   124,     0,   219,     0,   212,
     0,   219,     0,   210,     0,   212,     0,   210,   104,   212,
     0,   213,     0,   121,   181,   123,   211,     0,    85,   112,
   181,   113,   121,   210,   123,     0,    54,   112,   181,   113,
   121,   210,   123,     0,    83,   121,   181,   123,     0,   125,
   211,     0,   119,   211,     0,   115,   211,     0,   116,   211,
     0,   111,   211,     0,   211,   116,   211,     0,   211,   117,
   211,     0,   211,   118,   211,     0,   211,   114,   211,     0,
   211,   115,   211,     0,   211,   109,   211,     0,   211,   110,
   211,     0,   211,   111,   211,     0,   211,    13,   211,     0,
   211,    14,   211,     0,   211,    15,   211,     0,   211,    16,
   211,     0,   211,    17,   211,     0,   211,    18,   211,     0,
   211,    19,   211,     0,   211,    20,   211,     0,   211,   108,
   211,   106,   211,     0,   211,   122,   212,   126,     0,   211,
   121,   210,   123,     0,   211,   121,   123,     0,   211,   120,
   211,     0,   211,    29,   211,     0,   121,   210,   123,     0,
   213,     0,   121,   181,   123,   212,     0,    85,   112,   181,
   113,   121,   210,   123,     0,    54,   112,   181,   113,   121,
   210,   123,     0,     9,   121,   209,   123,     0,    68,   121,
   209,   123,     0,    48,   121,   209,   123,     0,    49,   121,
   209,   123,     0,    46,   121,   209,   123,     0,    81,   121,
   209,   123,     0,    69,   121,   209,   123,     0,    94,   121,
   209,   123,     0,    82,   121,   209,   123,     0,    62,   121,
   209,   123,     0,    53,   121,   209,   123,     0,    83,   121,
   181,   123,     0,    74,   180,     0,    74,   180,   121,   209,
   123,     0,   125,   212,     0,   119,   212,     0,   115,   212,
     0,   116,   212,     0,   111,   212,     0,   212,   116,   212,
     0,   212,   117,   212,     0,   212,   118,   212,     0,   212,
   114,   212,     0,   212,   115,   212,     0,   212,   109,   212,
     0,   212,   110,   212,     0,   212,   111,   212,     0,   212,
    13,   212,     0,   212,    14,   212,     0,   212,    15,   212,
     0,   212,    16,   212,     0,   212,    17,   212,     0,   212,
    18,   212,     0,   212,   112,   212,     0,   212,   113,   212,
     0,   212,    19,   212,     0,   212,    20,   212,     0,   212,
   108,   212,   106,   212,     0,   212,   122,   212,   126,     0,
   212,   121,   210,   123,     0,   212,   121,   123,     0,   212,
   120,   212,     0,   212,    29,   212,     0,   121,   210,   123,
     0,     4,     0,    89,     0,    61,     0,     5,     0,     3,
     0,   218,     0,     8,     0,   215,     0,   121,   181,   123,
   212,     0,    85,   112,   181,   113,   121,   210,   123,     0,
    54,   112,   181,   113,   121,   210,   123,     0,    83,   121,
   181,   123,     0,    74,   180,     0,    74,   180,   121,   209,
   123,     0,   125,   212,     0,   119,   212,     0,   115,   212,
     0,   111,   212,     0,   214,   116,   212,     0,   214,   117,
   212,     0,   214,   118,   212,     0,   214,   114,   212,     0,
   214,   115,   212,     0,   214,   109,   212,     0,   214,   110,
   212,     0,   214,   111,   212,     0,   214,    13,   212,     0,
   214,    14,   212,     0,   214,    15,   212,     0,   214,    16,
   212,     0,   214,    17,   212,     0,   214,    18,   212,     0,
   214,   112,   212,     0,   214,   113,   212,     0,   214,    19,
   212,     0,   214,    20,   212,     0,   214,   108,   212,   106,
   212,     0,   214,   122,   212,   126,     0,   214,   121,   210,
   123,     0,   214,   121,   123,     0,   214,   120,   212,     0,
   214,    29,   212,     0,   121,   210,   123,     0,     4,     0,
    89,     0,    61,     0,     5,     0,     3,     0,   218,     0,
   217,     0,   196,   217,     0,    92,   217,     0,     8,     0,
     9,     0,     6,     0,   218,     6,     0,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   380,   382,   386,   392,   394,   395,   398,   400,   403,   415,
   421,   427,   429,   430,   431,   432,   433,   434,   435,   448,
   457,   461,   469,   473,   477,   484,   492,   499,   504,   508,
   519,   523,   527,   531,   535,   539,   543,   547,   553,   558,
   567,   577,   590,   593,   601,   610,   619,   629,   639,   651,
   664,   667,   675,   678,   691,   700,   710,   720,   731,   738,
   756,   760,   780,   784,   810,   814,   825,   829,   842,   848,
   873,   879,   899,   911,   916,   920,   924,   930,   935,   939,
   943,   947,   951,   955,   959,   963,   967,   971,   975,   979,
   983,   987,   991,   995,   999,  1003,  1007,  1011,  1015,  1019,
  1023,  1027,  1031,  1035,  1039,  1043,  1047,  1051,  1055,  1059,
  1063,  1067,  1071,  1075,  1079,  1083,  1089,  1091,  1094,  1099,
  1105,  1107,  1110,  1117,  1125,  1130,  1134,  1140,  1149,  1154,
  1159,  1164,  1171,  1176,  1190,  1195,  1200,  1205,  1210,  1215,
  1220,  1225,  1229,  1246,  1251,  1256,  1260,  1265,  1272,  1278,
  1285,  1290,  1296,  1301,  1307,  1312,  1316,  1320,  1324,  1330,
  1332,  1333,  1336,  1340,  1341,  1342,  1345,  1351,  1359,  1365,
  1374,  1379,  1383,  1387,  1392,  1397,  1402,  1407,  1414,  1419,
  1423,  1427,  1432,  1437,  1442,  1447,  1452,  1458,  1465,  1470,
  1475,  1480,  1485,  1490,  1495,  1503,  1508,  1513,  1517,  1521,
  1525,  1529,  1533,  1549,  1567,  1572,  1577,  1581,  1585,  1589,
  1593,  1597,  1613,  1631,  1636,  1641,  1645,  1661,  1679,  1686,
  1695,  1710,  1719,  1740,  1749,  1751,  1754,  1756,  1759,  1764,
  1768,  1772,  1776,  1780,  1784,  1788,  1792,  1798,  1803,  1810,
  1815,  1822,  1824,  1825,  1828,  1834,  1839,  1844,  1851,  1858,
  1863,  1867,  1873,  1892,  1896,  1899,  1906,  1914,  1916,  1917,
  1920,  1925,  1929,  1933,  1938,  1943,  1948,  1953,  1958,  1962,
  1967,  1976,  1981,  1988,  1993,  1998,  2002,  2007,  2014,  2024,
  2029,  2034,  2036,  2039,  2043,  2046,  2049,  2052,  2055,  2058,
  2061,  2064,  2064,  2064,  2065,  2065,  2065,  2065,  2066,  2066,
  2066,  2066,  2066,  2067,  2067,  2067,  2068,  2068,  2068,  2068,
  2068,  2069,  2069,  2069,  2069,  2069,  2070,  2070,  2070,  2070,
  2070,  2070,  2071,  2071,  2071,  2071,  2071,  2072,  2072,  2072,
  2073,  2073,  2073,  2073,  2074,  2074,  2074,  2075,  2075,  2075,
  2075,  2075,  2076,  2076,  2076,  2076,  2077,  2077,  2077,  2077,
  2077,  2078,  2078,  2078,  2078,  2078,  2078,  2079,  2079,  2079,
  2079,  2079,  2080,  2080,  2081,  2084,  2084,  2084,  2084,  2084,
  2084,  2084,  2084,  2084,  2084,  2084,  2085,  2085,  2085,  2085,
  2085,  2085,  2085,  2085,  2085,  2085,  2086,  2086,  2089,  2094,
  2100,  2105,  2111,  2116,  2122,  2127,  2131,  2135,  2139,  2143,
  2147,  2151,  2163,  2167,  2171,  2175,  2179,  2183,  2187,  2191,
  2195,  2199,  2203,  2207,  2211,  2215,  2219,  2223,  2227,  2231,
  2235,  2239,  2243,  2247,  2251,  2255,  2259,  2266,  2271,  2275,
  2279,  2283,  2290,  2296,  2302,  2308,  2314,  2321,  2328,  2335,
  2342,  2348,  2354,  2358,  2362,  2366,  2370,  2374,  2386,  2390,
  2394,  2398,  2402,  2406,  2410,  2414,  2418,  2422,  2426,  2430,
  2434,  2438,  2442,  2446,  2450,  2454,  2458,  2462,  2466,  2470,
  2474,  2478,  2482,  2486,  2490,  2496,  2501,  2505,  2509,  2513,
  2517,  2521,  2534,  2539,  2543,  2547,  2551,  2555,  2559,  2563,
  2567,  2571,  2583,  2587,  2591,  2595,  2599,  2603,  2607,  2611,
  2615,  2619,  2623,  2627,  2631,  2635,  2639,  2643,  2647,  2651,
  2655,  2659,  2663,  2667,  2671,  2675,  2679,  2683,  2689,  2694,
  2698,  2702,  2706,  2710,  2716,  2725,  2733,  2755,  2760,  2766,
  2771,  2777
};

static const char * const yytname[] = {   "$","error","$undefined.","REAL","INTEGER",
"CHAR_TOK","STRING","SIMPLE_IDENTIFIER","IDENTIFIER","TYPENAME_IDENTIFIER","SCOPING",
"TYPEDEFNAME","ELLIPSIS","OROR","ANDAND","EQCOMPARE","NECOMPARE","LECOMPARE",
"GECOMPARE","LSHIFT","RSHIFT","POINTSAT_STAR","DOT_STAR","UNARY","UNARY_NOT",
"UNARY_NEGATE","UNARY_MINUS","UNARY_STAR","UNARY_REF","POINTSAT","SCOPE","PLUSPLUS",
"MINUSMINUS","TIMESEQUAL","DIVIDEEQUAL","MODEQUAL","PLUSEQUAL","MINUSEQUAL",
"OREQUAL","ANDEQUAL","XOREQUAL","LSHIFTEQUAL","RSHIFTEQUAL","TOKENPASTE","KW_BEGIN_PUBLISH",
"KW_BLOCKING","KW_BOOL","KW_CATCH","KW_CHAR","KW_WCHAR_T","KW_CLASS","KW_CONST",
"KW_DELETE","KW_DOUBLE","KW_DYNAMIC_CAST","KW_ELSE","KW_END_PUBLISH","KW_ENUM",
"KW_EXTERN","KW_EXPLICIT","KW_PUBLISHED","KW_FALSE","KW_FLOAT","KW_FRIEND","KW_FOR",
"KW_GOTO","KW_IF","KW_INLINE","KW_INT","KW_LONG","KW_LONGLONG","KW_MAKE_SEQ",
"KW_MUTABLE","KW_NAMESPACE","KW_NEW","KW_OPERATOR","KW_PRIVATE","KW_PROTECTED",
"KW_PUBLIC","KW_REGISTER","KW_RETURN","KW_SHORT","KW_SIGNED","KW_SIZEOF","KW_STATIC",
"KW_STATIC_CAST","KW_STRUCT","KW_TEMPLATE","KW_THROW","KW_TRUE","KW_TRY","KW_TYPEDEF",
"KW_TYPENAME","KW_UNION","KW_UNSIGNED","KW_USING","KW_VIRTUAL","KW_VOID","KW_VOLATILE",
"KW_WHILE","START_CPP","START_CONST_EXPR","START_TYPE","'{'","','","';'","':'",
"'='","'?'","'|'","'^'","'&'","'<'","'>'","'+'","'-'","'*'","'/'","'%'","'~'",
"'.'","'('","'['","')'","'}'","'!'","']'","grammar","cpp","constructor_inits",
"constructor_init","extern_c","@1","declaration","friend_declaration","@2","storage_class",
"type_like_declaration","multiple_var_declaration","@3","@4","multiple_instance_identifiers",
"multiple_const_instance_identifiers","typedef_declaration","@5","@6","typedef_instance_identifiers",
"typedef_const_instance_identifiers","function_prototype","@7","@8","@9","@10",
"@11","@12","@13","function_post","function_operator","more_template_declaration",
"template_declaration","@14","template_formal_parameters","template_nonempty_formal_parameters",
"template_formal_parameter","template_formal_parameter_type","instance_identifier",
"@15","formal_parameter_list","formal_parameters","template_parameter_maybe_initialize",
"maybe_initialize","maybe_initialize_or_function_body","structure_init","structure_init_body",
"formal_parameter","not_paren_formal_parameter_identifier","formal_parameter_identifier",
"empty_instance_identifier","type","type_decl","predefined_type","full_type",
"anonymous_struct","@16","named_struct","@17","maybe_class_derivation","class_derivation",
"base_specification","anonymous_enum","@18","named_enum","@19","enum_body","enum_body_no_trailing_comma",
"enum_keyword","struct_keyword","namespace_declaration","@20","using_declaration",
"simple_type","simple_int_type","simple_float_type","simple_void_type","code",
"@21","code_block","element","optional_const_expr","optional_const_expr_comma",
"const_expr_comma","no_angle_bracket_const_expr","const_expr","const_operand",
"formal_const_expr","formal_const_operand","class_derivation_name","name","string",
"empty",""
};
#endif

static const short yyr1[] = {     0,
   127,   127,   127,   128,   128,   128,   129,   129,   130,   132,
   131,   133,   133,   133,   133,   133,   133,   133,   133,   133,
   133,   133,   133,   133,   133,   135,   134,   136,   136,   136,
   136,   136,   136,   136,   136,   136,   136,   136,   137,   137,
   137,   139,   138,   140,   138,   141,   141,   142,   142,   144,
   143,   145,   143,   143,   146,   146,   147,   147,   149,   148,
   150,   148,   151,   148,   152,   148,   153,   148,   154,   148,
   155,   148,   148,   156,   156,   156,   156,   157,   157,   157,
   157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
   157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
   157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
   157,   157,   157,   157,   157,   157,   158,   158,   160,   159,
   161,   161,   162,   162,   163,   163,   163,   163,   164,   164,
   164,   164,   165,   165,   165,   165,   165,   165,   165,   165,
   165,   166,   165,   167,   167,   167,   167,   167,   168,   168,
   169,   169,   170,   170,   171,   171,   171,   171,   171,   172,
   172,   172,   173,   173,   173,   173,   174,   174,   174,   174,
   175,   175,   175,   175,   175,   175,   175,   175,   176,   176,
   176,   176,   176,   176,   176,   176,   176,   176,   177,   177,
   177,   177,   177,   177,   177,   178,   178,   178,   178,   178,
   178,   178,   178,   178,   179,   179,   179,   179,   179,   179,
   179,   179,   179,   180,   180,   180,   180,   180,   181,   181,
   183,   182,   185,   184,   186,   186,   187,   187,   188,   188,
   188,   188,   188,   188,   188,   188,   188,   190,   189,   192,
   191,   193,   193,   193,   194,   194,   194,   194,   195,   196,
   196,   196,   198,   197,   197,   199,   199,   200,   200,   200,
   201,   201,   201,   201,   201,   201,   201,   201,   201,   201,
   201,   201,   201,   202,   202,   202,   202,   202,   203,   205,
   204,   206,   206,   207,   207,   207,   207,   207,   207,   207,
   207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
   207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
   207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
   207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
   207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
   207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
   207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
   207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
   207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
   207,   207,   207,   207,   207,   207,   207,   207,   208,   208,
   209,   209,   210,   210,   211,   211,   211,   211,   211,   211,
   211,   211,   211,   211,   211,   211,   211,   211,   211,   211,
   211,   211,   211,   211,   211,   211,   211,   211,   211,   211,
   211,   211,   211,   211,   211,   211,   211,   212,   212,   212,
   212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
   212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
   212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
   212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
   212,   212,   212,   212,   212,   213,   213,   213,   213,   213,
   213,   213,   214,   214,   214,   214,   214,   214,   214,   214,
   214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
   214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
   214,   214,   214,   214,   214,   214,   214,   214,   215,   215,
   215,   215,   215,   215,   216,   216,   216,   217,   217,   218,
   218,   219
};

static const short yyr2[] = {     0,
     2,     2,     2,     1,     2,     2,     1,     3,     4,     0,
     5,     1,     1,     1,     1,     1,     1,     2,     1,     1,
     2,     2,     2,     2,     9,     0,     3,     1,     2,     3,
     2,     2,     2,     2,     2,     2,     2,     2,     1,     3,
     3,     0,     4,     0,     5,     2,     4,     2,     4,     0,
     4,     0,     5,     3,     2,     4,     2,     4,     0,     6,
     0,     6,     0,     7,     0,    10,     0,    11,     0,     8,
     0,     9,     1,     1,     1,     4,     5,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     2,     2,     1,     1,     1,     1,     0,     6,
     1,     1,     1,     3,     2,     4,     3,     4,     1,     1,
     1,     2,     1,     2,     2,     2,     2,     3,     4,     3,
     3,     0,     6,     1,     1,     1,     3,     2,     1,     3,
     1,     2,     1,     2,     1,     3,     5,     3,     4,     1,
     1,     2,     1,     3,     3,     5,     3,     3,     4,     1,
     1,     1,     1,     2,     2,     2,     3,     4,     1,     1,
     1,     2,     2,     2,     3,     4,     7,     3,     1,     2,
     2,     2,     3,     4,     7,     1,     1,     2,     1,     1,
     1,     1,     2,     2,     1,     1,     2,     1,     1,     1,
     1,     2,     2,     1,     1,     2,     2,     2,     2,     3,
     0,     5,     0,     7,     1,     1,     2,     3,     2,     2,
     2,     3,     3,     3,     3,     3,     3,     0,     5,     0,
     6,     1,     1,     2,     1,     3,     3,     5,     1,     1,
     1,     1,     0,     6,     4,     2,     3,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
     2,     2,     2,     1,     2,     1,     3,     2,     1,     0,
     2,     1,     2,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     3,     1,     1,
     1,     1,     1,     3,     1,     4,     7,     7,     4,     2,
     2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     5,     4,     4,     3,     3,     3,     3,     1,     4,     7,
     7,     4,     4,     4,     4,     4,     4,     4,     4,     4,
     4,     4,     4,     2,     5,     2,     2,     2,     2,     2,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     5,     4,
     4,     3,     3,     3,     3,     1,     1,     1,     1,     1,
     1,     1,     1,     4,     7,     7,     4,     2,     5,     2,
     2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     5,     4,     4,     3,     3,     3,     3,     1,     1,
     1,     1,     1,     1,     1,     2,     2,     1,     1,     1,
     2,     0
};

static const short yydefact[] = {     0,
   532,     0,     0,   532,     4,   480,   476,   479,   530,   482,
     0,     0,     0,     0,     0,     0,   478,     0,     0,     0,
     0,     0,     0,     0,     0,   477,     0,     0,     0,     0,
     0,     0,     0,     2,   428,   481,   197,   261,   262,   263,
   250,     0,   276,   249,   274,   269,   265,   266,   264,   268,
   251,     0,   252,   267,   279,   532,     3,   199,   200,   201,
   202,     0,     0,   196,   258,   259,   260,    19,    20,     0,
    26,     0,     0,     0,     0,     0,   119,   532,     0,     5,
    14,     6,    17,     0,    12,    39,    13,    15,    16,    28,
   532,   532,   532,   532,   532,     0,   532,   532,   532,   215,
     0,   444,     0,     0,   214,   532,   532,     0,     0,   532,
   450,   448,   449,   447,   197,   261,   262,   263,   276,   274,
   269,   265,   264,   268,   267,     0,     0,   393,   446,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   531,   532,   278,   275,   265,   271,   265,   270,
   273,   528,   529,   198,   272,     0,   532,   532,   532,   532,
   219,   189,   238,   204,   221,   203,    21,   532,     0,   532,
     0,    24,    23,    22,     0,     0,    18,     0,   256,    73,
   206,    38,     0,    29,    34,    32,    36,     0,    37,    31,
     0,    33,    35,    10,     0,     0,    42,   208,   209,   210,
   211,     0,     0,   205,     0,   392,   391,     0,     0,     0,
     0,     0,     0,     0,     0,   216,   532,   218,   217,     0,
     0,     0,     0,     0,     0,     0,   475,   459,   460,   461,
   462,   463,   464,   467,   468,   474,     0,   456,   457,   458,
   465,   466,   454,   455,   451,   452,   453,   473,   472,     0,
     0,   220,   277,   532,   190,   192,   191,     0,   532,   532,
   240,   532,   532,    27,     0,   532,   253,   532,     0,     0,
    50,   257,    59,    61,    44,    30,     0,   532,   207,   532,
     0,   280,   155,     0,     0,    41,    40,     0,   213,   212,
   432,   436,   434,   435,   442,     0,   441,   433,   438,     0,
   437,   440,   443,     0,   439,   429,   394,     0,   471,   470,
   193,     0,     0,   390,   389,     0,   243,   245,   242,   532,
   532,     0,     0,   226,   225,     0,   255,   532,   130,   131,
     0,     0,     0,     0,   122,   123,   532,   129,   121,    52,
    54,     0,   532,     0,     0,   532,     0,   532,   172,   173,
     0,   532,   532,   532,     0,   171,   532,    63,     0,   532,
     0,     7,     0,   532,     0,     0,     0,     0,     0,     0,
     0,    43,   532,   133,     0,   445,     0,   469,   532,   194,
   239,   244,     0,     0,   222,     0,     0,     0,     0,   227,
   532,     0,     0,   532,   125,   532,   132,   532,     0,   180,
   181,     0,   532,   532,   532,   532,   532,   179,     0,    51,
   532,   523,   519,   522,   532,   145,     0,     0,   521,     0,
     0,     0,   520,     0,     0,     0,     0,     0,     0,   146,
   149,   532,   170,   483,   524,   144,     0,     0,     0,    45,
   532,     0,   532,   174,   176,   175,    69,   532,    11,   532,
   156,   281,   282,   280,     0,   532,   532,     0,   161,   163,
   160,   158,     0,   135,    88,    89,    90,    91,    92,    93,
    96,    97,   112,   100,   101,   102,   103,   104,   105,   106,
   107,   108,   109,   110,   111,   116,   115,    99,    98,    85,
    87,    86,    94,    95,    83,    84,    80,    81,    82,    79,
     0,     0,    78,   134,   137,   136,     0,     0,     0,   142,
   532,     0,    46,   153,     0,     0,     0,   247,   246,   241,
     0,     0,     0,   231,   525,     0,   230,     0,   229,     0,
     0,     0,   532,   228,     0,   254,     0,   532,     0,   117,
   120,   118,   124,   532,   182,   184,   183,     0,     0,   532,
   127,   151,    53,   532,     0,    55,   532,   532,     0,   488,
     0,     0,   493,   492,   491,     0,     0,   490,   532,   148,
     0,   532,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   532,     0,
    48,    71,   177,   532,     0,     0,   284,   285,   287,   286,
   291,   288,   289,   290,   292,   293,   294,   295,   296,   297,
   298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
   308,   309,   310,   311,   312,   313,   314,   315,   316,   364,
   317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
   327,   328,   329,   344,   330,   331,   332,   333,   334,   335,
   336,   337,   338,   339,   340,   365,   341,   342,   343,   345,
   346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
   356,   357,   358,   359,   360,   361,   362,   363,   532,   382,
   383,   384,   375,   387,   371,   372,   370,   377,   378,   366,
   367,   368,   369,   376,   374,   381,   379,   385,   380,   373,
   386,   283,     0,     8,     0,     0,   159,   162,   138,   114,
   113,   141,   140,   154,   532,     0,     0,   431,   430,   532,
     0,   527,   237,   526,   236,   235,   234,   233,   232,   224,
     0,   126,   128,   185,   188,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   152,   395,     0,     0,    57,     0,
     0,   168,   532,     0,   532,     0,     0,     0,   518,    75,
    60,    74,   147,   150,   167,   502,   503,   504,   505,   506,
   507,   510,   511,   517,     0,   499,   500,   501,   508,   509,
   497,   498,   494,   495,   496,   516,   515,     0,     0,     0,
    65,    62,     0,   532,     0,   178,   532,     0,   157,     9,
   164,   532,   165,     0,   139,    47,   195,   248,     0,   532,
     0,     0,     0,   404,   402,   403,   401,     0,     0,   400,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   186,     0,    56,   154,   169,     0,     0,   487,     0,
   484,     0,     0,   514,   513,    67,   532,    49,     0,   532,
    64,   388,     0,   532,    25,     0,     0,     0,     0,     0,
   427,   413,   414,   415,   416,   417,   418,   419,   420,   426,
     0,   410,   411,   412,   408,   409,   405,   406,   407,   425,
   424,     0,     0,    58,     0,   489,     0,     0,   512,   532,
     0,   532,    70,   166,   143,   532,     0,   399,     0,   396,
     0,   423,   422,     0,     0,    76,     0,     0,   532,    72,
   187,     0,     0,   421,   486,   485,    77,   532,    66,     0,
     0,    68,   398,   397,     0,     0,     0
};

static const short yydefgoto[] = {   955,
     4,   371,   372,    81,   290,    82,    83,   178,    84,    85,
    86,   298,   357,   382,   450,   187,   352,   419,   420,   563,
   206,   353,   356,   460,   877,   920,   614,   814,   781,   514,
   551,    87,   185,   344,   345,   346,   347,   383,   735,   439,
   440,   561,   522,   296,   468,   469,   441,   365,   417,   171,
    56,   207,   102,    57,    58,   272,    59,   273,   333,   334,
   400,    60,   270,    61,   330,   326,   327,    62,    63,    88,
   338,    89,    64,    65,    66,    67,   369,   370,   462,   722,
   323,   215,   216,   765,   128,    35,   443,   444,   534,   384,
    36,   217
};

static const short yypact[] = {   160,
-32768,  2058,  3017,  2929,-32768,-32768,-32768,-32768,-32768,-32768,
   -85,   -80,   -60,   -46,   -36,   -29,-32768,   -33,   -16,    -6,
  3119,     2,    35,    41,   -11,-32768,    43,  2058,  2058,  2058,
  2058,  1338,  2058,  2488,-32768,   178,-32768,-32768,-32768,-32768,
-32768,  3169,-32768,-32768,-32768,-32768,  1902,-32768,   551,   551,
-32768,   163,-32768,   551,-32768,    23,-32768,-32768,-32768,-32768,
-32768,    26,    30,-32768,-32768,-32768,-32768,-32768,-32768,    45,
-32768,    55,    78,    80,    83,    86,-32768,-32768,    36,-32768,
-32768,-32768,-32768,  2269,-32768,-32768,-32768,-32768,-32768,-32768,
  2058,  2058,  2058,  2058,  2058,  3017,  2058,  2058,  2058,-32768,
   163,    77,   163,   163,-32768,  2058,  2058,  3017,  3017,  2058,
   114,   114,   114,   114,   -85,   -80,   -60,   -46,   -36,   -33,
   -16,  1728,  1334,  1524,  1815,    99,   -73,  2488,   114,  2058,
  2058,  2058,  2058,  2058,  2058,  2058,  2058,  2058,  2058,  2058,
  2058,  2058,  2058,  2058,  2058,  2058,  2058,  2058,  2058,  2058,
   679,  2058,-32768,    23,-32768,-32768,  1989,-32768,   551,-32768,
-32768,-32768,-32768,-32768,-32768,   108,    23,    23,    23,    23,
   128,-32768,-32768,   152,-32768,    44,-32768,  2659,   163,-32768,
   161,-32768,-32768,-32768,   164,  2325,-32768,   163,-32768,   182,
   187,-32768,  3169,   311,-32768,-32768,-32768,  3069,-32768,-32768,
   163,-32768,-32768,-32768,   163,   185,   217,-32768,-32768,-32768,
-32768,    26,    30,-32768,   208,   230,-32768,   213,   222,   224,
   225,   240,   236,   242,   244,-32768,  2058,-32768,-32768,   247,
   249,   251,   262,   255,  2058,  2058,-32768,  2648,  2679,   737,
   737,   372,   372,   396,   396,-32768,  1356,  2694,  2709,   587,
   372,   372,   136,   136,   114,   114,   114,-32768,-32768,   -53,
   888,   128,-32768,    23,   128,   128,   128,   125,  2058,   163,
-32768,-32768,   273,-32768,   279,  3196,-32768,  2863,  3169,   185,
-32768,-32768,-32768,    15,-32768,   178,  3169,   127,-32768,-32768,
   265,-32768,-32768,   163,  1623,-32768,-32768,    38,   152,    44,
-32768,-32768,-32768,-32768,-32768,   283,-32768,-32768,-32768,   261,
-32768,-32768,-32768,   285,-32768,   114,  2488,  2058,-32768,-32768,
   128,   287,   292,  2488,-32768,   297,   305,   304,-32768,   163,
  3221,   -18,   325,   326,-32768,   163,-32768,-32768,-32768,-32768,
   163,  2979,   163,   316,   328,-32768,    59,-32768,-32768,-32768,
-32768,    38,  1433,   318,    38,  1433,    38,   127,-32768,-32768,
   319,   127,   127,   127,   118,-32768,  3246,-32768,   313,-32768,
   170,-32768,   321,  1710,  1546,   324,    38,  2191,    38,    38,
    38,-32768,   204,-32768,  2058,-32768,  2058,  2488,  1433,-32768,
-32768,   163,  2058,   320,-32768,   220,   469,   774,   263,-32768,
-32768,   -18,   342,  3271,   340,    59,-32768,   362,  2863,-32768,
-32768,   335,    59,    59,    59,    59,   -14,-32768,    38,-32768,
   204,-32768,-32768,-32768,    59,-32768,  3169,   341,-32768,  3119,
   333,   344,-32768,  2058,  2058,  2058,  1338,  2058,   337,    10,
-32768,    59,  2505,-32768,   178,-32768,    38,   176,   338,-32768,
   204,   157,   127,   336,   336,   336,-32768,  2058,-32768,  1433,
-32768,  1211,-32768,-32768,   163,  2058,  1710,   346,   367,  2488,
-32768,-32768,    38,   162,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
   349,   350,-32768,-32768,   162,   162,   179,   117,  1623,-32768,
  2058,   369,-32768,-32768,   -50,   -47,   359,   376,  2488,-32768,
   163,   776,   163,-32768,-32768,   776,-32768,   776,-32768,   776,
   776,   776,  3296,-32768,   163,-32768,  3017,   -14,  2381,-32768,
-32768,-32768,-32768,    59,   374,   374,   374,   197,   716,  2058,
-32768,-32768,-32768,   204,   395,-32768,    39,    59,  3017,   379,
  3017,  3017,   114,   114,   114,   383,   -41,   114,   451,-32768,
  1528,    39,  2058,  2058,  2058,  2058,  2058,  2058,  2058,  2058,
  2058,  2058,  2058,  2058,  2058,  2058,  2058,  2058,  2058,  2058,
  2058,  2058,  2058,  1797,  2058,   221,   504,   388,   451,   411,
-32768,-32768,   336,  1433,   398,   399,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,   397,-32768,   405,   406,-32768,  1884,   162,-32768,
-32768,-32768,-32768,  1546,  1433,   408,    38,-32768,-32768,   451,
  2058,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
   409,-32768,-32768,   374,   410,   428,   420,   437,   716,   716,
   716,   716,  1338,   716,  2539,-32768,   424,   448,-32768,    38,
  2058,-32768,    39,   441,  2058,   433,   455,  2058,-32768,-32768,
   476,-32768,-32768,-32768,-32768,  2648,  2679,   737,   737,   372,
   372,   396,   396,-32768,  2467,  2694,  2709,   587,   372,   372,
   136,   136,   114,   114,   114,-32768,-32768,   -32,  2141,   449,
-32768,   476,    38,  1433,   446,-32768,   451,  1087,-32768,-32768,
-32768,  1710,  2488,   450,-32768,-32768,   476,  2488,   466,  1433,
  3017,  3017,  3017,   145,   145,   145,   145,   452,   -20,   145,
   716,   716,   716,   716,   716,   716,   716,   716,   716,   716,
   716,   716,   716,   716,   716,   716,   716,   716,   716,  1971,
  2058,-32768,    38,-32768,  2488,-32768,   453,   456,-32768,   463,
   114,   464,  2058,-32768,-32768,-32768,  1433,-32768,   468,   451,
   476,-32768,   462,   451,-32768,   471,   459,   472,   479,   716,
-32768,  2664,  2724,   932,   932,  1615,  1615,   522,   522,-32768,
  2522,  2832,  2847,  2862,   347,   347,   145,   145,   145,-32768,
-32768,     7,  2171,-32768,  2058,-32768,  2058,    19,  2488,  1433,
   473,   451,   476,-32768,   476,   451,   477,-32768,   488,   145,
   716,-32768,-32768,    34,   121,-32768,   489,   491,   451,   476,
   476,  2058,  2058,  2539,-32768,-32768,-32768,   451,   476,   122,
   123,   476,-32768,-32768,   589,   611,-32768
};

static const short yypgoto[] = {-32768,
  -150,-32768,   150,-32768,-32768,   439,-32768,-32768,   -59,   210,
-32768,-32768,-32768,  -115,  -189,-32768,-32768,-32768,  -145,  -237,
   442,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  -503,-32768,
-32768,   223,-32768,-32768,-32768,   232,   288,  -315,-32768,  -333,
-32768,    98,  -419,  -231,  -466,-32768,    66,  -268,  -183,   126,
   -38,   465,   218,    11,   -74,-32768,   -70,-32768,-32768,-32768,
   248,   -69,-32768,   -58,-32768,   343,-32768,    -4,    -3,-32768,
-32768,-32768,     8,   274,-32768,-32768,   211,-32768,   -23,-32768,
  -442,   -86,   -27,   184,    69,   134,-32768,-32768,  -345,   422,
   -21,    -1
};


#define	YYLAST		3420


static const short yytable[] = {     5,
   726,   565,    90,   154,   127,   218,   219,   220,   221,   208,
   223,   224,   225,   209,   210,   615,   103,   104,   186,   230,
   231,   580,   449,   234,   354,   211,   162,   163,   105,   276,
   236,   610,   166,   162,   163,    91,   421,   162,   163,   448,
    92,   451,   126,   162,   163,   162,   163,   376,   351,   237,
   236,   537,   539,   236,   172,   527,   236,   396,   397,   398,
    93,   474,   236,   515,   516,   517,   410,   411,   412,   319,
    34,   236,   738,   167,    94,   739,    90,   399,   736,   212,
   213,   779,    96,   236,    95,   162,   163,    97,   377,   452,
   874,   214,   559,   454,   455,   456,   111,   112,   113,   114,
   109,   129,   891,   564,    98,   812,   222,   560,   188,   413,
   236,   208,   378,   581,    99,   209,   210,   767,   232,   233,
   733,   331,   106,   260,   162,   163,   616,   211,   173,   932,
   355,   606,   175,   168,   359,   360,   361,   236,   169,   367,
   310,   936,   138,   170,   768,   771,  -223,   772,   379,  -223,
   177,   523,   172,   380,   285,   107,   945,   729,   381,   288,
   560,   108,   785,   110,   138,   172,   172,   172,   172,   414,
   162,   163,   286,   849,   415,   179,    90,   362,     5,   416,
   180,   212,   213,   153,   613,   182,   743,   404,   183,   566,
   745,   184,   746,   214,   747,   748,   749,   227,   238,   239,
   240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
   250,   251,   252,   253,   254,   255,   256,   257,   258,   611,
   261,   235,   548,   264,   236,   236,   236,   162,   163,   555,
   556,   557,   558,   150,   151,   152,   827,   363,   457,   458,
   350,   567,   364,   946,   953,   954,   269,   322,   358,   269,
   543,   147,   148,   149,   271,   150,   151,   152,   582,     1,
     2,     3,   172,   277,   859,   860,   861,   325,   329,    41,
     5,   335,   464,   465,    90,   278,   349,   612,   458,   262,
   815,   607,   520,   521,   607,   348,   366,   292,     5,   293,
   294,   295,   265,   266,   267,   268,   520,   521,   608,   520,
   521,   732,   283,   316,   317,    51,   292,   284,   293,   518,
   519,   531,    53,   881,   442,   532,     9,   442,   560,   755,
   158,   297,   160,   161,   520,   521,   607,   165,   329,    90,
   301,   445,   769,   236,   445,   302,     5,   324,   540,   541,
   542,   520,   521,   810,   303,   418,   304,   305,   549,   348,
   442,   446,   306,   866,   446,   883,   366,   525,   307,   526,
   366,   366,   366,   375,   308,    90,   309,   445,   463,   311,
   754,   312,   471,   313,   314,   849,   923,   315,   332,   725,
   925,   524,   336,   386,   773,   368,   388,   446,   568,   321,
   136,   137,   533,   533,   533,   158,   160,   161,   165,     5,
   138,   824,    90,   385,   418,   387,    90,   389,   392,   577,
   393,   418,   418,   418,   418,   562,   348,   390,   940,   524,
   391,   442,   941,   418,   138,   103,   104,   401,   408,   402,
   158,   409,   158,   447,   453,   949,   461,   105,   445,   473,
   418,   466,   470,   530,   952,   545,   547,   576,    77,   524,
   554,   366,   569,   571,   421,   572,   325,   458,   446,   579,
   609,   529,   856,   857,   858,   471,   859,   860,   861,   727,
   728,   730,   737,   164,   208,   731,   162,   163,   209,   210,
   879,   740,   741,   174,   176,   145,   146,   147,   148,   149,
   211,   150,   151,   152,   181,   560,   886,   451,   770,   775,
   189,   780,   573,   574,   575,   778,   578,   733,   811,   145,
   146,   147,   148,   149,   813,   150,   151,   152,    41,   325,
   819,   817,   226,   816,   228,   229,   324,   820,   533,   821,
   830,   829,   533,   825,   533,   470,   533,   533,   533,   831,
   832,    90,   442,   921,   212,   213,   562,   564,   833,   862,
   849,   863,   418,   867,    51,   869,   214,   752,   325,   445,
   531,    53,   524,   872,   536,   524,   418,   870,   880,   876,
   885,   927,   884,   915,   890,   442,   808,   782,   916,   774,
   524,   776,   777,   917,   918,   924,   938,   734,   956,   324,
   922,   929,   445,   926,   928,   939,    38,   942,    39,    40,
   275,   132,   133,   134,   135,   136,   137,   782,   943,   282,
   957,   947,   446,   948,   724,   138,   274,   550,    46,   159,
    48,   826,   289,   878,   864,   914,   291,   280,   324,   406,
   552,    49,    50,   299,   300,   854,   855,   856,   857,   858,
   553,   859,   860,   861,    54,   753,   784,   570,     0,   544,
   281,   786,   787,   788,   789,   790,   791,   792,   793,   794,
   795,   796,   797,   798,   799,   800,   801,   802,   803,   804,
   805,   806,   394,   809,   723,   818,     0,     0,     0,     0,
     0,     6,     7,     8,     9,     0,    10,    11,   868,     0,
     0,   328,   766,     0,     0,     0,   442,   463,   143,   144,
   145,   146,   147,   148,   149,     0,   150,   151,   152,     0,
     0,     0,     0,   445,     0,   373,     0,     0,     6,     7,
     8,     9,     0,    10,    12,     0,    13,    14,     0,     0,
     0,    15,    16,   446,     0,   839,     0,     0,   782,    17,
    18,     0,     0,     0,     0,     0,    19,    20,     0,     0,
     0,   328,    21,   134,   135,   136,   137,   403,     0,    22,
    23,    24,   405,    25,   407,   138,     0,    26,     0,   756,
     0,   524,    27,   838,     0,   442,    17,     0,     0,     0,
     0,   162,   163,   162,   163,     0,     0,     0,     0,    28,
     0,   442,   445,    29,    30,     0,   823,    31,   757,    32,
   758,   259,     0,    33,    26,     0,     0,     0,   445,   828,
     0,     0,   446,   528,     0,   782,     0,   535,   535,   535,
   471,     0,     0,    41,     0,    41,   759,     0,   446,     0,
   760,   761,   912,     0,   762,     0,   763,     0,   442,   865,
   764,   887,   888,   889,     0,     0,   871,     0,   143,   144,
   145,   146,   147,   148,   149,   445,   150,   151,   152,    51,
     0,    51,     0,     0,     0,   531,    53,   531,    53,   538,
     0,     0,     0,     0,     0,   446,     0,     0,   782,     0,
     0,   442,   782,     0,     0,     0,   373,   934,     0,   935,
   470,     0,   766,   766,   766,   766,     0,   766,   445,     0,
   130,   131,   132,   133,   134,   135,   136,   137,     0,     0,
     0,     0,     0,     0,   950,   951,   138,     0,   446,     0,
   782,     0,     0,     0,   782,     0,     0,     0,     0,   913,
     0,     0,     0,     0,     0,     0,     0,   782,     0,   373,
     0,   919,   834,   835,   836,   837,   782,   840,   845,   846,
   847,   848,   742,   535,   744,     0,     0,   535,     0,   535,
   849,   535,   535,   535,     0,     0,   751,     0,     0,     0,
     0,     0,     0,     0,   766,   766,   766,   766,   766,   766,
   766,   766,   766,   766,   766,   766,   766,   766,   766,   766,
   766,   766,   766,     0,     0,   139,   140,   141,   142,   143,
   144,   145,   146,   147,   148,   149,     0,   150,   151,   152,
     0,     0,     0,   320,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   766,   892,   893,   894,   895,   896,   897,
   898,   899,   900,   901,   902,   903,   904,   905,   906,   907,
   908,   909,   910,     0,     0,   854,   855,   856,   857,   858,
     0,   859,   860,   861,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   766,     0,     0,     0,     0,     0,
     0,     0,     0,   930,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   617,
   618,   619,   620,   621,   622,   623,   624,     0,   625,   626,
   627,   628,   629,   630,   631,   632,   633,   634,   635,     0,
     0,     0,     0,     0,   944,   636,   637,   638,   639,   640,
   641,   642,   643,   644,   645,   646,   647,   648,   649,   650,
     0,     0,   651,   652,   653,   654,   655,   656,   657,   658,
   659,   660,     0,   661,   662,   663,   664,   665,   666,   667,
   668,   669,   670,   671,   672,   673,     0,     0,   674,     0,
   675,   676,   677,   678,   679,   680,   681,   682,   683,   684,
   685,   686,   687,     0,   688,   689,   690,   691,   692,   693,
   694,     0,   695,   696,   697,   698,     0,     0,     0,   699,
   700,   701,   702,   703,   704,   705,   706,   707,   708,   709,
   710,   711,   712,   713,   714,   715,   716,   717,   718,   719,
   882,   720,   721,   617,   618,   619,   620,   621,   622,   623,
   624,     0,   625,   626,   627,   628,   629,   630,   631,   632,
   633,   634,   635,     0,     0,     0,     0,     0,     0,   636,
   637,   638,   639,   640,   641,   642,   643,   644,   645,   646,
   647,   648,   649,   650,     0,     0,   651,   652,   653,   654,
   655,   656,   657,   658,   659,   660,     0,   661,   662,   663,
   664,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     0,     0,   674,     0,   675,   676,   677,   678,   679,   680,
   681,   682,   683,   684,   685,   686,   687,     0,   688,   689,
   690,   691,   692,   693,   694,     0,   695,   696,   697,   698,
     0,     0,     0,   699,   700,   701,   702,   703,   704,   705,
   706,   707,   708,   709,   710,   711,   712,   713,   714,   715,
   716,   717,   718,   719,     0,   720,   721,     0,     0,   937,
     6,     7,     8,     9,     0,    10,   115,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   130,   131,
   132,   133,   134,   135,   136,   137,     0,     0,     0,    38,
     0,    39,    40,   116,   138,   117,   118,    41,    42,     0,
   119,    16,     0,     0,    44,     0,     0,     0,    17,   120,
     0,    46,   159,    48,     0,   121,   122,    48,     0,     0,
     0,    21,     0,     0,    49,    50,     0,     0,   123,   124,
    24,     0,    25,    51,     0,     0,    26,    54,     0,    52,
    53,   125,     0,     0,    55,   422,   423,   424,     9,     0,
   425,    37,     0,     0,   426,     0,     0,     0,    28,     0,
     0,     0,    29,    30,   106,     0,    31,     0,    32,     0,
     0,   318,    33,   139,   140,   141,   142,   143,   144,   145,
   146,   147,   148,   149,     0,   150,   151,   152,    38,     0,
    39,    40,    41,   427,     0,    43,   428,     0,     0,    44,
     0,     0,     0,   429,    45,     0,     0,     0,     0,     0,
    46,    47,    48,     0,     0,     0,   430,     0,     0,     0,
     0,     0,     0,    49,    50,   431,     0,   432,    51,     0,
     0,   433,     0,     0,    52,    53,    54,     0,     0,    55,
   422,   423,   424,     9,     0,   425,    37,     0,     0,   783,
     0,     0,     0,   434,     0,     0,     0,   435,     0,     0,
     0,   436,     0,   437,     0,     0,     0,   438,   130,   131,
   132,   133,   134,   135,   136,   137,     0,     0,     0,    38,
     0,    39,    40,    38,   138,    39,    40,    41,   427,     0,
    43,   428,     0,     0,    44,     0,     0,     0,   429,    45,
     0,    46,   159,    48,     0,    46,    47,    48,     0,     0,
     0,   430,     0,     0,    49,    50,     0,     0,    49,    50,
   431,     0,   432,    51,     0,     0,   433,    54,     0,    52,
    53,    54,     0,     0,    55,     6,     7,     8,     9,     0,
    10,    11,     0,   847,   848,     0,     0,     0,   434,     0,
     0,     0,   435,   849,   107,     0,   436,     0,   437,     0,
   472,     0,   438,   139,   140,   141,   142,   143,   144,   145,
   146,   147,   148,   149,     0,   150,   151,   152,    12,     0,
    13,    14,     0,     0,     0,    15,    16,     0,     0,     0,
     0,     0,     0,    17,    18,     0,     0,     0,     0,     0,
    19,    20,     0,     0,     0,     0,    21,     0,     0,     0,
     0,     0,     0,    22,    23,    24,     0,    25,     0,     0,
     0,    26,     6,     7,     8,     9,    27,    10,    11,     0,
     0,     0,     0,     0,     0,   374,     0,     0,   854,   855,
   856,   857,   858,    28,   859,   860,   861,    29,    30,     0,
     0,    31,     0,    32,     0,     0,     0,    33,     0,     0,
     0,     0,     0,     0,     0,    12,     0,    13,    14,     0,
     0,     0,    15,    16,     0,     0,     0,     0,     0,     0,
    17,    18,     0,    38,     0,    39,    40,    19,    20,     0,
   155,     0,     0,    21,     0,     0,     0,     0,     0,   156,
    22,    23,    24,     0,    25,    46,   157,    48,    26,     6,
     7,     8,     9,    27,    10,    11,     0,     0,    49,    50,
     0,     0,   467,     0,     0,     0,     0,     0,     0,     0,
    28,    54,     0,     0,    29,    30,     0,     0,    31,     0,
    32,     0,     0,     0,    33,     0,     0,     0,     0,     0,
     0,     0,    12,     0,    13,    14,     0,     0,    99,    15,
    16,     0,     0,     0,     0,     0,     0,    17,    18,     0,
    38,     0,    39,    40,    19,    20,     0,     0,     0,     0,
    21,     0,     0,     0,     0,     0,     0,    22,    23,    24,
     0,    25,    46,   159,    48,    26,     6,     7,     8,     9,
    27,    10,    11,     0,     0,    49,    50,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    28,    54,     0,
     0,    29,    30,     0,     0,    31,     0,    32,     0,   807,
     0,    33,     0,     0,     0,     0,     0,     0,     0,    12,
     0,    13,    14,     0,     0,   110,    15,    16,     0,     0,
     0,     0,     0,     0,    17,    18,     0,    38,     0,    39,
    40,    19,    20,     0,   155,     0,     0,    21,     0,     0,
     0,     0,     0,   156,    22,    23,    24,     0,    25,    46,
   157,    48,    26,     6,     7,     8,     9,    27,    10,    11,
     0,     0,    49,    50,     0,     0,   822,     0,     0,     0,
     0,     0,     0,     0,    28,    54,     0,     0,    29,    30,
     0,     0,    31,     0,    32,     0,     0,     0,    33,     0,
     0,     0,     0,     0,     0,     0,    12,     0,    13,    14,
     0,     0,     0,    15,    16,     0,     0,     0,     0,     0,
     0,    17,    18,     0,    38,     0,    39,    40,    19,    20,
     0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
   263,    22,    23,    24,     0,    25,    46,   159,    48,    26,
     6,     7,     8,     9,    27,    10,    11,     0,     0,    49,
    50,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    28,    54,     0,     0,    29,    30,     0,     0,    31,
     0,    32,     0,   911,     0,    33,     0,     0,     0,     0,
     0,     0,     0,    12,     0,    13,    14,     0,     0,     0,
    15,    16,     0,     0,     0,     0,     0,     0,    17,    18,
     0,     0,     0,     0,     0,    19,    20,     0,     0,     0,
     0,    21,     0,     0,     0,     0,     0,     0,    22,    23,
    24,     0,    25,     0,     0,     0,    26,     0,     0,     0,
     0,    27,     0,   130,   131,   132,   133,   134,   135,   136,
   137,     0,     0,     0,     0,     0,     0,     0,    28,   138,
     0,     0,    29,    30,     0,     0,    31,     0,    32,     0,
     0,     0,    33,   130,   131,   132,   133,   134,   135,   136,
   137,     0,     0,     0,     0,     0,     0,     0,     0,   138,
     0,     0,     0,   475,   476,   477,   478,   479,   480,   481,
   482,     0,     0,     0,     0,     0,     0,     0,     0,   483,
     0,   484,   485,   486,   487,   488,   489,   490,   491,   492,
   493,   494,   495,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   496,     0,     0,     0,     0,     0,   139,   140,
   141,   142,   143,   144,   145,   146,   147,   148,   149,     0,
   150,   151,   152,     0,   497,     0,   875,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   190,   191,   139,   140,
   141,   142,   143,   144,   145,   146,   147,   148,   149,     0,
   150,   151,   152,     0,   498,     0,   933,   499,     0,   500,
   501,   502,   503,   504,   505,   506,   507,   508,   509,   510,
     0,   511,   512,   192,    38,   513,    39,    40,    41,   193,
     0,    43,     0,     0,     0,    44,   194,   195,     0,     0,
    45,     0,   190,   191,     0,   196,    46,    47,    48,     0,
   197,     0,     0,   198,     0,     0,     0,   199,     0,    49,
    50,     0,   200,     0,    51,     0,     0,     0,     0,     0,
   201,    53,    54,     0,   202,    55,   203,     0,     0,   192,
    38,   204,    39,    40,    41,   279,     0,    43,     0,     0,
     0,    44,   194,   195,     0,     0,    45,   205,   190,   191,
     0,   196,    46,    47,    48,     0,   197,     0,     0,   198,
     0,     0,     0,   199,     0,    49,    50,     0,   200,     0,
    51,     0,     0,     0,     0,     0,   201,    53,    54,     0,
   202,    55,   203,     0,     0,   192,    38,     0,    39,    40,
    41,   193,     0,    43,     0,     0,     0,    44,   194,   195,
     0,     0,    45,   205,     0,     0,     0,   196,    46,    47,
    48,     0,   197,     0,     0,   198,     0,     0,     0,   199,
     0,    49,    50,     0,   200,     0,    51,     0,     0,     0,
     0,     0,   201,    53,    54,     0,   202,    55,   203,   130,
   131,   132,   133,   134,   135,   136,   137,     0,     0,     0,
     0,     0,     0,     0,     0,   138,     0,     0,     0,   205,
   130,   131,   132,   133,   134,   135,   136,   137,     0,     0,
     0,     0,     0,     0,     0,     0,   138,   583,   584,   585,
   586,   587,   588,   589,   590,     0,     0,     0,     0,     0,
     0,     0,     0,   591,   841,   842,   843,   844,   845,   846,
   847,   848,     0,     0,     0,     0,     0,     0,     0,     0,
   849,   841,   842,   843,   844,   845,   846,   847,   848,     0,
     0,     0,     0,     0,     0,     0,     0,   849,     0,     0,
     0,     0,   873,     0,   139,   140,   141,   142,   143,   144,
   145,   146,   147,   148,   149,     0,   150,   151,   152,     0,
     0,     0,     0,     0,     0,   139,   140,   141,   142,   143,
   144,   145,   146,   147,   148,   149,     0,   150,   151,   152,
     0,     0,   592,   593,   594,   595,   596,   597,   598,   599,
   600,   601,   602,     0,   603,   604,   605,   931,     0,   850,
   851,   852,   853,     0,     0,   854,   855,   856,   857,   858,
     0,   859,   860,   861,     0,     0,   850,   851,   852,   853,
     0,     0,   854,   855,   856,   857,   858,     0,   859,   860,
   861,   131,   132,   133,   134,   135,   136,   137,     0,     0,
     0,     0,     0,     0,     0,     0,   138,   842,   843,   844,
   845,   846,   847,   848,     0,     0,     0,     0,     0,     0,
     0,     0,   849,   132,   133,   134,   135,   136,   137,     0,
     0,     0,    68,     0,     0,     0,     0,   138,   132,   133,
   134,   135,   136,   137,    69,     0,     0,     0,    70,     0,
     0,    71,   138,   132,   133,   134,   135,   136,   137,    72,
     0,    73,     0,     0,    74,    75,    76,   138,   843,   844,
   845,   846,   847,   848,     0,    77,     0,     0,     0,    78,
     0,     0,   849,    79,     0,     0,   140,   141,   142,   143,
   144,   145,   146,   147,   148,   149,     0,   150,   151,   152,
     0,     0,   851,   852,   853,     0,     0,   854,   855,   856,
   857,   858,     0,   859,   860,   861,     0,   140,   141,   142,
   143,   144,   145,   146,   147,   148,   149,     0,   150,   151,
   152,     0,     0,   141,   142,   143,   144,   145,   146,   147,
   148,   149,     0,   150,   151,   152,     0,     0,     0,   142,
   143,   144,   145,   146,   147,   148,   149,     0,   150,   151,
   152,     0,   851,   852,   853,     0,     0,   854,   855,   856,
   857,   858,     0,   859,   860,   861,   843,   844,   845,   846,
   847,   848,     0,     0,     0,     0,     0,     0,     0,     0,
   849,   843,   844,   845,   846,   847,   848,     0,     0,     0,
   339,   340,     0,     0,     0,   849,   843,   844,   845,   846,
   847,   848,     0,     0,     0,     0,     0,     0,     0,     0,
   849,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    38,     0,
    39,    40,   341,   342,     0,    43,     0,     0,     0,     0,
     0,     0,     0,     0,    45,     0,     0,     0,    -1,     0,
    46,    47,    48,     0,     0,     0,     0,     0,     0,     0,
     0,   852,   853,    49,    50,   854,   855,   856,   857,   858,
     0,   859,   860,   861,   343,     0,    54,   853,     0,    55,
   854,   855,   856,   857,   858,     0,   859,   860,   861,     0,
     0,     0,    68,     0,     0,   854,   855,   856,   857,   858,
     0,   859,   860,   861,    69,     0,   339,   340,    70,     0,
     0,    71,     0,     0,     0,     0,     0,     0,     0,    72,
     0,    73,     0,     0,    74,    75,    76,     0,     0,     0,
     0,     0,     0,     0,     0,    77,     0,     0,     0,    78,
     0,     0,     0,    79,    38,    37,    39,    40,     0,     0,
     0,    43,     0,    80,     0,     0,     0,     0,     0,     0,
    45,     0,     0,     0,     0,     0,    46,    47,    48,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    49,
    50,     0,    38,     0,    39,    40,    41,    42,     0,    43,
   343,     0,    54,    44,     0,    55,     0,    37,    45,     0,
     0,     0,     0,     0,    46,    47,    48,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    49,    50,     0,
     0,     0,    51,     0,     0,     0,     0,     0,    52,    53,
    54,     0,     0,    55,    38,     0,    39,    40,    41,   287,
     0,    43,     0,     0,     0,    44,     0,   100,     0,     0,
    45,     0,     0,     0,     0,     0,    46,    47,    48,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    49,
    50,     0,     0,     0,    51,     0,     0,     0,     0,     0,
    52,    53,    54,     0,    38,    55,    39,    40,    41,     0,
     0,    43,     0,     0,     0,    44,     0,    37,     0,     0,
    45,     0,     0,     0,     0,     0,    46,    47,    48,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    49,
    50,     0,     0,     0,    51,     0,     0,     0,     0,     0,
   101,    53,    54,     0,    38,    55,    39,    40,    41,     0,
     0,    43,     0,     0,     0,    44,     0,     0,     0,     0,
    45,     0,     0,     0,     0,     0,    46,    47,    48,    68,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    49,
    50,    69,     0,     0,    51,    70,     0,     0,    71,     0,
    52,    53,    54,     0,    68,    55,    72,     0,    73,     0,
     0,    74,    75,    76,     0,     0,    69,     0,     0,     0,
    70,     0,    77,    71,     0,     0,    78,     0,     0,    68,
    79,    72,     0,    73,     0,     0,    74,    75,    76,     0,
    80,    69,     0,     0,     0,    70,     0,    77,    71,     0,
     0,    78,     0,     0,    68,    79,    72,     0,    73,   337,
     0,    74,    75,    76,     0,    80,    69,     0,     0,     0,
    70,     0,    77,    71,     0,     0,    78,     0,     0,    68,
    79,    72,     0,    73,   395,     0,    74,    75,    76,     0,
    80,    69,     0,     0,     0,    70,     0,    77,    71,     0,
     0,    78,     0,     0,     0,    79,    72,     0,    73,   459,
     0,    74,    75,    76,     0,    80,     0,     0,     0,     0,
     0,     0,    77,     0,     0,     0,    78,     0,     0,     0,
    79,     0,     0,     0,   546,     0,     0,     0,     0,     0,
    80,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   750
};

static const short yycheck[] = {     1,
   467,   421,     4,    42,    32,    92,    93,    94,    95,    84,
    97,    98,    99,    84,    84,   458,    21,    21,    78,   106,
   107,    12,   356,   110,    10,    84,     8,     9,    21,   180,
   104,   451,    10,     8,     9,   121,   352,     8,     9,   355,
   121,   357,    32,     8,     9,     8,     9,    10,   280,   123,
   104,   397,   398,   104,    56,   389,   104,    76,    77,    78,
   121,   377,   104,   379,   380,   381,     8,     9,    10,   123,
     2,   104,   123,    51,   121,   123,    78,    96,   521,    84,
    84,   123,   112,   104,   121,     8,     9,   121,    51,   358,
   123,    84,   107,   362,   363,   364,    28,    29,    30,    31,
   112,    33,   123,   419,   121,   609,    96,   122,    73,    51,
   104,   186,    75,   104,   121,   186,   186,   560,   108,   109,
     4,   272,   121,   151,     8,     9,   460,   186,   103,   123,
   116,   447,   103,   111,     8,     9,    10,   104,   116,   290,
   227,   123,    29,   121,   564,   107,   103,   567,   111,   106,
   106,   383,   154,   116,   193,   121,   123,   473,   121,   198,
   122,   121,   582,   121,    29,   167,   168,   169,   170,   111,
     8,     9,   194,    29,   116,   121,   178,    51,   180,   121,
   103,   186,   186,     6,   453,   106,   532,   338,   106,   421,
   536,   106,   538,   186,   540,   541,   542,   121,   130,   131,
   132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
   142,   143,   144,   145,   146,   147,   148,   149,   150,   451,
   152,   123,   406,   116,   104,   104,   104,     8,     9,   413,
   414,   415,   416,   120,   121,   122,   740,   111,   121,   122,
   279,   425,   116,   123,   123,   123,   122,   123,   287,   122,
   401,   116,   117,   118,   103,   120,   121,   122,   442,   100,
   101,   102,   264,   103,   120,   121,   122,   269,   270,    50,
   272,   273,   103,   104,   276,   112,   278,   121,   122,   154,
   614,   106,   121,   122,   106,   278,   288,   103,   290,   105,
   106,   107,   167,   168,   169,   170,   121,   122,   123,   121,
   122,   123,   121,   235,   236,    86,   103,   121,   105,   106,
   107,    92,    93,   817,   353,    96,     6,   356,   122,   123,
    47,   105,    49,    50,   121,   122,   106,    54,   330,   331,
   123,   353,   564,   104,   356,   123,   338,   269,    76,    77,
    78,   121,   122,   123,   123,   347,   123,   123,   408,   342,
   389,   353,   113,   773,   356,   822,   358,   385,   123,   387,
   362,   363,   364,   295,   123,   367,   123,   389,   370,   123,
   554,   123,   374,   123,   113,    29,   880,   123,   106,   466,
   884,   383,   104,   123,   568,   121,   318,   389,   427,   264,
    19,    20,   396,   397,   398,   122,   123,   124,   125,   401,
    29,   735,   404,   121,   406,   121,   408,   121,   104,   437,
   107,   413,   414,   415,   416,   417,   409,   126,   922,   421,
   124,   460,   926,   425,    29,   430,   430,   103,   113,   104,
   157,   104,   159,   116,   116,   939,   124,   430,   460,   116,
   442,   121,   374,   124,   948,   104,   107,   437,    87,   451,
   116,   453,   112,   121,   770,   112,   458,   122,   460,   123,
   123,   393,   116,   117,   118,   467,   120,   121,   122,   124,
   104,   123,   104,    52,   549,   126,     8,     9,   549,   549,
   814,   123,   107,    62,    63,   114,   115,   116,   117,   118,
   549,   120,   121,   122,    73,   122,   830,   813,   104,   121,
    79,    51,   434,   435,   436,   123,   438,     4,   121,   114,
   115,   116,   117,   118,   104,   120,   121,   122,    50,   521,
   124,   123,   101,   126,   103,   104,   458,   123,   532,   124,
   121,   123,   536,   126,   538,   467,   540,   541,   542,   112,
   121,   543,   581,   877,   549,   549,   548,   863,   112,   126,
    29,   104,   554,   113,    86,   123,   549,   547,   560,   581,
    92,    93,   564,    88,    96,   567,   568,   113,   123,   121,
   105,   113,   123,   121,   123,   614,   604,   579,   123,   569,
   582,   571,   572,   121,   121,   124,   920,   519,     0,   521,
   123,   113,   614,   123,   123,   123,    46,   121,    48,    49,
   179,    15,    16,    17,    18,    19,    20,   609,   121,   188,
     0,   123,   614,   123,   465,    29,   178,   408,    68,    69,
    70,   737,   201,   813,   770,   863,   205,   186,   560,   342,
   408,    81,    82,   212,   213,   114,   115,   116,   117,   118,
   409,   120,   121,   122,    94,   548,   581,   430,    -1,   402,
   186,   583,   584,   585,   586,   587,   588,   589,   590,   591,
   592,   593,   594,   595,   596,   597,   598,   599,   600,   601,
   602,   603,   330,   605,   464,   699,    -1,    -1,    -1,    -1,
    -1,     3,     4,     5,     6,    -1,     8,     9,   775,    -1,
    -1,   270,   559,    -1,    -1,    -1,   735,   699,   112,   113,
   114,   115,   116,   117,   118,    -1,   120,   121,   122,    -1,
    -1,    -1,    -1,   735,    -1,   294,    -1,    -1,     3,     4,
     5,     6,    -1,     8,    46,    -1,    48,    49,    -1,    -1,
    -1,    53,    54,   735,    -1,   763,    -1,    -1,   740,    61,
    62,    -1,    -1,    -1,    -1,    -1,    68,    69,    -1,    -1,
    -1,   330,    74,    17,    18,    19,    20,   336,    -1,    81,
    82,    83,   341,    85,   343,    29,    -1,    89,    -1,    54,
    -1,   773,    94,   763,    -1,   814,    61,    -1,    -1,    -1,
    -1,     8,     9,     8,     9,    -1,    -1,    -1,    -1,   111,
    -1,   830,   814,   115,   116,    -1,   728,   119,    83,   121,
    85,   123,    -1,   125,    89,    -1,    -1,    -1,   830,   741,
    -1,    -1,   814,   392,    -1,   817,    -1,   396,   397,   398,
   822,    -1,    -1,    50,    -1,    50,   111,    -1,   830,    -1,
   115,   116,   860,    -1,   119,    -1,   121,    -1,   877,   771,
   125,   831,   832,   833,    -1,    -1,   778,    -1,   112,   113,
   114,   115,   116,   117,   118,   877,   120,   121,   122,    86,
    -1,    86,    -1,    -1,    -1,    92,    93,    92,    93,    96,
    -1,    -1,    -1,    -1,    -1,   877,    -1,    -1,   880,    -1,
    -1,   920,   884,    -1,    -1,    -1,   465,   915,    -1,   917,
   822,    -1,   759,   760,   761,   762,    -1,   764,   920,    -1,
    13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
    -1,    -1,    -1,    -1,   942,   943,    29,    -1,   920,    -1,
   922,    -1,    -1,    -1,   926,    -1,    -1,    -1,    -1,   861,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   939,    -1,   518,
    -1,   873,   759,   760,   761,   762,   948,   764,    17,    18,
    19,    20,   531,   532,   533,    -1,    -1,   536,    -1,   538,
    29,   540,   541,   542,    -1,    -1,   545,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   841,   842,   843,   844,   845,   846,
   847,   848,   849,   850,   851,   852,   853,   854,   855,   856,
   857,   858,   859,    -1,    -1,   108,   109,   110,   111,   112,
   113,   114,   115,   116,   117,   118,    -1,   120,   121,   122,
    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   890,   841,   842,   843,   844,   845,   846,
   847,   848,   849,   850,   851,   852,   853,   854,   855,   856,
   857,   858,   859,    -1,    -1,   114,   115,   116,   117,   118,
    -1,   120,   121,   122,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   931,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   890,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
     4,     5,     6,     7,     8,     9,    10,    -1,    12,    13,
    14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
    -1,    -1,    -1,    -1,   931,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
    -1,    -1,    46,    47,    48,    49,    50,    51,    52,    53,
    54,    55,    -1,    57,    58,    59,    60,    61,    62,    63,
    64,    65,    66,    67,    68,    69,    -1,    -1,    72,    -1,
    74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
    84,    85,    86,    -1,    88,    89,    90,    91,    92,    93,
    94,    -1,    96,    97,    98,    99,    -1,    -1,    -1,   103,
   104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
   114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
   124,   125,   126,     3,     4,     5,     6,     7,     8,     9,
    10,    -1,    12,    13,    14,    15,    16,    17,    18,    19,
    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    29,
    30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
    40,    41,    42,    43,    -1,    -1,    46,    47,    48,    49,
    50,    51,    52,    53,    54,    55,    -1,    57,    58,    59,
    60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
    -1,    -1,    72,    -1,    74,    75,    76,    77,    78,    79,
    80,    81,    82,    83,    84,    85,    86,    -1,    88,    89,
    90,    91,    92,    93,    94,    -1,    96,    97,    98,    99,
    -1,    -1,    -1,   103,   104,   105,   106,   107,   108,   109,
   110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
   120,   121,   122,   123,    -1,   125,   126,    -1,    -1,   918,
     3,     4,     5,     6,    -1,     8,     9,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    14,
    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    46,
    -1,    48,    49,    46,    29,    48,    49,    50,    51,    -1,
    53,    54,    -1,    -1,    57,    -1,    -1,    -1,    61,    62,
    -1,    68,    69,    70,    -1,    68,    69,    70,    -1,    -1,
    -1,    74,    -1,    -1,    81,    82,    -1,    -1,    81,    82,
    83,    -1,    85,    86,    -1,    -1,    89,    94,    -1,    92,
    93,    94,    -1,    -1,    97,     3,     4,     5,     6,    -1,
     8,     9,    -1,    -1,    12,    -1,    -1,    -1,   111,    -1,
    -1,    -1,   115,   116,   121,    -1,   119,    -1,   121,    -1,
    -1,   106,   125,   108,   109,   110,   111,   112,   113,   114,
   115,   116,   117,   118,    -1,   120,   121,   122,    46,    -1,
    48,    49,    50,    51,    -1,    53,    54,    -1,    -1,    57,
    -1,    -1,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,
    68,    69,    70,    -1,    -1,    -1,    74,    -1,    -1,    -1,
    -1,    -1,    -1,    81,    82,    83,    -1,    85,    86,    -1,
    -1,    89,    -1,    -1,    92,    93,    94,    -1,    -1,    97,
     3,     4,     5,     6,    -1,     8,     9,    -1,    -1,    12,
    -1,    -1,    -1,   111,    -1,    -1,    -1,   115,    -1,    -1,
    -1,   119,    -1,   121,    -1,    -1,    -1,   125,    13,    14,
    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    46,
    -1,    48,    49,    46,    29,    48,    49,    50,    51,    -1,
    53,    54,    -1,    -1,    57,    -1,    -1,    -1,    61,    62,
    -1,    68,    69,    70,    -1,    68,    69,    70,    -1,    -1,
    -1,    74,    -1,    -1,    81,    82,    -1,    -1,    81,    82,
    83,    -1,    85,    86,    -1,    -1,    89,    94,    -1,    92,
    93,    94,    -1,    -1,    97,     3,     4,     5,     6,    -1,
     8,     9,    -1,    19,    20,    -1,    -1,    -1,   111,    -1,
    -1,    -1,   115,    29,   121,    -1,   119,    -1,   121,    -1,
   105,    -1,   125,   108,   109,   110,   111,   112,   113,   114,
   115,   116,   117,   118,    -1,   120,   121,   122,    46,    -1,
    48,    49,    -1,    -1,    -1,    53,    54,    -1,    -1,    -1,
    -1,    -1,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,
    68,    69,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,
    -1,    -1,    -1,    81,    82,    83,    -1,    85,    -1,    -1,
    -1,    89,     3,     4,     5,     6,    94,     8,     9,    -1,
    -1,    -1,    -1,    -1,    -1,   103,    -1,    -1,   114,   115,
   116,   117,   118,   111,   120,   121,   122,   115,   116,    -1,
    -1,   119,    -1,   121,    -1,    -1,    -1,   125,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    46,    -1,    48,    49,    -1,
    -1,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
    61,    62,    -1,    46,    -1,    48,    49,    68,    69,    -1,
    53,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    62,
    81,    82,    83,    -1,    85,    68,    69,    70,    89,     3,
     4,     5,     6,    94,     8,     9,    -1,    -1,    81,    82,
    -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   111,    94,    -1,    -1,   115,   116,    -1,    -1,   119,    -1,
   121,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    46,    -1,    48,    49,    -1,    -1,   121,    53,
    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    -1,
    46,    -1,    48,    49,    68,    69,    -1,    -1,    -1,    -1,
    74,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
    -1,    85,    68,    69,    70,    89,     3,     4,     5,     6,
    94,     8,     9,    -1,    -1,    81,    82,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    94,    -1,
    -1,   115,   116,    -1,    -1,   119,    -1,   121,    -1,   123,
    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
    -1,    48,    49,    -1,    -1,   121,    53,    54,    -1,    -1,
    -1,    -1,    -1,    -1,    61,    62,    -1,    46,    -1,    48,
    49,    68,    69,    -1,    53,    -1,    -1,    74,    -1,    -1,
    -1,    -1,    -1,    62,    81,    82,    83,    -1,    85,    68,
    69,    70,    89,     3,     4,     5,     6,    94,     8,     9,
    -1,    -1,    81,    82,    -1,    -1,   103,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   111,    94,    -1,    -1,   115,   116,
    -1,    -1,   119,    -1,   121,    -1,    -1,    -1,   125,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    48,    49,
    -1,    -1,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
    -1,    61,    62,    -1,    46,    -1,    48,    49,    68,    69,
    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,
    62,    81,    82,    83,    -1,    85,    68,    69,    70,    89,
     3,     4,     5,     6,    94,     8,     9,    -1,    -1,    81,
    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   111,    94,    -1,    -1,   115,   116,    -1,    -1,   119,
    -1,   121,    -1,   123,    -1,   125,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,
    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
    -1,    -1,    -1,    -1,    -1,    68,    69,    -1,    -1,    -1,
    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
    83,    -1,    85,    -1,    -1,    -1,    89,    -1,    -1,    -1,
    -1,    94,    -1,    13,    14,    15,    16,    17,    18,    19,
    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    29,
    -1,    -1,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
    -1,    -1,   125,    13,    14,    15,    16,    17,    18,    19,
    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,
    -1,    -1,    -1,    13,    14,    15,    16,    17,    18,    19,
    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,
    -1,    31,    32,    33,    34,    35,    36,    37,    38,    39,
    40,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,   108,   109,
   110,   111,   112,   113,   114,   115,   116,   117,   118,    -1,
   120,   121,   122,    -1,    74,    -1,   126,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,     8,     9,   108,   109,
   110,   111,   112,   113,   114,   115,   116,   117,   118,    -1,
   120,   121,   122,    -1,   104,    -1,   126,   107,    -1,   109,
   110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
    -1,   121,   122,    45,    46,   125,    48,    49,    50,    51,
    -1,    53,    -1,    -1,    -1,    57,    58,    59,    -1,    -1,
    62,    -1,     8,     9,    -1,    67,    68,    69,    70,    -1,
    72,    -1,    -1,    75,    -1,    -1,    -1,    79,    -1,    81,
    82,    -1,    84,    -1,    86,    -1,    -1,    -1,    -1,    -1,
    92,    93,    94,    -1,    96,    97,    98,    -1,    -1,    45,
    46,   103,    48,    49,    50,    51,    -1,    53,    -1,    -1,
    -1,    57,    58,    59,    -1,    -1,    62,   119,     8,     9,
    -1,    67,    68,    69,    70,    -1,    72,    -1,    -1,    75,
    -1,    -1,    -1,    79,    -1,    81,    82,    -1,    84,    -1,
    86,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,
    96,    97,    98,    -1,    -1,    45,    46,    -1,    48,    49,
    50,    51,    -1,    53,    -1,    -1,    -1,    57,    58,    59,
    -1,    -1,    62,   119,    -1,    -1,    -1,    67,    68,    69,
    70,    -1,    72,    -1,    -1,    75,    -1,    -1,    -1,    79,
    -1,    81,    82,    -1,    84,    -1,    86,    -1,    -1,    -1,
    -1,    -1,    92,    93,    94,    -1,    96,    97,    98,    13,
    14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,   119,
    13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    29,    13,    14,    15,
    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    29,    13,    14,    15,    16,    17,    18,
    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    29,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,
    -1,    -1,   106,    -1,   108,   109,   110,   111,   112,   113,
   114,   115,   116,   117,   118,    -1,   120,   121,   122,    -1,
    -1,    -1,    -1,    -1,    -1,   108,   109,   110,   111,   112,
   113,   114,   115,   116,   117,   118,    -1,   120,   121,   122,
    -1,    -1,   108,   109,   110,   111,   112,   113,   114,   115,
   116,   117,   118,    -1,   120,   121,   122,   106,    -1,   108,
   109,   110,   111,    -1,    -1,   114,   115,   116,   117,   118,
    -1,   120,   121,   122,    -1,    -1,   108,   109,   110,   111,
    -1,    -1,   114,   115,   116,   117,   118,    -1,   120,   121,
   122,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    29,    14,    15,    16,
    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    29,    15,    16,    17,    18,    19,    20,    -1,
    -1,    -1,    44,    -1,    -1,    -1,    -1,    29,    15,    16,
    17,    18,    19,    20,    56,    -1,    -1,    -1,    60,    -1,
    -1,    63,    29,    15,    16,    17,    18,    19,    20,    71,
    -1,    73,    -1,    -1,    76,    77,    78,    29,    15,    16,
    17,    18,    19,    20,    -1,    87,    -1,    -1,    -1,    91,
    -1,    -1,    29,    95,    -1,    -1,   109,   110,   111,   112,
   113,   114,   115,   116,   117,   118,    -1,   120,   121,   122,
    -1,    -1,   109,   110,   111,    -1,    -1,   114,   115,   116,
   117,   118,    -1,   120,   121,   122,    -1,   109,   110,   111,
   112,   113,   114,   115,   116,   117,   118,    -1,   120,   121,
   122,    -1,    -1,   110,   111,   112,   113,   114,   115,   116,
   117,   118,    -1,   120,   121,   122,    -1,    -1,    -1,   111,
   112,   113,   114,   115,   116,   117,   118,    -1,   120,   121,
   122,    -1,   109,   110,   111,    -1,    -1,   114,   115,   116,
   117,   118,    -1,   120,   121,   122,    15,    16,    17,    18,
    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    29,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
     8,     9,    -1,    -1,    -1,    29,    15,    16,    17,    18,
    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,
    48,    49,    50,    51,    -1,    53,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,     0,    -1,
    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   110,   111,    81,    82,   114,   115,   116,   117,   118,
    -1,   120,   121,   122,    92,    -1,    94,   111,    -1,    97,
   114,   115,   116,   117,   118,    -1,   120,   121,   122,    -1,
    -1,    -1,    44,    -1,    -1,   114,   115,   116,   117,   118,
    -1,   120,   121,   122,    56,    -1,     8,     9,    60,    -1,
    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
    -1,    73,    -1,    -1,    76,    77,    78,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,
    -1,    -1,    -1,    95,    46,     9,    48,    49,    -1,    -1,
    -1,    53,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
    62,    -1,    -1,    -1,    -1,    -1,    68,    69,    70,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
    82,    -1,    46,    -1,    48,    49,    50,    51,    -1,    53,
    92,    -1,    94,    57,    -1,    97,    -1,     9,    62,    -1,
    -1,    -1,    -1,    -1,    68,    69,    70,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    -1,
    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    92,    93,
    94,    -1,    -1,    97,    46,    -1,    48,    49,    50,    51,
    -1,    53,    -1,    -1,    -1,    57,    -1,     9,    -1,    -1,
    62,    -1,    -1,    -1,    -1,    -1,    68,    69,    70,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
    82,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
    92,    93,    94,    -1,    46,    97,    48,    49,    50,    -1,
    -1,    53,    -1,    -1,    -1,    57,    -1,     9,    -1,    -1,
    62,    -1,    -1,    -1,    -1,    -1,    68,    69,    70,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
    82,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
    92,    93,    94,    -1,    46,    97,    48,    49,    50,    -1,
    -1,    53,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,
    62,    -1,    -1,    -1,    -1,    -1,    68,    69,    70,    44,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
    82,    56,    -1,    -1,    86,    60,    -1,    -1,    63,    -1,
    92,    93,    94,    -1,    44,    97,    71,    -1,    73,    -1,
    -1,    76,    77,    78,    -1,    -1,    56,    -1,    -1,    -1,
    60,    -1,    87,    63,    -1,    -1,    91,    -1,    -1,    44,
    95,    71,    -1,    73,    -1,    -1,    76,    77,    78,    -1,
   105,    56,    -1,    -1,    -1,    60,    -1,    87,    63,    -1,
    -1,    91,    -1,    -1,    44,    95,    71,    -1,    73,   124,
    -1,    76,    77,    78,    -1,   105,    56,    -1,    -1,    -1,
    60,    -1,    87,    63,    -1,    -1,    91,    -1,    -1,    44,
    95,    71,    -1,    73,   124,    -1,    76,    77,    78,    -1,
   105,    56,    -1,    -1,    -1,    60,    -1,    87,    63,    -1,
    -1,    91,    -1,    -1,    -1,    95,    71,    -1,    73,   124,
    -1,    76,    77,    78,    -1,   105,    -1,    -1,    -1,    -1,
    -1,    -1,    87,    -1,    -1,    -1,    91,    -1,    -1,    -1,
    95,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124
};
#define YYPURE 1

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

case 2:
#line 383 "dtool/src/cppparser/cppbison.yxx"
{
  current_expr = yyvsp[0].u.expr;
;
    break;}
case 3:
#line 387 "dtool/src/cppparser/cppbison.yxx"
{
  current_type = yyvsp[0].u.type;
;
    break;}
case 9:
#line 405 "dtool/src/cppparser/cppbison.yxx"
{
  delete yyvsp[-1].u.expr;
;
    break;}
case 10:
#line 417 "dtool/src/cppparser/cppbison.yxx"
{
  push_storage_class((current_storage_class & ~CPPInstance::SC_c_binding) | 
                     (yyvsp[-1].u.integer & CPPInstance::SC_c_binding));
;
    break;}
case 11:
#line 422 "dtool/src/cppparser/cppbison.yxx"
{
  pop_storage_class();
;
    break;}
case 19:
#line 436 "dtool/src/cppparser/cppbison.yxx"
{
  if (publish_nest_level != 0) {
    yyerror("Unclosed __begin_publish", publish_loc);
    publish_nest_level = 0;
    current_scope->set_current_vis(V_public);
  }

  publish_previous = current_scope->get_current_vis();
  publish_loc = yylsp[0];
  publish_nest_level++;
  current_scope->set_current_vis(V_published);
;
    break;}
case 20:
#line 449 "dtool/src/cppparser/cppbison.yxx"
{
  if (publish_nest_level != 1) {
    yyerror("Unmatched __end_publish", yylsp[0]);
  } else {
    current_scope->set_current_vis(publish_previous);
  }
  publish_nest_level = 0;
;
    break;}
case 21:
#line 458 "dtool/src/cppparser/cppbison.yxx"
{
  current_scope->set_current_vis(V_published);
;
    break;}
case 22:
#line 462 "dtool/src/cppparser/cppbison.yxx"
{
  if (publish_nest_level > 0) {
    current_scope->set_current_vis(V_published);
  } else {
    current_scope->set_current_vis(V_public);
  }
;
    break;}
case 23:
#line 470 "dtool/src/cppparser/cppbison.yxx"
{
  current_scope->set_current_vis(V_protected);
;
    break;}
case 24:
#line 474 "dtool/src/cppparser/cppbison.yxx"
{
  current_scope->set_current_vis(V_private);
;
    break;}
case 25:
#line 478 "dtool/src/cppparser/cppbison.yxx"
{
  CPPMakeSeq *make_seq = new CPPMakeSeq(yyvsp[-6].u.identifier->get_simple_name(), yyvsp[-4].u.identifier->get_simple_name(), yyvsp[-2].u.identifier->get_simple_name(), yylsp[-8].file);
  current_scope->add_declaration(make_seq, global_scope, current_lexer, yylsp[-8]);
;
    break;}
case 26:
#line 486 "dtool/src/cppparser/cppbison.yxx"
{
  CPPScope *new_scope = new CPPScope(current_scope, CPPNameComponent("temp"),
                                     V_public);
  push_scope(new_scope);
;
    break;}
case 27:
#line 492 "dtool/src/cppparser/cppbison.yxx"
{
  delete current_scope;
  pop_scope();
;
    break;}
case 28:
#line 501 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.integer = 0;
;
    break;}
case 29:
#line 505 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.integer = yyvsp[-1].u.integer | (int)CPPInstance::SC_extern;
;
    break;}
case 30:
#line 509 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.integer = yyvsp[-2].u.integer | (int)CPPInstance::SC_extern;
  if (yyvsp[0].str == "C") {
    yyval.u.integer |= (int)CPPInstance::SC_c_binding;
  } else if (yyvsp[0].str == "C++") {
    yyval.u.integer &= ~(int)CPPInstance::SC_c_binding;
  } else {
    yywarning("Ignoring unknown linkage type \"" + yyvsp[0].str + "\"", yylsp[0]);
  }
;
    break;}
case 31:
#line 520 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.integer = yyvsp[-1].u.integer | (int)CPPInstance::SC_static;
;
    break;}
case 32:
#line 524 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.integer = yyvsp[-1].u.integer | (int)CPPInstance::SC_inline;
;
    break;}
case 33:
#line 528 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.integer = yyvsp[-1].u.integer | (int)CPPInstance::SC_virtual;
;
    break;}
case 34:
#line 532 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.integer = yyvsp[-1].u.integer | (int)CPPInstance::SC_explicit;
;
    break;}
case 35:
#line 536 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.integer = yyvsp[-1].u.integer | (int)CPPInstance::SC_volatile;
;
    break;}
case 36:
#line 540 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.integer = yyvsp[-1].u.integer | (int)CPPInstance::SC_mutable;
;
    break;}
case 37:
#line 544 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.integer = yyvsp[-1].u.integer | (int)CPPInstance::SC_register;
;
    break;}
case 38:
#line 548 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.integer = yyvsp[-1].u.integer | (int)CPPInstance::SC_blocking;
;
    break;}
case 39:
#line 555 "dtool/src/cppparser/cppbison.yxx"
{
  /* multiple_var_declaration adds itself to the scope. */
;
    break;}
case 40:
#line 559 "dtool/src/cppparser/cppbison.yxx"
{
  // We don't really care about the storage class here.  In fact, it's
  // not actually legal to define a class or struct using a particular
  // storage class, but we require it just to help yacc out in its
  // parsing.

  current_scope->add_declaration(yyvsp[-1].u.decl, global_scope, current_lexer, yylsp[-1]);
;
    break;}
case 41:
#line 568 "dtool/src/cppparser/cppbison.yxx"
{
  if (yyvsp[-1].u.instance != (CPPInstance *)NULL) {
    yyvsp[-1].u.instance->_storage_class |= (current_storage_class | yyvsp[-2].u.integer);
    current_scope->add_declaration(yyvsp[-1].u.instance, global_scope, current_lexer, yylsp[-1]);
    yyvsp[-1].u.instance->set_initializer(yyvsp[0].u.expr);
  }
;
    break;}
case 42:
#line 579 "dtool/src/cppparser/cppbison.yxx"
{
  // We don't need to push/pop type, because we can't nest
  // multiple_var_declarations.
  if (yyvsp[0].u.decl->as_type_declaration()) {
    current_type = yyvsp[0].u.decl->as_type_declaration()->_type;
  } else {
    current_type = yyvsp[0].u.decl->as_type();
  }
  push_storage_class(yyvsp[-1].u.integer);
;
    break;}
case 43:
#line 590 "dtool/src/cppparser/cppbison.yxx"
{
  pop_storage_class();
;
    break;}
case 44:
#line 594 "dtool/src/cppparser/cppbison.yxx"
{
  // We don't need to push/pop type, because we can't nest
  // multiple_var_declarations.
  current_type = yyvsp[0].u.type;
  push_storage_class(yyvsp[-2].u.integer);
;
    break;}
case 45:
#line 601 "dtool/src/cppparser/cppbison.yxx"
{
  pop_storage_class();
;
    break;}
case 46:
#line 612 "dtool/src/cppparser/cppbison.yxx"
{
  CPPInstance *inst = new CPPInstance(current_type, yyvsp[-1].u.inst_ident,
                                      current_storage_class,
                                      yylsp[-1].file);
  inst->set_initializer(yyvsp[0].u.expr);
  current_scope->add_declaration(inst, global_scope, current_lexer, yylsp[-1]);
;
    break;}
case 47:
#line 620 "dtool/src/cppparser/cppbison.yxx"
{
  CPPInstance *inst = new CPPInstance(current_type, yyvsp[-3].u.inst_ident,
                                      current_storage_class,
                                      yylsp[-3].file);
  inst->set_initializer(yyvsp[-2].u.expr);
  current_scope->add_declaration(inst, global_scope, current_lexer, yylsp[-3]);
;
    break;}
case 48:
#line 631 "dtool/src/cppparser/cppbison.yxx"
{
  yyvsp[-1].u.inst_ident->add_modifier(IIT_const);
  CPPInstance *inst = new CPPInstance(current_type, yyvsp[-1].u.inst_ident,
                                      current_storage_class,
                                      yylsp[-1].file);
  inst->set_initializer(yyvsp[0].u.expr);
  current_scope->add_declaration(inst, global_scope, current_lexer, yylsp[-1]);
;
    break;}
case 49:
#line 640 "dtool/src/cppparser/cppbison.yxx"
{
  yyvsp[-3].u.inst_ident->add_modifier(IIT_const);
  CPPInstance *inst = new CPPInstance(current_type, yyvsp[-3].u.inst_ident,
                                      current_storage_class,
                                      yylsp[-3].file);
  inst->set_initializer(yyvsp[-2].u.expr);
  current_scope->add_declaration(inst, global_scope, current_lexer, yylsp[-3]);
;
    break;}
case 50:
#line 653 "dtool/src/cppparser/cppbison.yxx"
{
  // We don't need to push/pop type, because we can't nest
  // multiple_var_declarations.
  if (yyvsp[0].u.decl->as_type_declaration()) {
    current_type = yyvsp[0].u.decl->as_type_declaration()->_type;
  } else {
    current_type = yyvsp[0].u.decl->as_type();
  }
  push_storage_class(yyvsp[-1].u.integer);
;
    break;}
case 51:
#line 664 "dtool/src/cppparser/cppbison.yxx"
{
  pop_storage_class();
;
    break;}
case 52:
#line 668 "dtool/src/cppparser/cppbison.yxx"
{
  // We don't need to push/pop type, because we can't nest
  // multiple_var_declarations.
  current_type = yyvsp[0].u.type;
  push_storage_class(yyvsp[-2].u.integer);
;
    break;}
case 53:
#line 675 "dtool/src/cppparser/cppbison.yxx"
{
  pop_storage_class();
;
    break;}
case 54:
#line 679 "dtool/src/cppparser/cppbison.yxx"
{
  if (yyvsp[-1].u.instance != (CPPDeclaration *)NULL) {
    CPPInstance *inst = yyvsp[-1].u.instance->as_instance();
    if (inst != (CPPInstance *)NULL) {
      inst->_storage_class |= (current_storage_class | yyvsp[-2].u.integer);
      current_scope->add_declaration(inst, global_scope, current_lexer, yylsp[-1]);
      current_scope->add_declaration(new CPPTypedef(inst, current_scope == global_scope), global_scope, current_lexer, yylsp[-1]);
    }
  }
;
    break;}
case 55:
#line 693 "dtool/src/cppparser/cppbison.yxx"
{
  CPPInstance *inst = new CPPInstance(current_type, yyvsp[-1].u.inst_ident,
                                      current_storage_class,
                                      yylsp[-1].file);
  inst->set_initializer(yyvsp[0].u.expr);
  current_scope->add_declaration(new CPPTypedef(inst, current_scope == global_scope), global_scope, current_lexer, yylsp[-1]);
;
    break;}
case 56:
#line 701 "dtool/src/cppparser/cppbison.yxx"
{
  CPPInstance *inst = new CPPInstance(current_type, yyvsp[-3].u.inst_ident,
                                      current_storage_class,
                                      yylsp[-3].file);
  inst->set_initializer(yyvsp[-2].u.expr);
  current_scope->add_declaration(new CPPTypedef(inst, current_scope == global_scope), global_scope, current_lexer, yylsp[-3]);
;
    break;}
case 57:
#line 712 "dtool/src/cppparser/cppbison.yxx"
{
  yyvsp[-1].u.inst_ident->add_modifier(IIT_const);
  CPPInstance *inst = new CPPInstance(current_type, yyvsp[-1].u.inst_ident,
                                      current_storage_class,
                                      yylsp[-1].file);
  inst->set_initializer(yyvsp[0].u.expr);
  current_scope->add_declaration(new CPPTypedef(inst, current_scope == global_scope), global_scope, current_lexer, yylsp[-1]);
;
    break;}
case 58:
#line 721 "dtool/src/cppparser/cppbison.yxx"
{
  yyvsp[-3].u.inst_ident->add_modifier(IIT_const);
  CPPInstance *inst = new CPPInstance(current_type, yyvsp[-3].u.inst_ident,
                                      current_storage_class,
                                      yylsp[-3].file);
  inst->set_initializer(yyvsp[-2].u.expr);
  current_scope->add_declaration(new CPPTypedef(inst, current_scope == global_scope), global_scope, current_lexer, yylsp[-3]);
;
    break;}
case 59:
#line 735 "dtool/src/cppparser/cppbison.yxx"
{
  push_scope(yyvsp[-1].u.identifier->get_scope(current_scope, global_scope));
;
    break;}
case 60:
#line 739 "dtool/src/cppparser/cppbison.yxx"
{
  pop_scope();
  CPPType *type;
  if (yyvsp[-5].u.identifier->get_simple_name() == current_scope->get_simple_name()) {
    // This is a constructor, and has no return.
    type = new CPPSimpleType(CPPSimpleType::T_void);
  } else {
    // This isn't a constructor, so it has an implicit return type of
    // int.
    type = new CPPSimpleType(CPPSimpleType::T_int);
  }

  CPPInstanceIdentifier *ii = new CPPInstanceIdentifier(yyvsp[-5].u.identifier);
  ii->add_func_modifier(yyvsp[-2].u.param_list, yyvsp[0].u.integer);

  yyval.u.instance = new CPPInstance(type, ii, 0, yylsp[-5].file);
;
    break;}
case 61:
#line 757 "dtool/src/cppparser/cppbison.yxx"
{
  push_scope(yyvsp[-1].u.identifier->get_scope(current_scope, global_scope));
;
    break;}
case 62:
#line 761 "dtool/src/cppparser/cppbison.yxx"
{
  pop_scope();
  CPPType *type;
  if (yyvsp[-5].u.identifier->get_simple_name() == current_scope->get_simple_name()) {
    // This is a constructor, and has no return.
    type = new CPPSimpleType(CPPSimpleType::T_void);
  } else {
    // This isn't a constructor, so it has an implicit return type of
    // int.
    type = new CPPSimpleType(CPPSimpleType::T_int);
  }

  CPPInstanceIdentifier *ii = new CPPInstanceIdentifier(yyvsp[-5].u.identifier);
  ii->add_func_modifier(yyvsp[-2].u.param_list, yyvsp[0].u.integer);

  yyval.u.instance = new CPPInstance(type, ii, 0, yylsp[-5].file);
;
    break;}
case 63:
#line 781 "dtool/src/cppparser/cppbison.yxx"
{
  push_scope(yyvsp[-1].u.identifier->get_scope(current_scope, global_scope));
;
    break;}
case 64:
#line 785 "dtool/src/cppparser/cppbison.yxx"
{
  pop_scope();
  if (yyvsp[-5].u.identifier->is_scoped()) {
    yyerror("Invalid destructor name: ~" + yyvsp[-5].u.identifier->get_fully_scoped_name(), yylsp[-5]);
  } else {
    CPPIdentifier *ident = 
      new CPPIdentifier("~" + yyvsp[-5].u.identifier->get_simple_name(), yylsp[-5].file);
    delete yyvsp[-5].u.identifier;

    CPPType *type;
    type = new CPPSimpleType(CPPSimpleType::T_void);
    
    CPPInstanceIdentifier *ii = new CPPInstanceIdentifier(ident);
    ii->add_func_modifier(yyvsp[-2].u.param_list, yyvsp[0].u.integer);

    yyval.u.instance = new CPPInstance(type, ii, 0, yylsp[-5].file);
  }
;
    break;}
case 65:
#line 811 "dtool/src/cppparser/cppbison.yxx"
{
  push_scope(yyvsp[-2].u.inst_ident->get_scope(current_scope, global_scope));
;
    break;}
case 66:
#line 815 "dtool/src/cppparser/cppbison.yxx"
{
  pop_scope();
  CPPType *type = yyvsp[-9].u.identifier->find_type(current_scope, global_scope, false, current_lexer);
  assert(type != NULL);

  CPPInstanceIdentifier *ii = yyvsp[-6].u.inst_ident;
  ii->add_modifier(IIT_pointer);
  ii->add_func_modifier(yyvsp[-2].u.param_list, yyvsp[0].u.integer);
  yyval.u.instance = new CPPInstance(type, ii, 0, yylsp[-9].file);
;
    break;}
case 67:
#line 826 "dtool/src/cppparser/cppbison.yxx"
{
  push_scope(yyvsp[-2].u.inst_ident->get_scope(current_scope, global_scope));
;
    break;}
case 68:
#line 830 "dtool/src/cppparser/cppbison.yxx"
{
  pop_scope();
  CPPType *type = yyvsp[-10].u.identifier->find_type(current_scope, global_scope, false, current_lexer);
  assert(type != NULL);

  CPPInstanceIdentifier *ii = yyvsp[-6].u.inst_ident;
  ii->add_scoped_pointer_modifier(yyvsp[-8].u.identifier);
  ii->add_func_modifier(yyvsp[-2].u.param_list, yyvsp[0].u.integer);
  yyval.u.instance = new CPPInstance(type, ii, 0, yylsp[-10].file);
;
    break;}
case 69:
#line 843 "dtool/src/cppparser/cppbison.yxx"
{
  if (yyvsp[-3].u.identifier != NULL) {
    push_scope(yyvsp[-3].u.identifier->get_scope(current_scope, global_scope));
  }
;
    break;}
case 70:
#line 849 "dtool/src/cppparser/cppbison.yxx"
{
  if (yyvsp[-7].u.identifier != NULL) {
    pop_scope();
  }

  // We use formal_parameter_identifier, because that can match a type
  // name with or without an identifier, but especially without, which
  // is what follows the keyword "operator" in a typecast function.
  // As an added bonus, the type of the formal_parameter will be the
  // typecast type, i.e. the return type of the typecast function.

  // We give typecast operators the name "operator typecast <name>",
  // where <name> is a simple name of the type to be typecast.  Use
  // the method's return type to determine the full type description.
  string name = "operator typecast " + yyvsp[-6].u.type->get_simple_name();
  CPPIdentifier *ident = yyvsp[-7].u.identifier;
  if (ident == NULL) {
    ident = new CPPIdentifier(name, yylsp[-7].file);
  } else {
    ident->add_name(name);
  }
  yyval.u.instance = CPPInstance::make_typecast_function
    (new CPPInstance(yyvsp[-6].u.type, yyvsp[-5].u.inst_ident, 0, yylsp[-5].file), ident, yyvsp[-2].u.param_list, yyvsp[0].u.integer);
;
    break;}
case 71:
#line 874 "dtool/src/cppparser/cppbison.yxx"
{
  if (yyvsp[-4].u.identifier != NULL) {
    push_scope(yyvsp[-4].u.identifier->get_scope(current_scope, global_scope));
  }
;
    break;}
case 72:
#line 880 "dtool/src/cppparser/cppbison.yxx"
{
  if (yyvsp[-8].u.identifier != NULL) {
    pop_scope();
  }

  CPPIdentifier *ident = yyvsp[-8].u.identifier;
  if (ident == NULL) {
    ident = new CPPIdentifier("operator typecast", yylsp[-8].file);
  } else {
    ident->add_name("operator typecast");
  }
  yyvsp[-5].u.inst_ident->add_modifier(IIT_const);
  yyval.u.instance = CPPInstance::make_typecast_function
    (new CPPInstance(yyvsp[-6].u.type, yyvsp[-5].u.inst_ident, 0, yylsp[-5].file), ident, yyvsp[-2].u.param_list, yyvsp[0].u.integer);
;
    break;}
case 73:
#line 900 "dtool/src/cppparser/cppbison.yxx"
{
  CPPDeclaration *decl = 
    yyvsp[0].u.identifier->find_symbol(current_scope, global_scope, current_lexer);
  if (decl != (CPPDeclaration *)NULL) {
    yyval.u.instance = decl->as_instance();
  } else {
    yyval.u.instance = (CPPInstance *)NULL;
  }
;
    break;}
case 74:
#line 913 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.integer = 0;
;
    break;}
case 75:
#line 917 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.integer = (int)CPPFunctionType::F_const_method;
;
    break;}
case 76:
#line 921 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.integer = yyvsp[-3].u.integer;
;
    break;}
case 77:
#line 925 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.integer = yyvsp[-4].u.integer;
;
    break;}
case 78:
#line 932 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "!";
;
    break;}
case 79:
#line 936 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "~";
;
    break;}
case 80:
#line 940 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "*";
;
    break;}
case 81:
#line 944 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "/";
;
    break;}
case 82:
#line 948 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "%";
;
    break;}
case 83:
#line 952 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "+";
;
    break;}
case 84:
#line 956 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "-";
;
    break;}
case 85:
#line 960 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "|";
;
    break;}
case 86:
#line 964 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "&";
;
    break;}
case 87:
#line 968 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "^";
;
    break;}
case 88:
#line 972 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "||";
;
    break;}
case 89:
#line 976 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "&&";
;
    break;}
case 90:
#line 980 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "==";
;
    break;}
case 91:
#line 984 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "!=";
;
    break;}
case 92:
#line 988 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "<=";
;
    break;}
case 93:
#line 992 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = ">=";
;
    break;}
case 94:
#line 996 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "<";
;
    break;}
case 95:
#line 1000 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = ">";
;
    break;}
case 96:
#line 1004 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "<<";
;
    break;}
case 97:
#line 1008 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = ">>";
;
    break;}
case 98:
#line 1012 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "=";
;
    break;}
case 99:
#line 1016 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = ",";
;
    break;}
case 100:
#line 1020 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "++";
;
    break;}
case 101:
#line 1024 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "--";
;
    break;}
case 102:
#line 1028 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "*=";
;
    break;}
case 103:
#line 1032 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "/=";
;
    break;}
case 104:
#line 1036 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "%=";
;
    break;}
case 105:
#line 1040 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "+=";
;
    break;}
case 106:
#line 1044 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "-=";
;
    break;}
case 107:
#line 1048 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "|=";
;
    break;}
case 108:
#line 1052 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "&=";
;
    break;}
case 109:
#line 1056 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "^=";
;
    break;}
case 110:
#line 1060 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "<<=";
;
    break;}
case 111:
#line 1064 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = ">>=";
;
    break;}
case 112:
#line 1068 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "->";
;
    break;}
case 113:
#line 1072 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "[]";
;
    break;}
case 114:
#line 1076 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "()";
;
    break;}
case 115:
#line 1080 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "new";
;
    break;}
case 116:
#line 1084 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = "delete";
;
    break;}
case 119:
#line 1096 "dtool/src/cppparser/cppbison.yxx"
{
  push_scope(new CPPTemplateScope(current_scope));
;
    break;}
case 120:
#line 1100 "dtool/src/cppparser/cppbison.yxx"
{
  pop_scope();
;
    break;}
case 123:
#line 1112 "dtool/src/cppparser/cppbison.yxx"
{
  CPPTemplateScope *ts = current_scope->as_template_scope();
  assert(ts != NULL);
  ts->add_template_parameter(yyvsp[0].u.decl);
;
    break;}
case 124:
#line 1118 "dtool/src/cppparser/cppbison.yxx"
{
  CPPTemplateScope *ts = current_scope->as_template_scope();
  assert(ts != NULL);
  ts->add_template_parameter(yyvsp[0].u.decl);
;
    break;}
case 125:
#line 1127 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.decl = CPPType::new_type(new CPPClassTemplateParameter(yyvsp[0].u.identifier));
;
    break;}
case 126:
#line 1131 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.decl = CPPType::new_type(new CPPClassTemplateParameter(yyvsp[-2].u.identifier, yyvsp[0].u.type));
;
    break;}
case 127:
#line 1135 "dtool/src/cppparser/cppbison.yxx"
{
  CPPInstance *inst = new CPPInstance(yyvsp[-2].u.type, yyvsp[-1].u.inst_ident, 0, yylsp[-1].file);
  inst->set_initializer(yyvsp[0].u.expr);
  yyval.u.decl = inst;
;
    break;}
case 128:
#line 1141 "dtool/src/cppparser/cppbison.yxx"
{
  yyvsp[-1].u.inst_ident->add_modifier(IIT_const);
  CPPInstance *inst = new CPPInstance(yyvsp[-2].u.type, yyvsp[-1].u.inst_ident, 0, yylsp[-1].file);
  inst->set_initializer(yyvsp[0].u.expr);
  yyval.u.decl = inst;
;
    break;}
case 129:
#line 1151 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.type = CPPType::new_type(yyvsp[0].u.simple_type);
;
    break;}
case 130:
#line 1155 "dtool/src/cppparser/cppbison.yxx"
{
  yywarning("Not a type: " + yyvsp[0].u.identifier->get_fully_scoped_name(), yylsp[0]);
  yyval.u.type = CPPType::new_type(new CPPSimpleType(CPPSimpleType::T_unknown));
;
    break;}
case 131:
#line 1160 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.type = yyvsp[0].u.identifier->find_type(current_scope, global_scope, false, current_lexer);
  assert(yyval.u.type != NULL);
;
    break;}
case 132:
#line 1165 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.type = CPPType::new_type(new CPPTBDType(yyvsp[0].u.identifier));
;
    break;}
case 133:
#line 1173 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = new CPPInstanceIdentifier(yyvsp[0].u.identifier);
;
    break;}
case 134:
#line 1177 "dtool/src/cppparser/cppbison.yxx"
{
  // For an operator function.  We implement this simply by building a
  // ficticious name for the function; in other respects it's just
  // like a regular function.
  CPPIdentifier *ident = yyvsp[-1].u.identifier;
  if (ident == NULL) {
    ident = new CPPIdentifier("operator "+yyvsp[0].str, yylsp[0].file);
  } else {
    ident->_names.push_back("operator "+yyvsp[0].str);
  }

  yyval.u.inst_ident = new CPPInstanceIdentifier(ident);
;
    break;}
case 135:
#line 1191 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[0].u.inst_ident;
  yyval.u.inst_ident->add_modifier(IIT_const);
;
    break;}
case 136:
#line 1196 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[0].u.inst_ident;
  yyval.u.inst_ident->add_modifier(IIT_pointer);
;
    break;}
case 137:
#line 1201 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[0].u.inst_ident;
  yyval.u.inst_ident->add_modifier(IIT_reference);
;
    break;}
case 138:
#line 1206 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[0].u.inst_ident;
  yyval.u.inst_ident->add_scoped_pointer_modifier(yyvsp[-2].u.identifier);
;
    break;}
case 139:
#line 1211 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[-3].u.inst_ident;
  yyval.u.inst_ident->add_array_modifier(yyvsp[-1].u.expr);
;
    break;}
case 140:
#line 1216 "dtool/src/cppparser/cppbison.yxx"
{
  // bitfield definition.  We ignore the bitfield for now.
  yyval.u.inst_ident = yyvsp[-2].u.inst_ident;
;
    break;}
case 141:
#line 1221 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[-1].u.inst_ident;
  yyval.u.inst_ident->add_modifier(IIT_paren);
;
    break;}
case 142:
#line 1226 "dtool/src/cppparser/cppbison.yxx"
{
  push_scope(yyvsp[-1].u.inst_ident->get_scope(current_scope, global_scope));
;
    break;}
case 143:
#line 1230 "dtool/src/cppparser/cppbison.yxx"
{
  pop_scope();
  yyval.u.inst_ident = yyvsp[-5].u.inst_ident;
  if (yyvsp[-2].u.param_list->is_parameter_expr() && yyvsp[0].u.integer == 0) {
    // Oops, this must have been an instance declaration with a
    // parameter list, not a function prototype.
    yyval.u.inst_ident->add_initializer_modifier(yyvsp[-2].u.param_list);

  } else {
    // This was (probably) a function prototype.
    yyval.u.inst_ident->add_func_modifier(yyvsp[-2].u.param_list, yyvsp[0].u.integer);
  }
;
    break;}
case 144:
#line 1248 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.param_list = new CPPParameterList;
;
    break;}
case 145:
#line 1252 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.param_list = new CPPParameterList;
  yyval.u.param_list->_includes_ellipsis = true;
;
    break;}
case 146:
#line 1257 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.param_list = yyvsp[0].u.param_list;
;
    break;}
case 147:
#line 1261 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.param_list = yyvsp[-2].u.param_list;
  yyval.u.param_list->_includes_ellipsis = true;
;
    break;}
case 148:
#line 1266 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.param_list = yyvsp[-1].u.param_list;
  yyval.u.param_list->_includes_ellipsis = true;
;
    break;}
case 149:
#line 1274 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.param_list = new CPPParameterList;
  yyval.u.param_list->_parameters.push_back(yyvsp[0].u.instance);
;
    break;}
case 150:
#line 1279 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.param_list = yyvsp[-2].u.param_list;
  yyval.u.param_list->_parameters.push_back(yyvsp[0].u.instance);
;
    break;}
case 151:
#line 1287 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = (CPPExpression *)NULL;
;
    break;}
case 152:
#line 1291 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = yyvsp[0].u.expr;
;
    break;}
case 153:
#line 1298 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = (CPPExpression *)NULL;
;
    break;}
case 154:
#line 1302 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = yyvsp[0].u.expr;
;
    break;}
case 155:
#line 1309 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = (CPPExpression *)NULL;
;
    break;}
case 156:
#line 1313 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = (CPPExpression *)NULL;
;
    break;}
case 157:
#line 1317 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = (CPPExpression *)NULL;
;
    break;}
case 158:
#line 1321 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = yyvsp[-1].u.expr;
;
    break;}
case 159:
#line 1325 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = (CPPExpression *)NULL;
;
    break;}
case 163:
#line 1338 "dtool/src/cppparser/cppbison.yxx"
{
;
    break;}
case 167:
#line 1347 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.instance = new CPPInstance(yyvsp[-2].u.type, yyvsp[-1].u.inst_ident, 0, yylsp[-1].file);
  yyval.u.instance->set_initializer(yyvsp[0].u.expr);
;
    break;}
case 168:
#line 1352 "dtool/src/cppparser/cppbison.yxx"
{
  yywarning("Not a type: " + yyvsp[-2].u.identifier->get_fully_scoped_name(), yylsp[-2]);
  CPPType *type =
    CPPType::new_type(new CPPSimpleType(CPPSimpleType::T_unknown));
  yyval.u.instance = new CPPInstance(type, yyvsp[-1].u.inst_ident, 0, yylsp[-1].file);
  yyval.u.instance->set_initializer(yyvsp[0].u.expr);
;
    break;}
case 169:
#line 1360 "dtool/src/cppparser/cppbison.yxx"
{
  yyvsp[-1].u.inst_ident->add_modifier(IIT_const);
  yyval.u.instance = new CPPInstance(yyvsp[-2].u.type, yyvsp[-1].u.inst_ident, 0, yylsp[-1].file);
  yyval.u.instance->set_initializer(yyvsp[0].u.expr);
;
    break;}
case 170:
#line 1366 "dtool/src/cppparser/cppbison.yxx"
{
  CPPType *type =
    CPPType::new_type(new CPPSimpleType(CPPSimpleType::T_parameter));
  yyval.u.instance = new CPPInstance(type, "expr");
  yyval.u.instance->set_initializer(yyvsp[0].u.expr);
;
    break;}
case 171:
#line 1376 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = new CPPInstanceIdentifier((CPPIdentifier *)NULL);
;
    break;}
case 172:
#line 1380 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = new CPPInstanceIdentifier(yyvsp[0].u.identifier);
;
    break;}
case 173:
#line 1384 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = new CPPInstanceIdentifier(yyvsp[0].u.identifier);
;
    break;}
case 174:
#line 1388 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[0].u.inst_ident;
  yyval.u.inst_ident->add_modifier(IIT_const);
;
    break;}
case 175:
#line 1393 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[0].u.inst_ident;
  yyval.u.inst_ident->add_modifier(IIT_pointer);
;
    break;}
case 176:
#line 1398 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[0].u.inst_ident;
  yyval.u.inst_ident->add_modifier(IIT_reference);
;
    break;}
case 177:
#line 1403 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[0].u.inst_ident;
  yyval.u.inst_ident->add_scoped_pointer_modifier(yyvsp[-2].u.identifier);
;
    break;}
case 178:
#line 1408 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[-3].u.inst_ident;
  yyval.u.inst_ident->add_array_modifier(yyvsp[-1].u.expr);
;
    break;}
case 179:
#line 1416 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = new CPPInstanceIdentifier((CPPIdentifier *)NULL);
;
    break;}
case 180:
#line 1420 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = new CPPInstanceIdentifier(yyvsp[0].u.identifier);
;
    break;}
case 181:
#line 1424 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = new CPPInstanceIdentifier(yyvsp[0].u.identifier);
;
    break;}
case 182:
#line 1428 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[0].u.inst_ident;
  yyval.u.inst_ident->add_modifier(IIT_const);
;
    break;}
case 183:
#line 1433 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[0].u.inst_ident;
  yyval.u.inst_ident->add_modifier(IIT_pointer);
;
    break;}
case 184:
#line 1438 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[0].u.inst_ident;
  yyval.u.inst_ident->add_modifier(IIT_reference);
;
    break;}
case 185:
#line 1443 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[0].u.inst_ident;
  yyval.u.inst_ident->add_scoped_pointer_modifier(yyvsp[-2].u.identifier);
;
    break;}
case 186:
#line 1448 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[-3].u.inst_ident;
  yyval.u.inst_ident->add_array_modifier(yyvsp[-1].u.expr);
;
    break;}
case 187:
#line 1453 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[-5].u.inst_ident;
  yyval.u.inst_ident->add_modifier(IIT_paren);
  yyval.u.inst_ident->add_func_modifier(yyvsp[-2].u.param_list, yyvsp[0].u.integer);
;
    break;}
case 188:
#line 1459 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[-1].u.inst_ident;
  yyval.u.inst_ident->add_modifier(IIT_paren);
;
    break;}
case 189:
#line 1467 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = new CPPInstanceIdentifier((CPPIdentifier *)NULL);
;
    break;}
case 190:
#line 1471 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[0].u.inst_ident;
  yyval.u.inst_ident->add_modifier(IIT_const);
;
    break;}
case 191:
#line 1476 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[0].u.inst_ident;
  yyval.u.inst_ident->add_modifier(IIT_pointer);
;
    break;}
case 192:
#line 1481 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[0].u.inst_ident;
  yyval.u.inst_ident->add_modifier(IIT_reference);
;
    break;}
case 193:
#line 1486 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[0].u.inst_ident;
  yyval.u.inst_ident->add_scoped_pointer_modifier(yyvsp[-2].u.identifier);
;
    break;}
case 194:
#line 1491 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[-3].u.inst_ident;
  yyval.u.inst_ident->add_array_modifier(yyvsp[-1].u.expr);
;
    break;}
case 195:
#line 1496 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.inst_ident = yyvsp[-5].u.inst_ident;
  yyval.u.inst_ident->add_modifier(IIT_paren);
  yyval.u.inst_ident->add_func_modifier(yyvsp[-2].u.param_list, yyvsp[0].u.integer);
;
    break;}
case 196:
#line 1505 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.type = CPPType::new_type(yyvsp[0].u.simple_type);
;
    break;}
case 197:
#line 1509 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.type = yyvsp[0].u.identifier->find_type(current_scope, global_scope, false, current_lexer);
  assert(yyval.u.type != NULL);
;
    break;}
case 198:
#line 1514 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.type = CPPType::new_type(new CPPTBDType(yyvsp[0].u.identifier));
;
    break;}
case 199:
#line 1518 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.type = CPPType::new_type(yyvsp[0].u.struct_type);
;
    break;}
case 200:
#line 1522 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.type = CPPType::new_type(yyvsp[0].u.struct_type);
;
    break;}
case 201:
#line 1526 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.type = CPPType::new_type(yyvsp[0].u.enum_type);
;
    break;}
case 202:
#line 1530 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.type = CPPType::new_type(yyvsp[0].u.enum_type);
;
    break;}
case 203:
#line 1534 "dtool/src/cppparser/cppbison.yxx"
{
  CPPType *type = yyvsp[0].u.identifier->find_type(current_scope, global_scope, false, current_lexer);
  if (type != NULL) {
    yyval.u.type = type;
  } else {
    CPPExtensionType *et = 
      CPPType::new_type(new CPPExtensionType(yyvsp[-1].u.extension_enum, yyvsp[0].u.identifier, current_scope, yylsp[-1].file))
      ->as_extension_type();
    CPPScope *scope = yyvsp[0].u.identifier->get_scope(current_scope, global_scope);
    if (scope != NULL) {
      scope->define_extension_type(et);
    }
    yyval.u.type = et;
  }
;
    break;}
case 204:
#line 1550 "dtool/src/cppparser/cppbison.yxx"
{
  CPPType *type = yyvsp[0].u.identifier->find_type(current_scope, global_scope, false, current_lexer);
  if (type != NULL) {
    yyval.u.type = type;
  } else {
    CPPExtensionType *et = 
      CPPType::new_type(new CPPExtensionType(yyvsp[-1].u.extension_enum, yyvsp[0].u.identifier, current_scope, yylsp[-1].file))
      ->as_extension_type();
    CPPScope *scope = yyvsp[0].u.identifier->get_scope(current_scope, global_scope);
    if (scope != NULL) {
      scope->define_extension_type(et);
    }
    yyval.u.type = et;
  }
;
    break;}
case 205:
#line 1569 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.decl = CPPType::new_type(yyvsp[0].u.simple_type);
;
    break;}
case 206:
#line 1573 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.decl = yyvsp[0].u.identifier->find_type(current_scope, global_scope, false, current_lexer);
  assert(yyval.u.decl != NULL);
;
    break;}
case 207:
#line 1578 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.decl = CPPType::new_type(new CPPTBDType(yyvsp[0].u.identifier));
;
    break;}
case 208:
#line 1582 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.decl = CPPType::new_type(yyvsp[0].u.struct_type);
;
    break;}
case 209:
#line 1586 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.decl = new CPPTypeDeclaration(CPPType::new_type(yyvsp[0].u.struct_type));
;
    break;}
case 210:
#line 1590 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.decl = CPPType::new_type(yyvsp[0].u.enum_type);
;
    break;}
case 211:
#line 1594 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.decl = new CPPTypeDeclaration(CPPType::new_type(yyvsp[0].u.enum_type));
;
    break;}
case 212:
#line 1598 "dtool/src/cppparser/cppbison.yxx"
{
  CPPType *type = yyvsp[0].u.identifier->find_type(current_scope, global_scope, false, current_lexer);
  if (type != NULL) {
    yyval.u.decl = type;
  } else {
    CPPExtensionType *et = 
      CPPType::new_type(new CPPExtensionType(yyvsp[-1].u.extension_enum, yyvsp[0].u.identifier, current_scope, yylsp[-1].file))
      ->as_extension_type();
    CPPScope *scope = yyvsp[0].u.identifier->get_scope(current_scope, global_scope);
    if (scope != NULL) {
      scope->define_extension_type(et);
    }
    yyval.u.decl = et;
  }
;
    break;}
case 213:
#line 1614 "dtool/src/cppparser/cppbison.yxx"
{
  CPPType *type = yyvsp[0].u.identifier->find_type(current_scope, global_scope, false, current_lexer);
  if (type != NULL) {
    yyval.u.decl = type;
  } else {
    CPPExtensionType *et = 
      CPPType::new_type(new CPPExtensionType(yyvsp[-1].u.extension_enum, yyvsp[0].u.identifier, current_scope, yylsp[-1].file))
      ->as_extension_type();
    CPPScope *scope = yyvsp[0].u.identifier->get_scope(current_scope, global_scope);
    if (scope != NULL) {
      scope->define_extension_type(et);
    }
    yyval.u.decl = et;
  }
;
    break;}
case 214:
#line 1633 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.type = CPPType::new_type(yyvsp[0].u.simple_type);
;
    break;}
case 215:
#line 1637 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.type = yyvsp[0].u.identifier->find_type(current_scope, global_scope, false, current_lexer);
  assert(yyval.u.type != NULL);
;
    break;}
case 216:
#line 1642 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.type = CPPType::new_type(new CPPTBDType(yyvsp[0].u.identifier));
;
    break;}
case 217:
#line 1646 "dtool/src/cppparser/cppbison.yxx"
{
  CPPType *type = yyvsp[0].u.identifier->find_type(current_scope, global_scope, false, current_lexer);
  if (type != NULL) {
    yyval.u.type = type;
  } else {
    CPPExtensionType *et = 
      CPPType::new_type(new CPPExtensionType(yyvsp[-1].u.extension_enum, yyvsp[0].u.identifier, current_scope, yylsp[-1].file))
      ->as_extension_type();
    CPPScope *scope = yyvsp[0].u.identifier->get_scope(current_scope, global_scope);
    if (scope != NULL) {
      scope->define_extension_type(et);
    }
    yyval.u.type = et;
  }
;
    break;}
case 218:
#line 1662 "dtool/src/cppparser/cppbison.yxx"
{
  CPPType *type = yyvsp[0].u.identifier->find_type(current_scope, global_scope, false, current_lexer);
  if (type != NULL) {
    yyval.u.type = type;
  } else {
    CPPExtensionType *et = 
      CPPType::new_type(new CPPExtensionType(yyvsp[-1].u.extension_enum, yyvsp[0].u.identifier, current_scope, yylsp[-1].file))
      ->as_extension_type();
    CPPScope *scope = yyvsp[0].u.identifier->get_scope(current_scope, global_scope);
    if (scope != NULL) {
      scope->define_extension_type(et);
    }
    yyval.u.type = et;
  }
;
    break;}
case 219:
#line 1681 "dtool/src/cppparser/cppbison.yxx"
{
  CPPInstance *inst = new CPPInstance(yyvsp[-1].u.type, yyvsp[0].u.inst_ident, 0, yylsp[-1].file);
  yyval.u.type = inst->_type;
  delete inst;
;
    break;}
case 220:
#line 1687 "dtool/src/cppparser/cppbison.yxx"
{
  yyvsp[0].u.inst_ident->add_modifier(IIT_const);
  CPPInstance *inst = new CPPInstance(yyvsp[-1].u.type, yyvsp[0].u.inst_ident, 0, yylsp[-2].file);
  yyval.u.type = inst->_type;
  delete inst;
;
    break;}
case 221:
#line 1697 "dtool/src/cppparser/cppbison.yxx"
{
  CPPVisibility starting_vis = 
  (yyvsp[-1].u.extension_enum == CPPExtensionType::T_class) ? V_private : V_public;
  
  CPPScope *new_scope = new CPPScope(current_scope, CPPNameComponent("anon"),
                                     starting_vis);
  CPPStructType *st = new CPPStructType(yyvsp[-1].u.extension_enum, NULL, current_scope,
                                        new_scope, yylsp[-1].file);
  new_scope->set_struct_type(st);

  push_scope(new_scope);
  push_struct(st);
;
    break;}
case 222:
#line 1711 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.struct_type = current_struct;
  current_struct->_incomplete = false;
  pop_struct();
  pop_scope();
;
    break;}
case 223:
#line 1721 "dtool/src/cppparser/cppbison.yxx"
{
  CPPVisibility starting_vis = 
  (yyvsp[-1].u.extension_enum == CPPExtensionType::T_class) ? V_private : V_public;

  CPPScope *scope = yyvsp[0].u.identifier->get_scope(current_scope, global_scope, current_lexer);
  if (scope == NULL) {
    scope = current_scope;
  }
  CPPScope *new_scope = new CPPScope(scope, yyvsp[0].u.identifier->_names.back(),
                                     starting_vis);

  CPPStructType *st = new CPPStructType(yyvsp[-1].u.extension_enum, yyvsp[0].u.identifier, current_scope,
                                        new_scope, yylsp[-1].file);
  new_scope->set_struct_type(st);
  current_scope->define_extension_type(st);
  
  push_scope(new_scope);
  push_struct(st);
;
    break;}
case 224:
#line 1741 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.struct_type = current_struct;
  current_struct->_incomplete = false;
  pop_struct();
  pop_scope();
;
    break;}
case 229:
#line 1761 "dtool/src/cppparser/cppbison.yxx"
{
  current_struct->append_derivation(yyvsp[0].u.type, V_public, false);
;
    break;}
case 230:
#line 1765 "dtool/src/cppparser/cppbison.yxx"
{
  current_struct->append_derivation(yyvsp[0].u.type, V_protected, false);
;
    break;}
case 231:
#line 1769 "dtool/src/cppparser/cppbison.yxx"
{
  current_struct->append_derivation(yyvsp[0].u.type, V_private, false);
;
    break;}
case 232:
#line 1773 "dtool/src/cppparser/cppbison.yxx"
{
  current_struct->append_derivation(yyvsp[0].u.type, V_public, true);
;
    break;}
case 233:
#line 1777 "dtool/src/cppparser/cppbison.yxx"
{
  current_struct->append_derivation(yyvsp[0].u.type, V_protected, true);
;
    break;}
case 234:
#line 1781 "dtool/src/cppparser/cppbison.yxx"
{
  current_struct->append_derivation(yyvsp[0].u.type, V_private, true);
;
    break;}
case 235:
#line 1785 "dtool/src/cppparser/cppbison.yxx"
{
  current_struct->append_derivation(yyvsp[0].u.type, V_public, true);
;
    break;}
case 236:
#line 1789 "dtool/src/cppparser/cppbison.yxx"
{
  current_struct->append_derivation(yyvsp[0].u.type, V_protected, true);
;
    break;}
case 237:
#line 1793 "dtool/src/cppparser/cppbison.yxx"
{
  current_struct->append_derivation(yyvsp[0].u.type, V_private, true);
;
    break;}
case 238:
#line 1800 "dtool/src/cppparser/cppbison.yxx"
{
  current_enum = new CPPEnumType(NULL, current_scope, yylsp[-1].file);
;
    break;}
case 239:
#line 1804 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.enum_type = current_enum;
  current_enum = NULL;
;
    break;}
case 240:
#line 1812 "dtool/src/cppparser/cppbison.yxx"
{
  current_enum = new CPPEnumType(yyvsp[-1].u.identifier, current_scope, yylsp[-2].file);
;
    break;}
case 241:
#line 1816 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.enum_type = current_enum;
  current_enum = NULL;
;
    break;}
case 245:
#line 1830 "dtool/src/cppparser/cppbison.yxx"
{
  assert(current_enum != NULL);
  current_enum->add_element(yyvsp[0].u.identifier->get_simple_name(), current_scope);
;
    break;}
case 246:
#line 1835 "dtool/src/cppparser/cppbison.yxx"
{
  assert(current_enum != NULL);
  current_enum->add_element(yyvsp[-2].u.identifier->get_simple_name(), current_scope, yyvsp[0].u.expr);
;
    break;}
case 247:
#line 1840 "dtool/src/cppparser/cppbison.yxx"
{
  assert(current_enum != NULL);
  current_enum->add_element(yyvsp[0].u.identifier->get_simple_name(), current_scope);
;
    break;}
case 248:
#line 1845 "dtool/src/cppparser/cppbison.yxx"
{
  assert(current_enum != NULL);
  current_enum->add_element(yyvsp[-2].u.identifier->get_simple_name(), current_scope, yyvsp[0].u.expr);
;
    break;}
case 249:
#line 1853 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.extension_enum = CPPExtensionType::T_enum;
;
    break;}
case 250:
#line 1860 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.extension_enum = CPPExtensionType::T_class;
;
    break;}
case 251:
#line 1864 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.extension_enum = CPPExtensionType::T_struct;
;
    break;}
case 252:
#line 1868 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.extension_enum = CPPExtensionType::T_union;
;
    break;}
case 253:
#line 1875 "dtool/src/cppparser/cppbison.yxx"
{
  CPPScope *scope = yyvsp[-1].u.identifier->find_scope(current_scope, global_scope, current_lexer);
  if (scope == NULL) {
    // This must be a new namespace declaration.
    CPPScope *parent_scope = 
      yyvsp[-1].u.identifier->get_scope(current_scope, global_scope, current_lexer);
    if (parent_scope == NULL) {
      parent_scope = current_scope;
    }
    scope = new CPPScope(parent_scope, yyvsp[-1].u.identifier->_names.back(), V_public);
  }

  CPPNamespace *nspace = new CPPNamespace(yyvsp[-1].u.identifier, scope, yylsp[-2].file);
  current_scope->add_declaration(nspace, global_scope, current_lexer, yylsp[-2]);
  current_scope->define_namespace(nspace);
  push_scope(scope);
;
    break;}
case 254:
#line 1893 "dtool/src/cppparser/cppbison.yxx"
{
  pop_scope();
;
    break;}
case 256:
#line 1901 "dtool/src/cppparser/cppbison.yxx"
{
  CPPUsing *using_decl = new CPPUsing(yyvsp[0].u.identifier, false, yylsp[-1].file);
  current_scope->add_declaration(using_decl, global_scope, current_lexer, yylsp[-1]);
  current_scope->add_using(using_decl, global_scope, current_lexer);
;
    break;}
case 257:
#line 1907 "dtool/src/cppparser/cppbison.yxx"
{
  CPPUsing *using_decl = new CPPUsing(yyvsp[0].u.identifier, true, yylsp[-2].file);
  current_scope->add_declaration(using_decl, global_scope, current_lexer, yylsp[-2]);
  current_scope->add_using(using_decl, global_scope, current_lexer);
;
    break;}
case 261:
#line 1922 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.simple_type = new CPPSimpleType(CPPSimpleType::T_bool);
;
    break;}
case 262:
#line 1926 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.simple_type = new CPPSimpleType(CPPSimpleType::T_char);
;
    break;}
case 263:
#line 1930 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.simple_type = new CPPSimpleType(CPPSimpleType::T_wchar_t);
;
    break;}
case 264:
#line 1934 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.simple_type = new CPPSimpleType(CPPSimpleType::T_int,
                         CPPSimpleType::F_short);
;
    break;}
case 265:
#line 1939 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.simple_type = new CPPSimpleType(CPPSimpleType::T_int,
                         CPPSimpleType::F_long);
;
    break;}
case 266:
#line 1944 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.simple_type = new CPPSimpleType(CPPSimpleType::T_int,
                         CPPSimpleType::F_longlong);
;
    break;}
case 267:
#line 1949 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.simple_type = new CPPSimpleType(CPPSimpleType::T_int,
                         CPPSimpleType::F_unsigned);
;
    break;}
case 268:
#line 1954 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.simple_type = new CPPSimpleType(CPPSimpleType::T_int,
                         CPPSimpleType::F_signed);
;
    break;}
case 269:
#line 1959 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.simple_type = new CPPSimpleType(CPPSimpleType::T_int);
;
    break;}
case 270:
#line 1963 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.simple_type = yyvsp[0].u.simple_type;
  yyval.u.simple_type->_flags |= CPPSimpleType::F_short;
;
    break;}
case 271:
#line 1968 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.simple_type = yyvsp[0].u.simple_type;
  if (yyval.u.simple_type->_flags & CPPSimpleType::F_long) {
    yyval.u.simple_type->_flags |= CPPSimpleType::F_longlong;
  } else {
    yyval.u.simple_type->_flags |= CPPSimpleType::F_long;
  }
;
    break;}
case 272:
#line 1977 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.simple_type = yyvsp[0].u.simple_type;
  yyval.u.simple_type->_flags |= CPPSimpleType::F_unsigned;
;
    break;}
case 273:
#line 1982 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.simple_type = yyvsp[0].u.simple_type;
  yyval.u.simple_type->_flags |= CPPSimpleType::F_signed;
;
    break;}
case 274:
#line 1990 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.simple_type = new CPPSimpleType(CPPSimpleType::T_float);
;
    break;}
case 275:
#line 1994 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.simple_type = new CPPSimpleType(CPPSimpleType::T_float,
                         CPPSimpleType::F_long);
;
    break;}
case 276:
#line 1999 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.simple_type = new CPPSimpleType(CPPSimpleType::T_double);
;
    break;}
case 277:
#line 2003 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.simple_type = new CPPSimpleType(CPPSimpleType::T_float,
                         CPPSimpleType::F_longlong);
;
    break;}
case 278:
#line 2008 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.simple_type = new CPPSimpleType(CPPSimpleType::T_double,
                         CPPSimpleType::F_long);
;
    break;}
case 279:
#line 2016 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.simple_type = new CPPSimpleType(CPPSimpleType::T_void);
;
    break;}
case 280:
#line 2025 "dtool/src/cppparser/cppbison.yxx"
{
  current_lexer->_resolve_identifiers = false;
;
    break;}
case 281:
#line 2029 "dtool/src/cppparser/cppbison.yxx"
{
  current_lexer->_resolve_identifiers = true;
;
    break;}
case 284:
#line 2041 "dtool/src/cppparser/cppbison.yxx"
{
;
    break;}
case 285:
#line 2044 "dtool/src/cppparser/cppbison.yxx"
{
;
    break;}
case 286:
#line 2047 "dtool/src/cppparser/cppbison.yxx"
{
;
    break;}
case 287:
#line 2050 "dtool/src/cppparser/cppbison.yxx"
{
;
    break;}
case 288:
#line 2053 "dtool/src/cppparser/cppbison.yxx"
{
;
    break;}
case 289:
#line 2056 "dtool/src/cppparser/cppbison.yxx"
{
;
    break;}
case 290:
#line 2059 "dtool/src/cppparser/cppbison.yxx"
{
;
    break;}
case 291:
#line 2062 "dtool/src/cppparser/cppbison.yxx"
{
;
    break;}
case 365:
#line 2082 "dtool/src/cppparser/cppbison.yxx"
{
;
    break;}
case 389:
#line 2091 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = (CPPExpression *)NULL;
;
    break;}
case 390:
#line 2095 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = yyvsp[0].u.expr;
;
    break;}
case 391:
#line 2102 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = (CPPExpression *)NULL;
;
    break;}
case 392:
#line 2106 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = yyvsp[0].u.expr;
;
    break;}
case 393:
#line 2113 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = yyvsp[0].u.expr;
;
    break;}
case 394:
#line 2117 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(',', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 395:
#line 2124 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = yyvsp[0].u.expr;
;
    break;}
case 396:
#line 2128 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(CPPExpression::typecast_op(yyvsp[-2].u.type, yyvsp[0].u.expr));
;
    break;}
case 397:
#line 2132 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(CPPExpression::typecast_op(yyvsp[-4].u.type, yyvsp[-1].u.expr));
;
    break;}
case 398:
#line 2136 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(CPPExpression::typecast_op(yyvsp[-4].u.type, yyvsp[-1].u.expr));
;
    break;}
case 399:
#line 2140 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(CPPExpression::sizeof_func(yyvsp[-1].u.type));
;
    break;}
case 400:
#line 2144 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(UNARY_NOT, yyvsp[0].u.expr);
;
    break;}
case 401:
#line 2148 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(UNARY_NEGATE, yyvsp[0].u.expr);
;
    break;}
case 402:
#line 2152 "dtool/src/cppparser/cppbison.yxx"
{
  if (yyvsp[0].u.expr->_type == CPPExpression::T_integer) {
    yyval.u.expr = yyvsp[0].u.expr;
    yyval.u.expr->_u._integer = -yyval.u.expr->_u._integer;
  } else if (yyvsp[0].u.expr->_type == CPPExpression::T_real) {
    yyval.u.expr = yyvsp[0].u.expr;
    yyval.u.expr->_u._real = -yyval.u.expr->_u._real;
  } else {
    yyval.u.expr = new CPPExpression(UNARY_MINUS, yyvsp[0].u.expr);
  }
;
    break;}
case 403:
#line 2164 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(UNARY_STAR, yyvsp[0].u.expr);
;
    break;}
case 404:
#line 2168 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(UNARY_REF, yyvsp[0].u.expr);
;
    break;}
case 405:
#line 2172 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('*', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 406:
#line 2176 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('/', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 407:
#line 2180 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('%', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 408:
#line 2184 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('+', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 409:
#line 2188 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('-', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 410:
#line 2192 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('|', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 411:
#line 2196 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('^', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 412:
#line 2200 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('&', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 413:
#line 2204 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(OROR, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 414:
#line 2208 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(ANDAND, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 415:
#line 2212 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(EQCOMPARE, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 416:
#line 2216 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(NECOMPARE, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 417:
#line 2220 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(LECOMPARE, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 418:
#line 2224 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(GECOMPARE, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 419:
#line 2228 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(LSHIFT, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 420:
#line 2232 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(RSHIFT, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 421:
#line 2236 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('?', yyvsp[-4].u.expr, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 422:
#line 2240 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('[', yyvsp[-3].u.expr, yyvsp[-1].u.expr);
;
    break;}
case 423:
#line 2244 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('f', yyvsp[-3].u.expr, yyvsp[-1].u.expr);
;
    break;}
case 424:
#line 2248 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('f', yyvsp[-2].u.expr);
;
    break;}
case 425:
#line 2252 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('.', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 426:
#line 2256 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(POINTSAT, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 427:
#line 2260 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = yyvsp[-1].u.expr;
;
    break;}
case 428:
#line 2268 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = yyvsp[0].u.expr;
;
    break;}
case 429:
#line 2272 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(CPPExpression::typecast_op(yyvsp[-2].u.type, yyvsp[0].u.expr));
;
    break;}
case 430:
#line 2276 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(CPPExpression::typecast_op(yyvsp[-4].u.type, yyvsp[-1].u.expr));
;
    break;}
case 431:
#line 2280 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(CPPExpression::typecast_op(yyvsp[-4].u.type, yyvsp[-1].u.expr));
;
    break;}
case 432:
#line 2284 "dtool/src/cppparser/cppbison.yxx"
{
  // A constructor call.
  CPPType *type = yyvsp[-3].u.identifier->find_type(current_scope, global_scope, false, current_lexer);
  assert(type != NULL);
  yyval.u.expr = new CPPExpression(CPPExpression::construct_op(type, yyvsp[-1].u.expr));
;
    break;}
case 433:
#line 2291 "dtool/src/cppparser/cppbison.yxx"
{
  CPPType *type = 
    CPPType::new_type(new CPPSimpleType(CPPSimpleType::T_int));
  yyval.u.expr = new CPPExpression(CPPExpression::construct_op(type, yyvsp[-1].u.expr));
;
    break;}
case 434:
#line 2297 "dtool/src/cppparser/cppbison.yxx"
{
  CPPType *type = 
    CPPType::new_type(new CPPSimpleType(CPPSimpleType::T_char));
  yyval.u.expr = new CPPExpression(CPPExpression::construct_op(type, yyvsp[-1].u.expr));
;
    break;}
case 435:
#line 2303 "dtool/src/cppparser/cppbison.yxx"
{
  CPPType *type = 
    CPPType::new_type(new CPPSimpleType(CPPSimpleType::T_wchar_t));
  yyval.u.expr = new CPPExpression(CPPExpression::construct_op(type, yyvsp[-1].u.expr));
;
    break;}
case 436:
#line 2309 "dtool/src/cppparser/cppbison.yxx"
{
  CPPType *type = 
    CPPType::new_type(new CPPSimpleType(CPPSimpleType::T_bool));
  yyval.u.expr = new CPPExpression(CPPExpression::construct_op(type, yyvsp[-1].u.expr));
;
    break;}
case 437:
#line 2315 "dtool/src/cppparser/cppbison.yxx"
{
  CPPType *type = 
    CPPType::new_type(new CPPSimpleType(CPPSimpleType::T_int,
                                        CPPSimpleType::F_short));
  yyval.u.expr = new CPPExpression(CPPExpression::construct_op(type, yyvsp[-1].u.expr));
;
    break;}
case 438:
#line 2322 "dtool/src/cppparser/cppbison.yxx"
{
  CPPType *type = 
    CPPType::new_type(new CPPSimpleType(CPPSimpleType::T_int,
                                        CPPSimpleType::F_long));
  yyval.u.expr = new CPPExpression(CPPExpression::construct_op(type, yyvsp[-1].u.expr));
;
    break;}
case 439:
#line 2329 "dtool/src/cppparser/cppbison.yxx"
{
  CPPType *type = 
    CPPType::new_type(new CPPSimpleType(CPPSimpleType::T_int,
                                        CPPSimpleType::F_unsigned));
  yyval.u.expr = new CPPExpression(CPPExpression::construct_op(type, yyvsp[-1].u.expr));
;
    break;}
case 440:
#line 2336 "dtool/src/cppparser/cppbison.yxx"
{
  CPPType *type = 
    CPPType::new_type(new CPPSimpleType(CPPSimpleType::T_int,
                                        CPPSimpleType::F_signed));
  yyval.u.expr = new CPPExpression(CPPExpression::construct_op(type, yyvsp[-1].u.expr));
;
    break;}
case 441:
#line 2343 "dtool/src/cppparser/cppbison.yxx"
{
  CPPType *type = 
    CPPType::new_type(new CPPSimpleType(CPPSimpleType::T_float));
  yyval.u.expr = new CPPExpression(CPPExpression::construct_op(type, yyvsp[-1].u.expr));
;
    break;}
case 442:
#line 2349 "dtool/src/cppparser/cppbison.yxx"
{
  CPPType *type = 
    CPPType::new_type(new CPPSimpleType(CPPSimpleType::T_double));
  yyval.u.expr = new CPPExpression(CPPExpression::construct_op(type, yyvsp[-1].u.expr));
;
    break;}
case 443:
#line 2355 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(CPPExpression::sizeof_func(yyvsp[-1].u.type));
;
    break;}
case 444:
#line 2359 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(CPPExpression::new_op(yyvsp[0].u.type));
;
    break;}
case 445:
#line 2363 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(CPPExpression::new_op(yyvsp[-3].u.type, yyvsp[-1].u.expr));
;
    break;}
case 446:
#line 2367 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(UNARY_NOT, yyvsp[0].u.expr);
;
    break;}
case 447:
#line 2371 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(UNARY_NEGATE, yyvsp[0].u.expr);
;
    break;}
case 448:
#line 2375 "dtool/src/cppparser/cppbison.yxx"
{
  if (yyvsp[0].u.expr->_type == CPPExpression::T_integer) {
    yyval.u.expr = yyvsp[0].u.expr;
    yyval.u.expr->_u._integer = -yyval.u.expr->_u._integer;
  } else if (yyvsp[0].u.expr->_type == CPPExpression::T_real) {
    yyval.u.expr = yyvsp[0].u.expr;
    yyval.u.expr->_u._real = -yyval.u.expr->_u._real;
  } else {
    yyval.u.expr = new CPPExpression(UNARY_MINUS, yyvsp[0].u.expr);
  }
;
    break;}
case 449:
#line 2387 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(UNARY_STAR, yyvsp[0].u.expr);
;
    break;}
case 450:
#line 2391 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(UNARY_REF, yyvsp[0].u.expr);
;
    break;}
case 451:
#line 2395 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('*', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 452:
#line 2399 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('/', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 453:
#line 2403 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('%', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 454:
#line 2407 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('+', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 455:
#line 2411 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('-', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 456:
#line 2415 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('|', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 457:
#line 2419 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('^', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 458:
#line 2423 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('&', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 459:
#line 2427 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(OROR, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 460:
#line 2431 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(ANDAND, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 461:
#line 2435 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(EQCOMPARE, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 462:
#line 2439 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(NECOMPARE, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 463:
#line 2443 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(LECOMPARE, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 464:
#line 2447 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(GECOMPARE, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 465:
#line 2451 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('<', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 466:
#line 2455 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('>', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 467:
#line 2459 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(LSHIFT, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 468:
#line 2463 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(RSHIFT, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 469:
#line 2467 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('?', yyvsp[-4].u.expr, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 470:
#line 2471 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('[', yyvsp[-3].u.expr, yyvsp[-1].u.expr);
;
    break;}
case 471:
#line 2475 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('f', yyvsp[-3].u.expr, yyvsp[-1].u.expr);
;
    break;}
case 472:
#line 2479 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('f', yyvsp[-2].u.expr);
;
    break;}
case 473:
#line 2483 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('.', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 474:
#line 2487 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(POINTSAT, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 475:
#line 2491 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = yyvsp[-1].u.expr;
;
    break;}
case 476:
#line 2498 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(yyvsp[0].u.integer);
;
    break;}
case 477:
#line 2502 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(true);
;
    break;}
case 478:
#line 2506 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(false);
;
    break;}
case 479:
#line 2510 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(yyvsp[0].u.integer);
;
    break;}
case 480:
#line 2514 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(yyvsp[0].u.real);
;
    break;}
case 481:
#line 2518 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(yyvsp[0].str);
;
    break;}
case 482:
#line 2522 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(yyvsp[0].u.identifier, current_scope, global_scope, current_lexer);
;
    break;}
case 483:
#line 2536 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = yyvsp[0].u.expr;
;
    break;}
case 484:
#line 2540 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(CPPExpression::typecast_op(yyvsp[-2].u.type, yyvsp[0].u.expr));
;
    break;}
case 485:
#line 2544 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(CPPExpression::typecast_op(yyvsp[-4].u.type, yyvsp[-1].u.expr));
;
    break;}
case 486:
#line 2548 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(CPPExpression::typecast_op(yyvsp[-4].u.type, yyvsp[-1].u.expr));
;
    break;}
case 487:
#line 2552 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(CPPExpression::sizeof_func(yyvsp[-1].u.type));
;
    break;}
case 488:
#line 2556 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(CPPExpression::new_op(yyvsp[0].u.type));
;
    break;}
case 489:
#line 2560 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(CPPExpression::new_op(yyvsp[-3].u.type, yyvsp[-1].u.expr));
;
    break;}
case 490:
#line 2564 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(UNARY_NOT, yyvsp[0].u.expr);
;
    break;}
case 491:
#line 2568 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(UNARY_NEGATE, yyvsp[0].u.expr);
;
    break;}
case 492:
#line 2572 "dtool/src/cppparser/cppbison.yxx"
{
  if (yyvsp[0].u.expr->_type == CPPExpression::T_integer) {
    yyval.u.expr = yyvsp[0].u.expr;
    yyval.u.expr->_u._integer = -yyval.u.expr->_u._integer;
  } else if (yyvsp[0].u.expr->_type == CPPExpression::T_real) {
    yyval.u.expr = yyvsp[0].u.expr;
    yyval.u.expr->_u._real = -yyval.u.expr->_u._real;
  } else {
    yyval.u.expr = new CPPExpression(UNARY_MINUS, yyvsp[0].u.expr);
  }
;
    break;}
case 493:
#line 2584 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(UNARY_REF, yyvsp[0].u.expr);
;
    break;}
case 494:
#line 2588 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('*', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 495:
#line 2592 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('/', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 496:
#line 2596 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('%', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 497:
#line 2600 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('+', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 498:
#line 2604 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('-', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 499:
#line 2608 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('|', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 500:
#line 2612 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('^', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 501:
#line 2616 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('&', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 502:
#line 2620 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(OROR, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 503:
#line 2624 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(ANDAND, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 504:
#line 2628 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(EQCOMPARE, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 505:
#line 2632 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(NECOMPARE, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 506:
#line 2636 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(LECOMPARE, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 507:
#line 2640 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(GECOMPARE, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 508:
#line 2644 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('<', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 509:
#line 2648 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('>', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 510:
#line 2652 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(LSHIFT, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 511:
#line 2656 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(RSHIFT, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 512:
#line 2660 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('?', yyvsp[-4].u.expr, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 513:
#line 2664 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('[', yyvsp[-3].u.expr, yyvsp[-1].u.expr);
;
    break;}
case 514:
#line 2668 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('f', yyvsp[-3].u.expr, yyvsp[-1].u.expr);
;
    break;}
case 515:
#line 2672 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('f', yyvsp[-2].u.expr);
;
    break;}
case 516:
#line 2676 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression('.', yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 517:
#line 2680 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(POINTSAT, yyvsp[-2].u.expr, yyvsp[0].u.expr);
;
    break;}
case 518:
#line 2684 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = yyvsp[-1].u.expr;
;
    break;}
case 519:
#line 2691 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(yyvsp[0].u.integer);
;
    break;}
case 520:
#line 2695 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(true);
;
    break;}
case 521:
#line 2699 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(false);
;
    break;}
case 522:
#line 2703 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(yyvsp[0].u.integer);
;
    break;}
case 523:
#line 2707 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(yyvsp[0].u.real);
;
    break;}
case 524:
#line 2711 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.expr = new CPPExpression(yyvsp[0].str);
;
    break;}
case 525:
#line 2718 "dtool/src/cppparser/cppbison.yxx"
{
  CPPType *type = yyvsp[0].u.identifier->find_type(current_scope, global_scope, true);
  if (type == NULL) {
    type = CPPType::new_type(new CPPTBDType(yyvsp[0].u.identifier));
  }
  yyval.u.type = type;
;
    break;}
case 526:
#line 2726 "dtool/src/cppparser/cppbison.yxx"
{
  CPPType *type = yyvsp[0].u.identifier->find_type(current_scope, global_scope, true, current_lexer);
  if (type == NULL) {
    type = CPPType::new_type(new CPPTBDType(yyvsp[0].u.identifier));
  }
  yyval.u.type = type;
;
    break;}
case 527:
#line 2734 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.type = CPPType::new_type(new CPPTBDType(yyvsp[0].u.identifier));
;
    break;}
case 528:
#line 2757 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.identifier = yyvsp[0].u.identifier;
;
    break;}
case 529:
#line 2761 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.u.identifier = yyvsp[0].u.identifier;
;
    break;}
case 530:
#line 2768 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = yyvsp[0].str;
;
    break;}
case 531:
#line 2772 "dtool/src/cppparser/cppbison.yxx"
{
  yyval.str = yyvsp[-1].str + yyvsp[0].str;
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
#line 2779 "dtool/src/cppparser/cppbison.yxx"
