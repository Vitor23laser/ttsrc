
#include "dtoolbase.h"
#include "interrogate_request.h"

#undef _POSIX_C_SOURCE
#include "py_panda.h"

extern LibrayDef libpandaode_moddef ;
#ifdef _WIN32
extern "C" __declspec(dllexport) void initlibpandaode();
#else
extern "C" void initlibpandaode();
#endif

void initlibpandaode() 
{
  LibrayDef   *defs[] = {&libpandaode_moddef, NULL };
   Dtool_PyModuleInitHelper( defs, "libpandaode");



}
