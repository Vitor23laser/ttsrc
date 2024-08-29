
#include "dtoolbase.h"
#include "interrogate_request.h"

#undef _POSIX_C_SOURCE
#include "py_panda.h"

extern LibrayDef libdownloader_moddef ;
extern LibrayDef libexpress_moddef ;
#ifdef _WIN32
extern "C" __declspec(dllexport) void initlibpandaexpress();
#else
extern "C" void initlibpandaexpress();
#endif

void initlibpandaexpress() 
{
  LibrayDef   *defs[] = {&libdownloader_moddef,&libexpress_moddef, NULL };
   Dtool_PyModuleInitHelper( defs, "libpandaexpress");



}
