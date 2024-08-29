
#include "dtoolbase.h"
#include "interrogate_request.h"

#undef _POSIX_C_SOURCE
#include "py_panda.h"

extern LibrayDef libaudio_moddef ;
extern LibrayDef libchan_moddef ;
extern LibrayDef libchar_moddef ;
extern LibrayDef libcollide_moddef ;
extern LibrayDef libdevice_moddef ;
extern LibrayDef libdgraph_moddef ;
extern LibrayDef libdisplay_moddef ;
extern LibrayDef libdxml_moddef ;
extern LibrayDef libevent_moddef ;
extern LibrayDef libgobj_moddef ;
extern LibrayDef libgrutil_moddef ;
extern LibrayDef libgsgbase_moddef ;
extern LibrayDef liblerp_moddef ;
extern LibrayDef liblinmath_moddef ;
extern LibrayDef libmathutil_moddef ;
extern LibrayDef libmovies_moddef ;
extern LibrayDef libnativenet_moddef ;
extern LibrayDef libnet_moddef ;
extern LibrayDef libparametrics_moddef ;
extern LibrayDef libpgraph_moddef ;
extern LibrayDef libpgraphnodes_moddef ;
extern LibrayDef libpgui_moddef ;
extern LibrayDef libpipeline_moddef ;
extern LibrayDef libpnmimage_moddef ;
extern LibrayDef libpnmtext_moddef ;
extern LibrayDef libpstatclient_moddef ;
extern LibrayDef libputil_moddef ;
extern LibrayDef librecorder_moddef ;
extern LibrayDef libtext_moddef ;
extern LibrayDef libtform_moddef ;
extern LibrayDef libvrpn_moddef ;
#ifdef _WIN32
extern "C" __declspec(dllexport) void initlibpanda();
#else
extern "C" void initlibpanda();
#endif

void initlibpanda() 
{
  LibrayDef   *defs[] = {&libaudio_moddef,&libchan_moddef,&libchar_moddef,&libcollide_moddef,&libdevice_moddef,&libdgraph_moddef,&libdisplay_moddef,&libdxml_moddef,&libevent_moddef,&libgobj_moddef,&libgrutil_moddef,&libgsgbase_moddef,&liblerp_moddef,&liblinmath_moddef,&libmathutil_moddef,&libmovies_moddef,&libnativenet_moddef,&libnet_moddef,&libparametrics_moddef,&libpgraph_moddef,&libpgraphnodes_moddef,&libpgui_moddef,&libpipeline_moddef,&libpnmimage_moddef,&libpnmtext_moddef,&libpstatclient_moddef,&libputil_moddef,&librecorder_moddef,&libtext_moddef,&libtform_moddef,&libvrpn_moddef, NULL };
   Dtool_PyModuleInitHelper( defs, "libpanda");



}
