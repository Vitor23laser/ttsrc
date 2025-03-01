// Filename: virtualFileSystem.cxx
// Created by:  drose (03Aug02)
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

#include "virtualFileSystem.h"
#include "virtualFileSimple.h"
#include "virtualFileComposite.h"
#include "virtualFileMount.h"
#include "virtualFileMountMultifile.h"
#include "virtualFileMountSystem.h"
#include "streamWrapper.h"
#include "dSearchPath.h"
#include "dcast.h"
#include "config_express.h"
#include "executionEnvironment.h"
#include "pset.h"

VirtualFileSystem *VirtualFileSystem::_global_ptr = NULL;


////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::Constructor
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
VirtualFileSystem::
VirtualFileSystem() :
  vfs_case_sensitive
("vfs-case-sensitive", 
#ifdef NDEBUG
 false,  // The default for a production build is not case-sensitive;
         // this avoids runtime overhead to verify case sensitivity.
#else
 true,
#endif
 PRC_DESC("Set this true to make the VirtualFileSystem present the native "
          "OS-provided filesystem as if it were a case-sensitive file "
          "system, even if it is not (e.g. on Windows).  This variable "
          "has no effect if the native filesystem is already case-sensitive, "
          "and it has no effect on mounted multifile systems, which are "
          "always case-sensitive.")),
  vfs_implicit_pz
  ("vfs-implicit-pz", true,
   PRC_DESC("When this is true, the VirtualFileSystem will pretend a named "
            "file exists even if it doesn't, as long as a filename with the "
            "same name and the additional extension .pz does exist.  In this "
            "case, the VirtualFileSystem will implicitly open the .pz file "
            "and decompress it on-the-fly.")),
  vfs_implicit_mf
  ("vfs-implicit-mf", false,
   PRC_DESC("When this is true, the VirtualFileSystem will automatically "
            "mount multifiles on-the-fly when they are used as directories.  "
            "For instance, opening the file /c/files/foo.mf/dirname/mytex.jpg "
            "will implicitly retrieve a file named 'dirname/mytex.jpg' "
            "within the multifile /c/files/foo.mf, even if the multifile "
            "has not already been mounted.  This makes all of your multifiles "
            "act like directories."))
{
  _cwd = "/";
  _mount_seq = 0;
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::Destructor
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
VirtualFileSystem::
~VirtualFileSystem() {
  unmount_all();
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::mount
//       Access: Published
//  Description: Mounts the indicated Multifile at the given mount
//               point.
////////////////////////////////////////////////////////////////////
bool VirtualFileSystem::
mount(Multifile *multifile, const Filename &mount_point, int flags) {
  PT(VirtualFileMountMultifile) new_mount = 
    new VirtualFileMountMultifile(multifile);
  return mount(new_mount, mount_point, flags);
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::mount
//       Access: Published
//  Description: Mounts the indicated system file or directory at the
//               given mount point.  If the named file is a directory,
//               mounts the directory.  If the named file is a
//               Multifile, mounts it as a Multifile.  Returns true on
//               success, false on failure.
//
//               A given system directory may be mounted to multiple
//               different mount point, and the same mount point may
//               share multiple system directories.  In the case of
//               ambiguities (that is, two different files with
//               exactly the same full pathname), the most-recently
//               mounted system wins.
//
//               The filename specified as the first parameter must
//               refer to a real, physical filename on disk; it cannot
//               be a virtual file already appearing within the vfs
//               filespace.  However, it is possible to mount such a
//               file; see mount_loop() for this.
////
//               Note that a mounted VirtualFileSystem directory is
//               fully case-sensitive, unlike the native Windows file
//               system, so you must refer to files within the virtual
//               file system with exactly the right case.
////////////////////////////////////////////////////////////////////
bool VirtualFileSystem::
mount(const Filename &physical_filename, const Filename &mount_point, 
      int flags, const string &password) {
  if (!physical_filename.exists()) {
    express_cat->warning()
      << "Attempt to mount " << physical_filename << ", not found.\n";
    return false;
  }

  if (physical_filename.is_directory()) {
    PT(VirtualFileMountSystem) new_mount =
      new VirtualFileMountSystem(physical_filename);
    return mount(new_mount, mount_point, flags);
  } else {
    // It's not a directory; it must be a Multifile.
    PT(Multifile) multifile = new Multifile;
    multifile->set_encryption_password(password);

    // For now these are always opened read only.  Maybe later we'll
    // support read-write on Multifiles.
    flags |= MF_read_only;
    if (!multifile->open_read(physical_filename)) {
      return false;
    }

    return mount(multifile, mount_point, flags);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::mount_loop
//       Access: Published
//  Description: This is similar to mount(), but it receives the name
//               of a Multifile that already appears within the
//               virtual file system.  It can be used to mount a
//               Multifile that is itself hosted within a
//               virtually-mounted Multifile.
//
//               This interface can also be used to mount physical
//               files (that appear within the virtual filespace), but
//               it cannot be used to mount directories.  Use mount()
//               if you need to mount a directory.
//
//               Note that there is additional overhead, in the form
//               of additional buffer copies of the data, for
//               recursively mounting a multifile like this.
////////////////////////////////////////////////////////////////////
bool VirtualFileSystem::
mount_loop(const Filename &virtual_filename, const Filename &mount_point, 
           int flags, const string &password) {
  PT(VirtualFile) file = get_file(virtual_filename, false);
  if (file == NULL) {
    express_cat->warning()
      << "Attempt to mount " << virtual_filename << ", not found.\n";
    return false;
  }

  if (file->is_directory()) {
    PT(VirtualFileMountSystem) new_mount =
      new VirtualFileMountSystem(virtual_filename);
    return mount(new_mount, mount_point, flags);

  } else {
    // It's not a directory; it must be a Multifile.
    PT(Multifile) multifile = new Multifile;
    multifile->set_encryption_password(password);

    // For now these are always opened read only.  Maybe later we'll
    // support read-write on Multifiles.
    flags |= MF_read_only;
    if (!multifile->open_read(virtual_filename)) {
      return false;
    }

    return mount(multifile, mount_point, flags);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::mount
//       Access: Published
//  Description: Adds the given VirtualFileMount object to the mount
//               list.  This is a lower-level function that the other
//               flavors of mount(); it requires you to create a
//               VirtualFileMount object specifically.
////////////////////////////////////////////////////////////////////
bool VirtualFileSystem::
mount(VirtualFileMount *mount, const Filename &mount_point, int flags) {
  if (express_cat->is_debug()) {
    express_cat->debug()
      << "mount " << *mount << " under " << mount_point << "\n";
  }

  _lock.acquire();
  bool result = do_mount(mount, mount_point, flags);
  _lock.release();
  return result;
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::unmount
//       Access: Published
//  Description: Unmounts all appearances of the indicated Multifile
//               from the file system.  Returns the number of
//               appearances unmounted.
////////////////////////////////////////////////////////////////////
int VirtualFileSystem::
unmount(Multifile *multifile) {
  _lock.acquire();
  Mounts::iterator ri, wi;
  wi = ri = _mounts.begin();
  while (ri != _mounts.end()) {
    VirtualFileMount *mount = (*ri);
    (*wi) = mount;

    if (mount->is_exact_type(VirtualFileMountMultifile::get_class_type())) {
      VirtualFileMountMultifile *mmount = 
        DCAST(VirtualFileMountMultifile, mount);
      if (mmount->get_multifile() == multifile) {
        // Remove this one.  Don't increment wi.
        if (express_cat->is_debug()) {
          express_cat->debug()
            << "unmount " << *mount << " from " << mount->get_mount_point() << "\n";
        }
        mount->_file_system = NULL;

      } else {
        // Don't remove this one.
        ++wi;
      }
    } else {
      // Don't remove this one.
      ++wi;
    }
    ++ri;
  }

  int num_removed = _mounts.end() - wi;
  _mounts.erase(wi, _mounts.end());
  ++_mount_seq;
  _lock.release();
  return num_removed;
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::unmount
//       Access: Published
//  Description: Unmounts all appearances of the indicated directory
//               name or multifile name from the file system.  Returns
//               the number of appearances unmounted.
////////////////////////////////////////////////////////////////////
int VirtualFileSystem::
unmount(const Filename &physical_filename) {
  _lock.acquire();
  Mounts::iterator ri, wi;
  wi = ri = _mounts.begin();
  while (ri != _mounts.end()) {
    VirtualFileMount *mount = (*ri);
    (*wi) = mount;

    if (mount->is_exact_type(VirtualFileMountSystem::get_class_type())) {
      VirtualFileMountSystem *smount = 
        DCAST(VirtualFileMountSystem, mount);
      if (smount->get_physical_filename() == physical_filename) {
        // Remove this one.  Don't increment wi.
        if (express_cat->is_debug()) {
          express_cat->debug()
            << "unmount " << *mount << " from " << mount->get_mount_point() << "\n";
        }
        mount->_file_system = NULL;
        
      } else {
        // Don't remove this one.
        ++wi;
      }

    } else if (mount->is_exact_type(VirtualFileMountMultifile::get_class_type())) {
      VirtualFileMountMultifile *mmount = 
        DCAST(VirtualFileMountMultifile, mount);
      if (mmount->get_multifile()->get_multifile_name() == physical_filename) {
        // Remove this one.  Don't increment wi.
        if (express_cat->is_debug()) {
          express_cat->debug()
            << "unmount " << *mount << " from " << mount->get_mount_point() << "\n";
        }
        mount->_file_system = NULL;

      } else {
        // Don't remove this one.
        ++wi;
      }

    } else {
      // Don't remove this one.
      ++wi;
    }
    ++ri;
  }

  int num_removed = _mounts.end() - wi;
  _mounts.erase(wi, _mounts.end());
  ++_mount_seq;
  _lock.release();
  return num_removed;
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::unmount
//       Access: Published
//  Description: Unmounts the indicated VirtualFileMount object
//               from the file system.  Returns the number of
//               appearances unmounted.
////////////////////////////////////////////////////////////////////
int VirtualFileSystem::
unmount(VirtualFileMount *mount) {
  _lock.acquire();
  Mounts::iterator ri, wi;
  wi = ri = _mounts.begin();
  while (ri != _mounts.end()) {
    (*wi) = (*ri);
    if ((*ri) == mount) {
      // Remove this one.  Don't increment wi.
      if (express_cat->is_debug()) {
        express_cat->debug()
          << "unmount " << *mount << " from " << mount->get_mount_point() << "\n";
      }
      (*ri)->_file_system = NULL;

    } else {
      // Don't remove this one.
      ++wi;
    }
    ++ri;
  }

  int num_removed = _mounts.end() - wi;
  _mounts.erase(wi, _mounts.end());
  ++_mount_seq;
  _lock.release();
  return num_removed;
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::unmount_point
//       Access: Published
//  Description: Unmounts all systems attached to the given mount
//               point from the file system.  Returns the number of
//               appearances unmounted.
////////////////////////////////////////////////////////////////////
int VirtualFileSystem::
unmount_point(const Filename &mount_point) {
  _lock.acquire();
  Filename nmp = normalize_mount_point(mount_point);
  Mounts::iterator ri, wi;
  wi = ri = _mounts.begin();
  while (ri != _mounts.end()) {
    VirtualFileMount *mount = (*ri);
    (*wi) = mount;

    if (mount->get_mount_point() == nmp) {
      // Remove this one.  Don't increment wi.
      if (express_cat->is_debug()) {
        express_cat->debug()
          << "unmount " << *mount << " from " << mount->get_mount_point() << "\n";
      }
      mount->_file_system = NULL;

    } else {
      // Don't remove this one.
      ++wi;
    }
    ++ri;
  }

  int num_removed = _mounts.end() - wi;
  _mounts.erase(wi, _mounts.end());
  ++_mount_seq;
  _lock.release();
  return num_removed;
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::unmount_all
//       Access: Published
//  Description: Unmounts all files from the file system.  Returns the
//               number of systems unmounted.
////////////////////////////////////////////////////////////////////
int VirtualFileSystem::
unmount_all() {
  _lock.acquire();
  Mounts::iterator ri;
  for (ri = _mounts.begin(); ri != _mounts.end(); ++ri) {
    VirtualFileMount *mount = (*ri);
    if (express_cat->is_debug()) {
      express_cat->debug()
        << "unmount " << *mount << " from " << mount->get_mount_point() << "\n";
    }
    mount->_file_system = NULL;
  }

  int num_removed = _mounts.size();
  _mounts.clear();
  ++_mount_seq;
  _lock.release();
  return num_removed;
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::get_num_mounts
//       Access: Published
//  Description: Returns the number of individual mounts in the
//               system.
////////////////////////////////////////////////////////////////////
int VirtualFileSystem::
get_num_mounts() const {
  ((VirtualFileSystem *)this)->_lock.acquire();
  int result = _mounts.size();
  ((VirtualFileSystem *)this)->_lock.release();
  return result;
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::get_mount
//       Access: Published
//  Description: Returns the nth mount in the system.
////////////////////////////////////////////////////////////////////
PT(VirtualFileMount) VirtualFileSystem::
get_mount(int n) const {
  ((VirtualFileSystem *)this)->_lock.acquire();
  nassertd(n >= 0 && n < (int)_mounts.size()) {
    ((VirtualFileSystem *)this)->_lock.release();
    return NULL;
  }
  PT(VirtualFileMount) result = _mounts[n];
  ((VirtualFileSystem *)this)->_lock.release();
  return result;
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::chdir
//       Access: Published
//  Description: Changes the current directory.  This is used to
//               resolve relative pathnames in get_file() and/or
//               find_file().  Returns true if successful, false
//               otherwise.
////////////////////////////////////////////////////////////////////
bool VirtualFileSystem::
chdir(const Filename &new_directory) {
  _lock.acquire();
  if (new_directory == "/") {
    // We can always return to the root.
    _cwd = new_directory;
    _lock.release();
    return true;
  }

  PT(VirtualFile) file = do_get_file(new_directory, true);
  if (file != (VirtualFile *)NULL && file->is_directory()) {
    _cwd = file->get_filename();
    _lock.release();
    return true;
  }
  _lock.release();
  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::get_cwd
//       Access: Published
//  Description: Returns the current directory name.  See chdir().
////////////////////////////////////////////////////////////////////
Filename VirtualFileSystem::
get_cwd() const {
  ((VirtualFileSystem *)this)->_lock.acquire();
  Filename result = _cwd;
  ((VirtualFileSystem *)this)->_lock.release();
  return result;
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::get_file
//       Access: Published
//  Description: Looks up the file by the indicated name in the file
//               system.  Returns a VirtualFile pointer representing
//               the file if it is found, or NULL if it is not.
//
//               If status_only is true, the file will be checked for
//               existence and length and so on, but the returned
//               file's contents cannot be read.  This is an
//               optimization which is especially important for
//               certain mount types, for instance HTTP, for which
//               opening a file to determine its status is
//               substantially less expensive than opening it to read
//               its contents.
////////////////////////////////////////////////////////////////////
PT(VirtualFile) VirtualFileSystem::
get_file(const Filename &filename, bool status_only) const {
  ((VirtualFileSystem *)this)->_lock.acquire();
  PT(VirtualFile) result = do_get_file(filename, status_only);
  ((VirtualFileSystem *)this)->_lock.release();
  return result;
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::find_file
//       Access: Published
//  Description: Uses the indicated search path to find the file
//               within the file system.  Returns the first occurrence
//               of the file found, or NULL if the file cannot be
//               found.
////////////////////////////////////////////////////////////////////
PT(VirtualFile) VirtualFileSystem::
find_file(const Filename &filename, const DSearchPath &searchpath,
          bool status_only) const {
  if (!filename.is_local()) {
    return get_file(filename, status_only);
  }

  int num_directories = searchpath.get_num_directories();
  for (int i = 0; i < num_directories; ++i) {
    Filename match(searchpath.get_directory(i), filename);
    if (searchpath.get_directory(i) == "." && 
        filename.is_fully_qualified()) {
      // A special case for the "." directory: to avoid prefixing an
      // endless stream of ./ in front of files, if the filename
      // already has a ./ prefixed (i.e. is_fully_qualified() is
      // true), we don't prefix another one.
      match = filename;
    }
    PT(VirtualFile) found_file = get_file(match, status_only);
    if (found_file != (VirtualFile *)NULL) {
      return found_file;
    }
  }

  return NULL;
}


////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::resolve_filename
//       Access: Public
//  Description: Searches the given search path for the filename.  If
//               it is found, updates the filename to the full
//               pathname found and returns true; otherwise, returns
//               false.
////////////////////////////////////////////////////////////////////
bool VirtualFileSystem::
resolve_filename(Filename &filename,
                 const DSearchPath &searchpath,
                 const string &default_extension) const {
  PT(VirtualFile) found;

  if (filename.is_local()) {
    found = find_file(filename, searchpath, true);

    if (found.is_null()) {
      // We didn't find it with the given extension; can we try the
      // default extension?
      if (filename.get_extension().empty() && !default_extension.empty()) {
        Filename try_ext = filename;
        try_ext.set_extension(default_extension);
        found = find_file(try_ext, searchpath, true);
      }
    }
  } else {
    if (exists(filename)) {
      // The full pathname exists.  Return true.
      return true;
    } else {
      // The full pathname doesn't exist with the given extension;
      // does it exist with the default extension?
      if (filename.get_extension().empty() && !default_extension.empty()) {
        Filename try_ext = filename;
        try_ext.set_extension(default_extension);
        found = get_file(try_ext, true);
      }
    }
  }

  if (!found.is_null()) {
    filename = found->get_original_filename();
    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::find_all_files
//       Access: Public
//  Description: Searches all the directories in the search list for
//               the indicated file, in order.  Fills up the results
//               list with *all* of the matching filenames found, if
//               any.  Returns the number of matches found.
//
//               It is the responsibility of the the caller to clear
//               the results list first; otherwise, the newly-found
//               files will be appended to the list.
////////////////////////////////////////////////////////////////////
int VirtualFileSystem::
find_all_files(const Filename &filename, const DSearchPath &searchpath,
               DSearchPath::Results &results) const {
  int num_added = 0;

  if (filename.is_local()) {
    int num_directories = searchpath.get_num_directories();
    for (int i = 0; i < num_directories; ++i) {
      Filename match(searchpath.get_directory(i), filename);
      if (exists(match)) {
        if (searchpath.get_directory(i) == "." &&
            filename.is_fully_qualified()) {
          // A special case for the "." directory: to avoid prefixing
          // an endless stream of ./ in front of files, if the
          // filename already has a ./ prefixed
          // (i.e. is_fully_fully_qualified() is true), we don't
          // prefix another one.
          results.add_file(filename);
        } else {
          results.add_file(match);
        }
        ++num_added;
      }
    }
  }

  return num_added;
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::write
//       Access: Published
//  Description: Print debugging information.
//               (e.g. from Python or gdb prompt).
////////////////////////////////////////////////////////////////////
void VirtualFileSystem::
write(ostream &out) const {
  ((VirtualFileSystem *)this)->_lock.acquire();
  Mounts::const_iterator mi;
  for (mi = _mounts.begin(); mi != _mounts.end(); ++mi) {
    VirtualFileMount *mount = (*mi);
    mount->write(out);
  }
  ((VirtualFileSystem *)this)->_lock.release();
}


////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::get_global_ptr
//       Access: Published, Static
//  Description: Returns the default global VirtualFileSystem.  You
//               may create your own personal VirtualFileSystem
//               objects and use them for whatever you like, but Panda
//               will attempt to load models and stuff from this
//               default object.
//
//               Initially, the global VirtualFileSystem is set up to
//               mount the OS filesystem to root; i.e. it is
//               equivalent to the OS filesystem.  This may be
//               subsequently adjusted by the user.
////////////////////////////////////////////////////////////////////
VirtualFileSystem *VirtualFileSystem::
get_global_ptr() {
  if (_global_ptr == (VirtualFileSystem *)NULL) {
    // Make sure this is initialized.
    init_libexpress();

    _global_ptr = new VirtualFileSystem;
    
    // Set up the default mounts.  First, there is always the root
    // mount.
    _global_ptr->mount("/", "/", 0);

    // And our initial cwd comes from the environment.
    _global_ptr->chdir(ExecutionEnvironment::get_cwd());

    // Then, we add whatever mounts are listed in the Configrc file.
    ConfigVariableList mounts
      ("vfs-mount",
       PRC_DESC("vfs-mount system-filename mount-point [options]"));

    int num_unique_values = mounts.get_num_unique_values();
    for (int i = 0; i < num_unique_values; i++) {
      string mount_desc = mounts.get_unique_value(i);

      // The vfs-mount syntax is:
      
      // vfs-mount system-filename mount-point [options]
      
      // The last two spaces mark the beginning of the mount point,
      // and of the options, respectively.  There might be multiple
      // spaces in the system filename, which are part of the
      // filename.
      
      // The last space marks the beginning of the mount point.
      // Spaces before that are part of the system filename.
      size_t space = mount_desc.rfind(' ');
      if (space == string::npos) {
        express_cat.warning()
          << "No space in vfs-mount descriptor: " << mount_desc << "\n";
        
      } else {
        string mount_point = mount_desc.substr(space + 1);
        while (space > 0 && isspace(mount_desc[space - 1])) {
          space--;
        }
        mount_desc = mount_desc.substr(0, space);
        string options;
        
        space = mount_desc.rfind(' ');
        if (space != string::npos) {
          // If there's another space, we have the optional options field.
          options = mount_point;
          mount_point = mount_desc.substr(space + 1);
          while (space > 0 && isspace(mount_desc[space - 1])) {
            --space;
          }
          mount_desc = mount_desc.substr(0, space);
        }
        
        mount_desc = ExecutionEnvironment::expand_string(mount_desc);
        Filename physical_filename = Filename::from_os_specific(mount_desc);
        
        int flags = 0;
        string password;
        
        // Split the options up by commas.
        size_t p = 0;
        size_t q = options.find(',', p);
        while (q != string::npos) {
          parse_option(options.substr(p, q - p),
                       flags, password);
          p = q + 1;
          q = options.find(',', p);
        }
        parse_option(options.substr(p), flags, password);
        
        _global_ptr->mount(physical_filename, mount_point, flags, password);
      }
    }
  }

  return _global_ptr;
}

#ifdef HAVE_PYTHON
////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::__py__read_file
//       Access: Published
//  Description: Convenience function; returns the entire contents of
//               the indicated file as a string.
//
//               This variant on read_file() is implemented directly
//               for Python, as a small optimization, to avoid the
//               double-construction of a string object that would be
//               otherwise required for the return value.
////////////////////////////////////////////////////////////////////
PyObject *VirtualFileSystem::
__py__read_file(const Filename &filename, bool auto_unwrap) const {
  pvector<unsigned char> pv;
  bool okflag = read_file(filename, pv, auto_unwrap);
  nassertr(okflag, NULL);

  if (pv.empty()) {
    return PyString_FromStringAndSize("", 0);
  } else {
    return PyString_FromStringAndSize((const char *)&pv[0], pv.size());
  }
}
#endif  // HAVE_PYTHON

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::open_read_file
//       Access: Published
//  Description: Convenience function; returns a newly allocated
//               istream if the file exists and can be read, or NULL
//               otherwise.  Does not return an invalid istream.
//
//               If auto_unwrap is true, an explicitly-named .pz file
//               is automatically decompressed and the decompressed
//               contents are returned.  This is different than
//               vfs-implicit-pz, which will automatically decompress
//               a file if the extension .pz is *not* given.
////////////////////////////////////////////////////////////////////
istream *VirtualFileSystem::
open_read_file(const Filename &filename, bool auto_unwrap) const {
  PT(VirtualFile) file = get_file(filename, false);
  if (file == (VirtualFile *)NULL) {
    return NULL;
  }
  istream *str = file->open_read_file(auto_unwrap);
  if (str != (istream *)NULL && str->fail()) {
    close_read_file(str);
    str = (istream *)NULL;
  }
  return str;
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::close_read_file
//       Access: Published, Static
//  Description: Closes a file opened by a previous call to
//               open_read_file().  This really just deletes the
//               istream pointer, but it is recommended to use this
//               interface instead of deleting it explicitly, to help
//               work around compiler issues.
////////////////////////////////////////////////////////////////////
void VirtualFileSystem::
close_read_file(istream *stream) {
  if (stream != (istream *)NULL) {
    // For some reason--compiler bug in gcc 3.2?--explicitly deleting
    // the stream pointer does not call the appropriate global delete
    // function; instead apparently calling the system delete
    // function.  So we call the delete function by hand instead.
#if (!defined(WIN32_VC) && !defined(WIN64_VC)) && !defined(USE_MEMORY_NOWRAPPERS) && defined(REDEFINE_GLOBAL_OPERATOR_NEW)
    stream->~istream();
    (*global_operator_delete)(stream);
#else
    delete stream;
#endif
  }
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::scan_mount_points
//       Access: Public
//  Description: Adds to names a list of all the mount points in use
//               that are one directory below path, if any.  That is,
//               these are the external files or directories mounted
//               directly to the indicated path.
//
//               The names vector is filled with a set of basenames,
//               the basename part of the mount point.
////////////////////////////////////////////////////////////////////
void VirtualFileSystem::
scan_mount_points(vector_string &names, const Filename &path) const {
  nassertv(!path.empty() && !path.is_local());
  string prefix = path.get_fullpath().substr(1);
  Mounts::const_iterator mi;
  for (mi = _mounts.begin(); mi != _mounts.end(); ++mi) {
    VirtualFileMount *mount = (*mi);
    
    string mount_point = mount->get_mount_point();
    if (prefix.empty()) {
      // The indicated path is the root.  Is the mount point on the
      // root?
      if (mount_point.find('/') == string::npos) {
        // No embedded slashes, so the mount point is only one
        // directory below the root.
        names.push_back(mount_point);
      }
    } else {
      if (mount_point.substr(0, prefix.length()) == prefix &&
          mount_point.length() > prefix.length() &&
          mount_point[prefix.length()] == '/') {
        // This mount point is below the indicated path.  Is it only one
        // directory below?
        string basename = mount_point.substr(prefix.length());
        if (basename.find('/') == string::npos) {
          // No embedded slashes, so it's only one directory below.
          names.push_back(basename);
        }
      }
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::parse_option
//       Access: Public, Static
//  Description: Parses one of the option flags in the options list on
//               the vfs-mount Config.prc line.
////////////////////////////////////////////////////////////////////
void VirtualFileSystem::
parse_option(const string &option, int &flags, string &password) {
  if (option == "0" || option.empty()) {
    // 0 is the null option.
  } else if (option == "ro") {
    flags |= MF_read_only;
  } else if (option.substr(0, 3) == "pw:") {
    password = option.substr(3);
  } else {
    express_cat.warning()
      << "Invalid option on vfs-mount: \"" << option << "\"\n";
  }
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::normalize_mount_point
//       Access: Private
//  Description: Converts the mount point string supplied by the user
//               to standard form (relative to the current directory,
//               with no double slashes, and not terminating with a
//               slash).  The initial slash is removed.
//
//               Assumes the lock is already held.
////////////////////////////////////////////////////////////////////
Filename VirtualFileSystem::
normalize_mount_point(const Filename &mount_point) const {
  Filename nmp = mount_point;
  if (nmp.is_local()) {
    nmp = Filename(_cwd, mount_point);
  }
  nmp.standardize();
  nassertr(!nmp.empty() && nmp[0] == '/', nmp);
  return nmp.get_fullpath().substr(1);
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::do_mount
//       Access: Private
//  Description: The private implementation of mount().  Assumes the
//               lock is already held.
////////////////////////////////////////////////////////////////////
bool VirtualFileSystem::
do_mount(VirtualFileMount *mount, const Filename &mount_point, int flags) {
  nassertr(mount->_file_system == NULL, false);
  mount->_file_system = this;
  mount->_mount_point = normalize_mount_point(mount_point);
  mount->_mount_flags = flags;
  _mounts.push_back(mount);
  ++_mount_seq;
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::do_get_file
//       Access: Private
//  Description: The private implementation of get_file().  Assumes
//               the lock is already held.
////////////////////////////////////////////////////////////////////
PT(VirtualFile) VirtualFileSystem::
do_get_file(const Filename &filename, bool status_only) const {
  nassertr(!filename.empty(), NULL);
  Filename pathname(filename);
  if (pathname.is_local()) {
    pathname = Filename(_cwd, filename);
    if (filename.is_text()) {
      pathname.set_text();
    }
  }
  pathname.standardize();
  Filename strpath = pathname.get_filename_index(0).get_fullpath().substr(1);
  // Also transparently look for a regular file suffixed .pz.
  Filename strpath_pz = strpath + ".pz";

  // Now scan all the mount points, from the back (since later mounts
  // override more recent ones), until a match is found.
  PT(VirtualFile) found_file = NULL;
  VirtualFileComposite *composite_file = NULL;

  // We use an index instead of an iterator, since the vector might
  // change if implicit mounts are added during this loop.
  unsigned int start_seq = _mount_seq;

  size_t i = _mounts.size();
  while (i > 0) {
    --i;
    VirtualFileMount *mount = _mounts[i];
    Filename mount_point = mount->get_mount_point();
    if (strpath == mount_point) {
      // Here's an exact match on the mount point.  This filename is
      // the root directory of this mount object.
      if (consider_match(found_file, composite_file, mount, "", pathname,
                         false, status_only)) {
        return found_file;
      }
    } else if (mount_point.empty()) {
      // This is the root mount point; all files are in here.
      if (consider_match(found_file, composite_file, mount, strpath, 
                         pathname, false, status_only)) {
        return found_file;
      }
#ifdef HAVE_ZLIB
      if (vfs_implicit_pz) {
        if (consider_match(found_file, composite_file, mount, strpath_pz, 
                           pathname, true, status_only)) {
          return found_file;
        }
      }
#endif  // HAVE_ZLIB

    } else if (strpath.length() > mount_point.length() &&
               mount_point == strpath.substr(0, mount_point.length()) &&
               strpath[mount_point.length()] == '/') {
      // This pathname falls within this mount system.
      Filename local_filename = strpath.substr(mount_point.length() + 1);
      Filename local_filename_pz = strpath_pz.substr(mount_point.length() + 1);
      if (consider_match(found_file, composite_file, mount, local_filename, 
                         pathname, false, status_only)) {
        return found_file;
      }
#ifdef HAVE_ZLIB
      if (vfs_implicit_pz) {
        // Bingo!
        if (consider_match(found_file, composite_file, mount, local_filename_pz,
                           pathname, true, status_only)) {
          return found_file;
        }
      }
#endif  // HAVE_ZLIB
    }

    // If we discover that a file has been implicitly mounted during
    // one of the above operations, start over from the beginning of
    // the loop.
    if (start_seq != _mount_seq) {
      start_seq = _mount_seq;
      i = _mounts.size();
    }
  }

  if (found_file == (VirtualFile *)NULL && vfs_implicit_mf) {
    // The file wasn't found, as-is.  Does it appear to be an implicit
    // .mf file reference?
    ((VirtualFileSystem *)this)->consider_mount_mf(filename);

    if (start_seq != _mount_seq) {
      // Yes, it was, or some nested file was.  Now that we've
      // implicitly mounted the .mf file, go back and look again.
      return do_get_file(filename, status_only);
    }
  }

  return found_file;
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::consider_match
//       Access: Private
//  Description: Evaluates one possible filename match found during a
//               get_file() operation.  There may be multiple matches
//               for a particular filename due to the ambiguities
//               introduced by allowing multiple mount points, so we
//               may have to keep searching even after the first match
//               is found.
//
//               Returns true if the search should terminate now, or
//               false if it should keep iterating.
////////////////////////////////////////////////////////////////////
bool VirtualFileSystem::
consider_match(PT(VirtualFile) &found_file, VirtualFileComposite *&composite_file,
               VirtualFileMount *mount, const Filename &local_filename,
               const Filename &original_filename, bool implicit_pz_file,
               bool status_only) const {
  PT(VirtualFile) vfile = 
    mount->make_virtual_file(local_filename, original_filename, false, status_only);
  if (!vfile->has_file()) {
    // Keep looking.
    return false;
  }

  if (found_file == (VirtualFile *)NULL) {
    // This was our first match.  Save it.
    found_file = vfile;
    if (!found_file->is_directory()) {
      // If it's not a directory, we're done.
      return true;
    }
    // It is a directory, so save it for later.
    if (implicit_pz_file) {
      // Don't look for directories named file.pz.
      found_file = NULL;
    }

  } else {
    // This was our second match.  The previous match(es) must
    // have been directories.
    if (!vfile->is_directory()) {
      // However, this one isn't a directory.  We're done.
      return true;
    }

    if (!implicit_pz_file) {
      // At least two directories matched to the same path.  We
      // need a composite directory.
      if (composite_file == (VirtualFileComposite *)NULL) {
        composite_file =
          new VirtualFileComposite((VirtualFileSystem *)this, found_file->get_original_filename());
        composite_file->set_original_filename(original_filename);
        composite_file->add_component(found_file);
        found_file = composite_file;
      }

      composite_file->add_component(vfile);
    }
  }

  // Keep going, looking for more directories.
  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: VirtualFileSystem::consider_mount_mf
//       Access: Private
//  Description: The indicated filename was not found.  Check to see
//               if it is using an implicit reference to a .mf file as
//               a directory, that hasn't already been mounted.  If it
//               is, mount the .mf file in-place, and return true; if
//               it is not, or if its .mf file is already mounted
//               in-place, return false.
//
//               Assumes the lock is already held.
////////////////////////////////////////////////////////////////////
bool VirtualFileSystem::
consider_mount_mf(const Filename &filename) {
  Filename dirname = filename.get_dirname();
  if (dirname.empty() || dirname == filename) {
    // Reached the top directory; no .mf file references.
    return false;
  }
  if (is_directory(dirname)) {
    // Reached a real (or already-mounted) directory; no unmounted .mf
    // file references.
    return false;
  }
  if (dirname.get_extension() == "mf") {
    // Hey, here's a multifile reference!
    dirname.set_binary();
    PT(VirtualFile) file = do_get_file(dirname, false);
    if (file == (VirtualFile *)NULL || !file->is_regular_file()) {
      // Oh, never mind.  Not a real file.
      return false;
    }

    PT(Multifile) multifile = new Multifile;
   
    istream *stream = file->open_read_file(false);
    if (stream == (istream *)NULL) {
      // Couldn't read file.
      return false;
    }

    // Wrap a thread-safe wrapper around that stream, so multiple
    // threads can safely read the multifile simultaneously.
    IStreamWrapper *streamw = new IStreamWrapper(stream, true);

    if (!multifile->open_read(streamw, true)) {
      // Invalid multifile.
      return false;
    }

    multifile->set_multifile_name(dirname.get_basename());
    express_cat->info()
      << "Implicitly mounting " << dirname << "\n";

    PT(VirtualFileMountMultifile) new_mount = 
      new VirtualFileMountMultifile(multifile);
    return do_mount(new_mount, dirname, MF_read_only);
  }

  // Recurse.
  return consider_mount_mf(dirname);
}
