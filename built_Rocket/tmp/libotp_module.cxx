
#include "dtoolbase.h"
#include "interrogate_request.h"

#undef _POSIX_C_SOURCE
#include "py_panda.h"

extern LibrayDef libmovement_moddef ;
extern LibrayDef libnametag_moddef ;
extern LibrayDef libnavigation_moddef ;
extern LibrayDef libsecure_moddef ;
extern LibrayDef libsettings_moddef ;
#ifdef _WIN32
extern "C" __declspec(dllexport) void initlibotp();
#else
extern "C" void initlibotp();
#endif

void initlibotp() 
{
  LibrayDef   *defs[] = {&libmovement_moddef,&libnametag_moddef,&libnavigation_moddef,&libsecure_moddef,&libsettings_moddef, NULL };
   Dtool_PyModuleInitHelper( defs, "libotp");



}
