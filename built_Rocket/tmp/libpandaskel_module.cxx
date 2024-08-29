
#include "dtoolbase.h"
#include "interrogate_request.h"

#undef _POSIX_C_SOURCE
#include "py_panda.h"

extern LibrayDef libskel_moddef ;
#ifdef _WIN32
extern "C" __declspec(dllexport) void initlibpandaskel();
#else
extern "C" void initlibpandaskel();
#endif

void initlibpandaskel() 
{
  LibrayDef   *defs[] = {&libskel_moddef, NULL };
   Dtool_PyModuleInitHelper( defs, "libpandaskel");



}
