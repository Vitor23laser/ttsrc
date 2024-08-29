
#include "dtoolbase.h"
#include "interrogate_request.h"

#undef _POSIX_C_SOURCE
#include "py_panda.h"

extern LibrayDef libvision_moddef ;
#ifdef _WIN32
extern "C" __declspec(dllexport) void initlibp3vision();
#else
extern "C" void initlibp3vision();
#endif

void initlibp3vision() 
{
  LibrayDef   *defs[] = {&libvision_moddef, NULL };
   Dtool_PyModuleInitHelper( defs, "libp3vision");



}
