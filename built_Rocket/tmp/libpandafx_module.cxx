
#include "dtoolbase.h"
#include "interrogate_request.h"

#undef _POSIX_C_SOURCE
#include "py_panda.h"

extern LibrayDef libdistort_moddef ;
#ifdef _WIN32
extern "C" __declspec(dllexport) void initlibpandafx();
#else
extern "C" void initlibpandafx();
#endif

void initlibpandafx() 
{
  LibrayDef   *defs[] = {&libdistort_moddef, NULL };
   Dtool_PyModuleInitHelper( defs, "libpandafx");



}
