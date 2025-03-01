// Filename: load_dso.cxx
// Created by:  drose (12May00)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//
////////////////////////////////////////////////////////////////////

#include "load_dso.h"

static Filename resolve_dso(const DSearchPath &path, const Filename &filename) {
  if (filename.is_local()) {
    if ((path.get_num_directories()==1)&&(path.get_directory(0)=="<auto>")) {
      // This is a special case, meaning to search in the same
      // directory in which libp3dtool.dll, or the exe, was started
      // from.
      Filename dtoolpath = ExecutionEnvironment::get_dtool_name();
      DSearchPath spath(dtoolpath.get_dirname());
      return spath.find_file(filename);
    } else {
      return path.find_file(filename);
    }
  } else {
    return filename;
  }
}

#if defined(WIN32)
/* begin Win32-specific code */

#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#undef WINDOWS_LEAN_AND_MEAN

// Loads in a dynamic library like an .so or .dll.  Returns NULL if
// failure, otherwise on success.  If the filename is not absolute,
// searches the path.  If the path is empty, searches the dtool
// directory.

void *
load_dso(const DSearchPath &path, const Filename &filename) {
  Filename abspath = resolve_dso(path, filename);
  if (!abspath.is_regular_file()) {
    return NULL;
  }
  string os_specific = abspath.to_os_specific();
  
  // Try using LoadLibraryEx, if possible.
  typedef HMODULE (WINAPI *tLoadLibraryEx)(LPCTSTR, HANDLE, DWORD);
  tLoadLibraryEx pLoadLibraryEx;
  HINSTANCE hLib = LoadLibrary("kernel32.dll");
  if (hLib) {
    pLoadLibraryEx = (tLoadLibraryEx)GetProcAddress(hLib, "LoadLibraryExA");
    if (pLoadLibraryEx) {
      return pLoadLibraryEx(os_specific.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
    }
  }
  
  return LoadLibrary(os_specific.c_str());
}

bool
unload_dso(void *dso_handle) {
  HMODULE dll_handle = (HMODULE) dso_handle;

  // true indicates success
  return (FreeLibrary(dll_handle)!=0);
}

string
load_dso_error() {
  DWORD last_error = GetLastError();

  /*
  LPVOID ptr;
  if (FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |
                     FORMAT_MESSAGE_FROM_SYSTEM,
                     NULL,
                     last_error,
                     MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ),
                     (LPTSTR)&ptr,
                     0, NULL))
    {
      cout << "ERROR: " << " result = " << (char*) ptr << "\n";
      LocalFree( ptr );
    }
  */

  switch (last_error) {
    case 2: return "File not found";
    case 3: return "Path not found";
    case 4: return "Too many open files";
    case 5: return "Access denied";
    case 14: return "Out of memory";
    case 18: return "No more files";
    case 126: return "Module not found";
    case 127: return "The specified procedure could not be found";
    case 193: return "Not a valid Win32 application";
    case 998: return "Invalid access to memory location";
  }

  // Some unknown error code.
  ostringstream errmsg;
  errmsg << "Unknown error " << last_error;
  return errmsg.str();
}

void *
get_dso_symbol(void *handle, const string &name) {
  // Windows puts a leading underscore in front of the symbol name.
  return (void *)GetProcAddress((HMODULE)handle, name.c_str());
}

/* end Win32-specific code */

#elif defined(__SWITCH__)
/* begin Switch code */

void *
load_dso(const DSearchPath &path, const Filename &filename) {
  return NULL;
}

bool
unload_dso(void *dso_handle) {
  return false;
}

string
load_dso_error() {
   return "Not supported.";
}

void *
get_dso_symbol(void *handle, const string &name) {
  return NULL;
}


/* end Switch code */

#else
/* begin Posix code */

#if defined(IS_OSX)
#include <mach-o/dyld.h>
#endif

#include <dlfcn.h>

void *
load_dso(const DSearchPath &path, const Filename &filename) {
  Filename abspath = resolve_dso(path, filename);
  if (!abspath.is_regular_file()) {
    return NULL;
  }
  string os_specific = abspath.to_os_specific();
  return dlopen(os_specific.c_str(), RTLD_NOW | RTLD_GLOBAL);
}

bool
unload_dso(void *dso_handle) {
  return dlclose(dso_handle)==0;
}

string
load_dso_error() {
   const char *message = dlerror();
   if (message != (const char *)NULL) {
    return std::string(message);
  }
  return "No error.";
}

void *
get_dso_symbol(void *handle, const string &name) {
  return dlsym(handle, name.c_str());
}

#endif
