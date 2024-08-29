
#include "dtoolbase.h"
#include "interrogate_request.h"

#undef _POSIX_C_SOURCE
#include "py_panda.h"

extern LibrayDef libdcparser_moddef ;
extern LibrayDef libdeadrec_moddef ;
extern LibrayDef libdistributed_moddef ;
extern LibrayDef libhttp_moddef ;
extern LibrayDef libinterval_moddef ;
extern LibrayDef libmotiontrail_moddef ;
extern LibrayDef libshowbase_moddef ;
#ifdef _WIN32
extern "C" __declspec(dllexport) void initlibp3direct();
#else
extern "C" void initlibp3direct();
#endif

void initlibp3direct() 
{
  LibrayDef   *defs[] = {&libdcparser_moddef,&libdeadrec_moddef,&libdistributed_moddef,&libhttp_moddef,&libinterval_moddef,&libmotiontrail_moddef,&libshowbase_moddef, NULL };
   Dtool_PyModuleInitHelper( defs, "libp3direct");



}
