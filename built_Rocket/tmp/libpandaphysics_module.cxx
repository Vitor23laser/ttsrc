
#include "dtoolbase.h"
#include "interrogate_request.h"

#undef _POSIX_C_SOURCE
#include "py_panda.h"

extern LibrayDef libparticlesystem_moddef ;
extern LibrayDef libphysics_moddef ;
#ifdef _WIN32
extern "C" __declspec(dllexport) void initlibpandaphysics();
#else
extern "C" void initlibpandaphysics();
#endif

void initlibpandaphysics() 
{
  LibrayDef   *defs[] = {&libparticlesystem_moddef,&libphysics_moddef, NULL };
   Dtool_PyModuleInitHelper( defs, "libpandaphysics");



}
