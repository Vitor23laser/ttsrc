// Filename: glesgsg.h
// Created by:  pro-rsoft (21May09)
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

#ifndef GLESGSG_H
#define GLESGSG_H

// This header file compiles a GSG for the limited subset of OpenGL
// that is OpenGL ES.

#include "pandabase.h"
#include "config_glesgsg.h"

#define GLP(name) gl##name
#define GLUP(name) glu##name
#define CLP(name) GLES##name
#define GLPREFIX_QUOTED "gl"
#define CLASSPREFIX_QUOTED "GLES"
#define GLSYSTEM_NAME "OpenGL ES"
#define CONFIGOBJ config_glesgsg
#define GLCAT glesgsg_cat
#define EXPCL_GL EXPCL_PANDAGLES
#define EXPTP_GL EXPTP_PANDAGLES
#ifndef OPENGLES
  #define OPENGLES
#endif
#ifndef OPENGLES_1
  #define OPENGLES_1
#endif
#ifdef OPENGLES_2
  #error OPENGLES_2 should not be defined!
#endif

// This prevents glext.h from getting included by gl.h
// That way, we can provide our own, better version.
#define __glext_h_
#define ES1_GLEXT_H_GUARD

#ifdef IS_OSX
  #include <OpenGLES/ES1/gl.h>
  #include <OpenGLES/ES1/glext.h>
#else
  #include <GLES/gl.h>
  #include <GLES/glext.h>
#endif

#include "panda_esglext.h"

// This helps to keep the source clean of hundreds of #ifdefs.
#define GL_NONE GL_NONE_OES
#define GL_RENDERBUFFER_EXT GL_RENDERBUFFER_OES
#define GL_FRAMEBUFFER_EXT GL_FRAMEBUFFER_OES
#define GL_DRAW_FRAMEBUFFER_EXT GL_FRAMEBUFFER_OES
#define GL_READ_FRAMEBUFFER_EXT GL_FRAMEBUFFER_OES
#define GL_COLOR_ATTACHMENT0_EXT GL_COLOR_ATTACHMENT0_OES
#define GL_DEPTH_ATTACHMENT_EXT GL_DEPTH_ATTACHMENT_OES
#define GL_STENCIL_ATTACHMENT_EXT GL_STENCIL_ATTACHMENT_OES
#define GL_FRAMEBUFFER_COMPLETE_EXT GL_FRAMEBUFFER_COMPLETE_OES
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES
#define GL_FRAMEBUFFER_UNSUPPORTED_EXT GL_FRAMEBUFFER_UNSUPPORTED_OES
#define GL_DEPTH_COMPONENT GL_DEPTH_COMPONENT24_OES
#define GL_DEPTH_STENCIL GL_DEPTH_STENCIL_OES
#define GL_DEPTH_STENCIL_EXT GL_DEPTH_STENCIL_OES
#define GL_UNSIGNED_INT_24_8_EXT GL_UNSIGNED_INT_24_8_OES
#define GL_DEPTH24_STENCIL8_EXT GL_DEPTH24_STENCIL8_OES
#define GL_DEPTH_COMPONENT16 GL_DEPTH_COMPONENT16_OES
#define GL_DEPTH_COMPONENT24 GL_DEPTH_COMPONENT24_OES
#define GL_DEPTH_COMPONENT32 GL_DEPTH_COMPONENT32_OES
#define GL_TEXTURE_CUBE_MAP GL_TEXTURE_CUBE_MAP_OES
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X GL_TEXTURE_CUBE_MAP_POSITIVE_X_OES
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X GL_TEXTURE_CUBE_MAP_NEGATIVE_X_OES
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y GL_TEXTURE_CUBE_MAP_POSITIVE_Y_OES
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_OES
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z GL_TEXTURE_CUBE_MAP_POSITIVE_Z_OES
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_OES
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE GL_MAX_CUBE_MAP_TEXTURE_SIZE_OES
#define GL_MIRRORED_REPEAT GL_MIRRORED_REPEAT_OES
#define GL_RGB5_A1 GL_RGB5_A1_OES
#define GL_RGBA4 GL_RGBA4_OES
#define GL_RGB8 GL_RGB8_OES
#define GL_RGBA8 GL_RGBA8_OES
#define GL_FUNC_ADD GL_FUNC_ADD_OES
#define GL_FUNC_SUBTRACT GL_FUNC_SUBTRACT_OES
#define GL_FUNC_REVERSE_SUBTRACT GL_FUNC_REVERSE_SUBTRACT_OES

#ifdef __SWITCH__
// Missing on switch, for some reason
#define GL_BGRA GL_BGRA_EXT
#endif

#undef SUPPORT_IMMEDIATE_MODE
#define APIENTRY
#define APIENTRYP *

#include "glstuff_src.h"
 
#endif  // GLESGSG_H
