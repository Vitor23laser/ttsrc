// Filename: physxShape.h
// Created by:  enn0x (16Sep09)
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

#ifndef PHYSXSHAPE_H
#define PHYSXSHAPE_H

#include "pandabase.h"
#include "pointerTo.h"
#include "lpoint3.h"
#include "lmatrix.h"
#include "lquaternion.h"

#include "physxObject.h"
#include "physxEnums.h"
#include "physx_includes.h"

class PhysxActor;
class PhysxMaterial;
class PhysxGroupsMask;
class PhysxBounds3;
class PhysxSphere;
class PhysxBox;
class PhysxCapsule;
class PhysxRay;
class PhysxRaycastHit;

////////////////////////////////////////////////////////////////////
//       Class : PhysxShape
// Description : Abstract base class for shapes.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxShape : public PhysxObject, public PhysxEnums {

PUBLISHED:
  void release();

  PhysxActor *get_actor() const;

  void set_name(const char *name);
  void set_flag(const PhysxShapeFlag flag, bool value);
  void set_skin_width(float skinWidth);
  void set_group(unsigned short group);
  void set_local_pos(const LPoint3f &pos);
  void set_local_mat(const LMatrix4f &mat);
  void set_material(const PhysxMaterial &material);
  void set_material_index(unsigned short idx);
  void set_groups_mask(const PhysxGroupsMask &mask);

  const char *get_name() const;
  bool get_flag(const PhysxShapeFlag flag) const;
  float get_skin_width() const;
  unsigned short get_group() const;
  LPoint3f get_local_pos() const;
  LMatrix4f get_local_mat() const;
  unsigned short get_material_index() const;
  PhysxGroupsMask get_groups_mask() const;
  PhysxBounds3 get_world_bounds() const;

  bool check_overlap_aabb(const PhysxBounds3 &world_bounds) const;
  bool check_overlap_capsule(const PhysxCapsule &world_capsule) const;
  bool check_overlap_obb(const PhysxBox &world_box) const;
  bool check_overlap_sphere(const PhysxSphere &world_sphere) const;
  PhysxRaycastHit raycast(const PhysxRay &worldRay, bool firstHit, bool smoothNormal) const;

  INLINE void ls() const;
  INLINE void ls(ostream &out, int indent_level=0) const;

public:
  static PhysxShape *factory(NxShapeType shapeType);

  virtual NxShape *ptr() const = 0;

  virtual void link(NxShape *shapePtr) = 0;
  virtual void unlink() = 0;

protected:
  INLINE PhysxShape();

private:
  string _name;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    PhysxObject::init_type();
    register_type(_type_handle, "PhysxShape", 
                  PhysxObject::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {
    init_type();
    return get_class_type();
  }

private:
  static TypeHandle _type_handle;
};

#include "physxShape.I"

#endif // PHYSXSHAPE_H
