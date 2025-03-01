// Filename: renderAttrib.h
// Created by:  drose (21Feb02)
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

#ifndef RENDERATTRIB_H
#define RENDERATTRIB_H

#include "pandabase.h"

#include "typedWritableReferenceCount.h"
#include "renderAttribRegistry.h"
#include "pointerTo.h"
#include "pset.h"
#include "lightReMutex.h"

class AttribSlots;
class GraphicsStateGuardianBase;
class CullTraverser;
class CullTraverserData;

////////////////////////////////////////////////////////////////////
//       Class : RenderAttrib
// Description : This is the base class for a number of render
//               attributes (other than transform) that may be set on
//               scene graph nodes to control the appearance of
//               geometry.  This includes TextureAttrib, ColorAttrib,
//               etc.
//
//               RenderAttrib represents render attributes that always
//               propagate down to the leaves without regard to the
//               particular node they are assigned to.  A RenderAttrib
//               will have the same effect on a leaf node whether it
//               is assigned to the graph at the leaf or several nodes
//               above.  This is different from RenderEffect, which
//               represents a particular render property that is
//               applied immediately to the node on which it is
//               encountered, like billboarding or decaling.
//
//               You should not attempt to create or modify a
//               RenderAttrib directly; instead, use the make() method
//               of the appropriate kind of attrib you want.  This
//               will allocate and return a new RenderAttrib of the
//               appropriate type, and it may share pointers if
//               possible.  Do not modify the new RenderAttrib if you
//               wish to change its properties; instead, create a new
//               one.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA_PGRAPH RenderAttrib : public TypedWritableReferenceCount {
protected:
  RenderAttrib();
private:
  RenderAttrib(const RenderAttrib &copy);
  void operator = (const RenderAttrib &copy);

public:
  virtual ~RenderAttrib();

PUBLISHED:
  INLINE CPT(RenderAttrib) compose(const RenderAttrib *other) const;
  INLINE CPT(RenderAttrib) invert_compose(const RenderAttrib *other) const;
  virtual bool lower_attrib_can_override() const;

public:
  INLINE bool always_reissue() const;

  virtual bool has_cull_callback() const;
  virtual bool cull_callback(CullTraverser *trav, const CullTraverserData &data) const;

PUBLISHED:
  INLINE int compare_to(const RenderAttrib &other) const;
  INLINE CPT(RenderAttrib) get_unique() const;

  virtual bool unref() const;

  virtual void output(ostream &out) const;
  virtual void write(ostream &out, int indent_level) const;

  static int get_num_attribs();
  static void list_attribs(ostream &out);
  static bool validate_attribs();

  virtual int get_slot() const=0;

  enum PandaCompareFunc {   // intentionally defined to match D3DCMPFUNC
    M_none=0,           // alpha-test disabled (always-draw)
    M_never,            // Never draw.
    M_less,             // incoming < reference_alpha
    M_equal,            // incoming == reference_alpha
    M_less_equal,       // incoming <= reference_alpha
    M_greater,          // incoming > reference_alpha
    M_not_equal,        // incoming != reference_alpha
    M_greater_equal,    // incoming >= reference_alpha
    M_always            // Always draw.  
  };

  // This is the enumerated type for TexGenAttrib.  It is inherited
  // into TexGenAttrib.  It is defined up at this level only to avoid
  // circular dependencies in the header files.
  enum TexGenMode {
    M_off,

    // In the types below, "eye" means the coordinate space of the
    // observing camera, and "world" means world coordinates, e.g. the
    // coordinate space of the root of the graph.

    // Sphere maps are classic static reflection maps.  They are
    // supported on just about any hardware, and require a precomputed
    // 360-degree fisheye image.  Sphere maps only make sense in eye
    // coordinate space.
    M_eye_sphere_map,

    // Cube maps are a modern improvement on the sphere map; they
    // don't suffer from any polar singularities, but they require six
    // texture images.  They can also be generated dynamically for
    // real-time reflections (see GraphicsOutput::make_cube_map()).
    // Typically, a statically-generated cube map will be in eye
    // space, while a dynamically-generated map will be in world
    // space.

    // Cube mapping is not supported on all hardware.
    M_world_cube_map,
    M_eye_cube_map,

    // Normal maps are most useful for applying diffuse lighting
    // effects via a pregenerated cube map.
    M_world_normal,
    M_eye_normal,

    // Position maps convert XYZ coordinates directly to texture
    // coordinates.  This is particularly useful for implementing
    // projective texturing (see NodePath::project_texture()).
    M_world_position,
    M_unused,  // formerly M_object_position, now deprecated.
    M_eye_position,

    // With M_point_sprite, texture coordinates will be generated for
    // large points in the range (0,0) - (1,1) from upper-left to
    // lower-right across the point's face.  Without this, each point
    // will have just a single uniform texture coordinate value across
    // its face.

    // Unfortunately, the generated texture coordinates are inverted
    // (upside-down) from Panda's usual convention, but this is what
    // the graphics card manufacturers decided to use.  You could use
    // a texture matrix to re-invert the texture, but that will
    // probably force the sprites' vertices to be computed in the CPU.
    // You'll have to paint your textures upside-down if you want true
    // hardware sprites.
    M_point_sprite,

    // M_light_vector generates special 3-d texture coordinates that
    // represent the vector to a particular Light in the scene graph,
    // expressed in each vertex's tangent space.  This is used to
    // implement bumpmapping.  It is always computed on the CPU.

    // This requires a Light to be specified to the TexGenAttrib.  It
    // also requires each vertex to define a normal, as well as a
    // tangent and binormal for the particular named texture
    // coordinate set.
    M_light_vector,

    // M_constant generates the same fixed texture coordinates at each
    // vertex.  Not terribly useful, of course, except for certain
    // special effects involving moving a flat color over an object.
    M_constant,
  };

protected:
  static CPT(RenderAttrib) return_new(RenderAttrib *attrib);
  static CPT(RenderAttrib) return_unique(RenderAttrib *attrib);
  virtual int compare_to_impl(const RenderAttrib *other) const;
  virtual CPT(RenderAttrib) compose_impl(const RenderAttrib *other) const;
  virtual CPT(RenderAttrib) invert_compose_impl(const RenderAttrib *other) const;
  void output_comparefunc(ostream &out, PandaCompareFunc fn) const;

public:
  INLINE static int register_slot(TypeHandle type_handle, int sort,
                                  RenderAttribRegistry::MakeDefaultFunc *make_default_func);

private:
  void release_new();

protected:
  bool _always_reissue;

public:
  static void init_attribs();

private:
  // This mutex protects _attribs.
  static LightReMutex *_attribs_lock;
  typedef pset<const RenderAttrib *, indirect_compare_to<const RenderAttrib *> > Attribs;
  static Attribs *_attribs;

  Attribs::iterator _saved_entry;

public:
  virtual void write_datagram(BamWriter *manager, Datagram &dg);
  static TypedWritable *change_this(TypedWritable *old_ptr, BamReader *manager);
  virtual void finalize(BamReader *manager);

protected:
  static TypedWritable *new_from_bam(RenderAttrib *attrib, BamReader *manager);
  void fillin(DatagramIterator &scan, BamReader *manager);
  
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedWritableReferenceCount::init_type();
    register_type(_type_handle, "RenderAttrib",
                  TypedWritableReferenceCount::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

INLINE ostream &operator << (ostream &out, const RenderAttrib &attrib) {
  attrib.output(out);
  return out;
}

#include "renderAttrib.I"

#endif

