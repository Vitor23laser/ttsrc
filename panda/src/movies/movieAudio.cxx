// Filename: movieAudio.cxx
// Created by: jyelon (02Jul07)
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

#include "movieAudio.h"
#include "movieAudioCursor.h"

#ifdef __SWITCH__
#include "nxAudio.h"
#endif

TypeHandle MovieAudio::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: MovieAudio::Constructor
//       Access: Public
//  Description: This constructor returns a null audio stream --- a
//               stream of total silence, at 8000 samples per second.
//               To get more interesting audio, you need to construct
//               a subclass of this class.
////////////////////////////////////////////////////////////////////
MovieAudio::
MovieAudio(const string &name) :
  Namable(name)
{
}

////////////////////////////////////////////////////////////////////
//     Function: MovieAudio::Destructor
//       Access: Public, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
MovieAudio::
~MovieAudio() {
}

////////////////////////////////////////////////////////////////////
//     Function: MovieAudio::open
//       Access: Published, Virtual
//  Description: Open this audio, returning a MovieAudioCursor
////////////////////////////////////////////////////////////////////
PT(MovieAudioCursor) MovieAudio::
open() {
  return new MovieAudioCursor(this);
}

////////////////////////////////////////////////////////////////////
//     Function: MovieAudio::get
//       Access: Published, Static
//  Description: Obtains a MovieAudio that references a file.
////////////////////////////////////////////////////////////////////
PT(MovieAudio) MovieAudio::
get(const Filename &name) {
#ifdef __SWITCH__
  return new NxAudio(name);
#elif defined(HAVE_FFMPEG)
  // Someday, I'll probably put a dispatcher here.
  // But for now, just hardwire it to go to FFMPEG.
  return new FfmpegAudio(name);
#else
  return new MovieAudio("Load-Failure Stub");
#endif
}
