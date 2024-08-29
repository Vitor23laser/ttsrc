
#include "dtoolbase.h"
#include "interrogate_request.h"

#undef _POSIX_C_SOURCE
#include "py_panda.h"

extern LibrayDef libdna_moddef ;
extern LibrayDef libpets_moddef ;
extern LibrayDef libsuit_moddef ;
#ifdef _WIN32
extern "C" __declspec(dllexport) void initlibtoontown();
#else
extern "C" void initlibtoontown();
#endif

void initlibtoontown() 
{
  LibrayDef   *defs[] = {&libdna_moddef,&libpets_moddef,&libsuit_moddef, NULL };
   Dtool_PyModuleInitHelper( defs, "libtoontown");



}
