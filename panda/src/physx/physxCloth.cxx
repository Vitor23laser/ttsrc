// Filename: physxCloth.cxx
// Created by:  enn0x (30Mar10)
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

#include "physxCloth.h"
#include "physxClothDesc.h"
#include "physxScene.h"
#include "physxGroupsMask.h"
#include "physxShape.h"
#include "physxManager.h"

#include "boundingBox.h"

TypeHandle PhysxCloth::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::link
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxCloth::
link(NxCloth *clothPtr) {

  // Link self
  _ptr = clothPtr;
  _error_type = ET_ok;
  _ptr->userData = this;

  set_name(clothPtr->getName());

  PhysxScene *scene = (PhysxScene *)_ptr->getScene().userData;
  scene->_cloths.add(this);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::unlink
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxCloth::
unlink() {

  // Unlink self
  _ptr->userData = NULL;
  _error_type = ET_released;

  PhysxScene *scene = (PhysxScene *)_ptr->getScene().userData;
  scene->_cloths.remove(this);

  _node = NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::release
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxCloth::
release() {

  nassertv(_error_type == ET_ok);

  unlink();
  _ptr->getScene().releaseCloth(*_ptr);
  _ptr = NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::update
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxCloth::
update() {

  // Update node mesh data
  _node->update();

  // Update node bounding volume
  NxBounds3 bounds;
  _ptr->getWorldBounds(bounds);

  BoundingBox bb(PhysxManager::nxVec3_to_point3(bounds.min),
                 PhysxManager::nxVec3_to_point3(bounds.max));
  _node->set_bounds(&bb);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::get_scene
//       Access: Published
//  Description: Returns the scene which this cloth belongs to.
////////////////////////////////////////////////////////////////////
PhysxScene *PhysxCloth::
get_scene() const {

  nassertr(_error_type == ET_ok, NULL);
  return (PhysxScene *)_ptr->getScene().userData;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::get_cloth_node
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
PhysxClothNode *PhysxCloth::
get_cloth_node() const {

  nassertr(_error_type == ET_ok, NULL);
  return _node;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::create_cloth_node
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
PhysxClothNode *PhysxCloth::
create_cloth_node(const char *name) {

  nassertr(_error_type == ET_ok, NULL);

  _node = new PhysxClothNode(name);
  _node->allocate(this);

  return _node;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::set_name
//       Access: Published
//  Description: Sets a name string for the object that can be
//               retrieved with get_name(). 
//               This is for debugging and is not used by the
//               engine.
////////////////////////////////////////////////////////////////////
void PhysxCloth::
set_name(const char *name) {

  nassertv(_error_type == ET_ok);

  _name = name ? name : "";
  _ptr->setName(_name.c_str());
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::get_name
//       Access: Published
//  Description: Retrieves the name string.
////////////////////////////////////////////////////////////////////
const char *PhysxCloth::
get_name() const {

  nassertr(_error_type == ET_ok, "");
  return _ptr->getName();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::set_group
//       Access: Published
//  Description: Sets which collision group this cloth is part of.
//               Collision group must be between 0 and 31.
////////////////////////////////////////////////////////////////////
void PhysxCloth::
set_group(unsigned int group) {

  nassertv(_error_type == ET_ok);
  nassertv(group >= 0 && group < 32);
  _ptr->setGroup(group);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::get_group
//       Access: Published
//  Description: Retrieves the collision group this cloth is part
//               of.
////////////////////////////////////////////////////////////////////
unsigned int PhysxCloth::
get_group() const {

  nassertr(_error_type == ET_ok, 0);
  return _ptr->getGroup();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::set_thickness
//       Access: Published
//  Description: Sets the cloth thickness (must be positive).
////////////////////////////////////////////////////////////////////
void PhysxCloth::
set_thickness(float thickness) {

  nassertv(_error_type == ET_ok);
  _ptr->setFriction(thickness);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::get_thickness
//       Access: Published
//  Description: Gets the cloth thickness.
////////////////////////////////////////////////////////////////////
float PhysxCloth::
get_thickness() const {

  nassertr(_error_type == ET_ok, 0.0f);
  return _ptr->getThickness();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::get_density
//       Access: Published
//  Description: Gets the cloth density.
////////////////////////////////////////////////////////////////////
float PhysxCloth::
get_density() const {

  nassertr(_error_type == ET_ok, 0.0f);
  return _ptr->getDensity();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::get_relative_grid_spacing
//       Access: Published
//  Description: Gets the relative grid spacing for the broad
//               phase. The cloth is represented by a set of
//               world aligned cubical cells in broad phase. The
//               size of these cells is determined by multiplying
//               the length of the diagonal of the AABB of the
//               initial soft body size with this constant.
////////////////////////////////////////////////////////////////////
float PhysxCloth::
get_relative_grid_spacing() const {

  nassertr(_error_type == ET_ok, 0.0f);
  return _ptr->getRelativeGridSpacing();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::get_num_particles
//       Access: Published
//  Description: Gets the number of cloth particles.
////////////////////////////////////////////////////////////////////
unsigned int PhysxCloth::
get_num_particles() {

  nassertr(_error_type == ET_ok, 0);
  return _ptr->getNumberOfParticles();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::set_flag
//       Access: Published
//  Description: Sets the value of a single flag.
////////////////////////////////////////////////////////////////////
void PhysxCloth::
set_flag(PhysxClothFlag flag, bool value) {

  nassertv(_error_type == ET_ok);

  NxU32 flags = _ptr->getFlags();

  if (value == true) {
    flags |= flag;
  } 
  else {
    flags &= ~(flag);
  }

  _ptr->setFlags(flags);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::get_flag
//       Access: Published
//  Description: Retrieves the value of a single flag.
////////////////////////////////////////////////////////////////////
bool PhysxCloth::
get_flag(PhysxClothFlag flag) const {

  nassertr(_error_type == ET_ok, false);

  return (_ptr->getFlags() & flag) ? true : false;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::set_groups_mask
//       Access: Published
//  Description: Sets 128-bit mask used for collision filtering.
////////////////////////////////////////////////////////////////////
void PhysxCloth::
set_groups_mask(const PhysxGroupsMask &mask) {

  nassertv(_error_type == ET_ok);

  NxGroupsMask _mask = mask.get_mask();
  _ptr->setGroupsMask(_mask);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::get_groups_mask
//       Access: Published
//  Description: Gets the 128-bit groups mask used for collision
//               filtering.
////////////////////////////////////////////////////////////////////
PhysxGroupsMask PhysxCloth::
get_groups_mask() const {

  PhysxGroupsMask mask;

  nassertr(_error_type == ET_ok, mask);

  NxGroupsMask _mask = _ptr->getGroupsMask();
  mask.set_mask(_mask);

  return mask;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::is_sleeping
//       Access: Published
//  Description: Returns true if this cloth is sleeping. 
//
//               When a cloth does not move for a period of time,
//               it is no longer simulated in order to save time.
//               This state is called sleeping. However, because the
//               object automatically wakes up when it is either
//               touched by an awake object, or one of its
//               properties is changed by the user, the entire sleep
//               mechanism should be transparent to the user.
////////////////////////////////////////////////////////////////////
bool PhysxCloth::
is_sleeping() const {

  nassertr(_error_type == ET_ok, false);
  return _ptr->isSleeping();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::wake_up
//       Access: Published
//  Description: Wakes up the cloth if it is sleeping. 
//
//               The wakeCounterValue determines how long until the
//               body is put to sleep, a value of zero means that
//               the body is sleeping. wake_up(0) is equivalent to
//               PhysxCloth::put_to_sleep().
////////////////////////////////////////////////////////////////////
void PhysxCloth::
wake_up(float wakeCounterValue) {

  nassertv(_error_type == ET_ok);
  _ptr->wakeUp(wakeCounterValue);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::put_to_sleep
//       Access: Published
//  Description: Forces the cloth to sleep. 
//
//               The cloth  will stay asleep until the next
//               call to simulate, and will not wake up until then
//               even when otherwise it would (for example a force
//               is applied to it). It can however wake up during
//               the next do_physics call.
////////////////////////////////////////////////////////////////////
void PhysxCloth::
put_to_sleep() {

  nassertv(_error_type == ET_ok);
  _ptr->putToSleep();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::set_sleep_linear_velocity
//       Access: Published
//  Description: Sets the linear velocity below which an cloth
//               may go to sleep. Cloths whose linear velocity is
//               above this threshold will not be put to sleep.
//
//               Setting the sleep angular/linear velocity only
//               makes sense when the BF_energy_sleep_test is not
//               set.
////////////////////////////////////////////////////////////////////
void PhysxCloth::
set_sleep_linear_velocity(float threshold) {

  nassertv(_error_type == ET_ok);
  _ptr->setSleepLinearVelocity(threshold);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::get_sleep_linear_velocity
//       Access: Published
//  Description: Returns the linear velocity below which an soft
//               body may go to sleep. cloths whose linear velocity
//               is above this threshold will not be put to sleep.
////////////////////////////////////////////////////////////////////
float PhysxCloth::
get_sleep_linear_velocity() const {

  nassertr(_error_type == ET_ok, 0.0f);
  return _ptr->getSleepLinearVelocity();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::attach_vertex_to_global_pos
//       Access: Published
//  Description: Attaches a cloth vertex to a position in world
//               space.
////////////////////////////////////////////////////////////////////
void PhysxCloth::
attach_vertex_to_global_pos(unsigned int vertexId, LPoint3f const &pos) {

  nassertv(_error_type == ET_ok);
  nassertv(!pos.is_nan());

  _ptr->attachVertexToGlobalPosition(vertexId, PhysxManager::point3_to_nxVec3(pos));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::attach_to_shape
//       Access: Published
//  Description: Attaches the cloth to a shape. All cloth points
//               currently inside the shape are attached.
//
//               This method only works with primitive and convex
//               shapes. Since the inside of a general triangle mesh
//               is not clearly defined.
////////////////////////////////////////////////////////////////////
void PhysxCloth::
attach_to_shape(PhysxShape *shape) {

  nassertv(_error_type == ET_ok);
  nassertv(shape);

  NxU32 attachmentFlags = 0; // --TODO--
  _ptr->attachToShape(shape->ptr(), attachmentFlags);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::attach_to_colliding_shapes
//       Access: Published
//  Description: Attaches the cloth to all shapes, currently
//               colliding. 
//
//               This method only works with primitive and convex
//               shapes. Since the inside of a general triangle mesh
//               is not clearly defined.
////////////////////////////////////////////////////////////////////
void PhysxCloth::
attach_to_colliding_shapes() {

  nassertv(_error_type == ET_ok);

  NxU32 attachmentFlags = 0; // --TODO--
  _ptr->attachToCollidingShapes(attachmentFlags);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::detach_from_shape
//       Access: Published
//  Description: Detaches the cloth from a shape it has been
//               attached to before. 
//
//               If the cloth has not been attached to the shape
//               before, the call has no effect.
////////////////////////////////////////////////////////////////////
void PhysxCloth::
detach_from_shape(PhysxShape *shape) {

  nassertv(_error_type == ET_ok);
  nassertv(shape);

  _ptr->detachFromShape(shape->ptr());
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::free_vertex
//       Access: Published
//  Description: Frees a previously attached cloth point.
////////////////////////////////////////////////////////////////////
void PhysxCloth::
free_vertex(unsigned int vertexId) {

  nassertv(_error_type == ET_ok);
  _ptr->freeVertex(vertexId);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::attach_vertex_to_shape
//       Access: Published
//  Description: Attaches a cloth vertex to a local position within
//               a shape.
////////////////////////////////////////////////////////////////////
void PhysxCloth::
attach_vertex_to_shape(unsigned int vertexId, PhysxShape *shape, LPoint3f const &localPos) {

  nassertv(_error_type == ET_ok);
  nassertv(!localPos.is_nan());
  nassertv(shape);

  NxU32 attachmentFlags = 0; // --TODO--
  _ptr->attachVertexToShape(vertexId, shape->ptr(),
                            PhysxManager::point3_to_nxVec3(localPos),
                            attachmentFlags);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::get_vertex_attachment_status
//       Access: Published
//  Description: Return the attachment status of the given vertex.
////////////////////////////////////////////////////////////////////
PhysxEnums::PhysxVertexAttachmentStatus PhysxCloth::
get_vertex_attachment_status(unsigned int vertexId) const {

  nassertr(_error_type == ET_ok, VAS_none);
  // --TODO-- nassertr(vertexId < _ptr->getNumberOfParticles(), VAS_none);

  return (PhysxVertexAttachmentStatus) _ptr->getVertexAttachmentStatus(vertexId);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::get_vertex_attachment_shape
//       Access: Published
//  Description: Returns the pointer to an attached shape pointer
//               of the given vertex. If the vertex is not attached
//               or attached to a global position, NULL is returned.
////////////////////////////////////////////////////////////////////
PhysxShape *PhysxCloth::
get_vertex_attachment_shape(unsigned int vertexId) const {

  nassertr(_error_type == ET_ok, NULL);
  // --TODO-- nassertr(vertexId < _ptr->getNumberOfParticles(), NULL);

  NxShape *shapePtr = _ptr->getVertexAttachmentShape(vertexId);
  PhysxShape *shape = shapePtr ? (PhysxShape *)(shapePtr->userData) : NULL;

  return shape;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::get_vertex_attachment_pos
//       Access: Published
//  Description: Returns the attachment position of the given
//               vertex. If the vertex is attached to shape, the
//               position local to the shape's pose is returned. If
//               the vertex is not attached, the return value is
//               undefined.
////////////////////////////////////////////////////////////////////
LPoint3f PhysxCloth::
get_vertex_attachment_pos(unsigned int vertexId) const {

  nassertr(_error_type == ET_ok, LPoint3f::zero());
  // --TODO-- nassertr(vertexId < _ptr->getNumberOfParticles(), LPoint3f::zero());

  return PhysxManager::nxVec3_to_point3(_ptr->getVertexAttachmentPosition(vertexId));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::set_external_acceleration
//       Access: Published
//  Description: Sets an external acceleration which affects all non
//               attached particles of the cloth.
////////////////////////////////////////////////////////////////////
void PhysxCloth::
set_external_acceleration(LVector3f const &acceleration) {

  nassertv(_error_type == ET_ok);
  nassertv_always(!acceleration.is_nan());

  _ptr->setExternalAcceleration(PhysxManager::vec3_to_nxVec3(acceleration));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::set_wind_acceleration
//       Access: Published
//  Description: Sets an acceleration acting normal to the cloth
//               surface at each vertex.
////////////////////////////////////////////////////////////////////
void PhysxCloth::
set_wind_acceleration(LVector3f const &acceleration) {

  nassertv(_error_type == ET_ok);
  nassertv_always(!acceleration.is_nan());

  _ptr->setWindAcceleration(PhysxManager::vec3_to_nxVec3(acceleration));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::get_external_acceleration
//       Access: Published
//  Description: Retrieves the external acceleration which affects
//               all non attached particles of the cloth.
////////////////////////////////////////////////////////////////////
LVector3f PhysxCloth::
get_external_acceleration() const {

  nassertr(_error_type == ET_ok, LVector3f::zero());
  return PhysxManager::nxVec3_to_vec3(_ptr->getExternalAcceleration());
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::get_wind_acceleration
//       Access: Published
//  Description: Retrieves the acceleration acting normal to the
//               cloth surface at each vertex
////////////////////////////////////////////////////////////////////
LVector3f PhysxCloth::
get_wind_acceleration() const {

  nassertr(_error_type == ET_ok, LVector3f::zero());
  return PhysxManager::nxVec3_to_vec3(_ptr->getWindAcceleration());
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::add_force_at_vertex
//       Access: Published
//  Description: Applies a force (or impulse) defined in the 
//               global coordinate frame, to a particular vertex
//               of the cloth.
////////////////////////////////////////////////////////////////////
void PhysxCloth::
add_force_at_vertex(LVector3f const &force, int vertexId, PhysxForceMode mode) {

  nassertv(_error_type == ET_ok);
  _ptr->addForceAtVertex(PhysxManager::vec3_to_nxVec3(force),
                         vertexId,
                         (NxForceMode) mode);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::add_force_at_pos
//       Access: Published
//  Description: Applies a radial force (or impulse) at a
//               particular position. All vertices within radius
//               will be affected with a quadratic drop-off. 
////////////////////////////////////////////////////////////////////
void PhysxCloth::
add_force_at_pos(LPoint3f const &pos, float magnitude, float radius, PhysxForceMode mode) {

  nassertv(_error_type == ET_ok);
  _ptr->addForceAtPos(PhysxManager::point3_to_nxVec3(pos),
                      magnitude,
                      radius,
                      (NxForceMode) mode);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCloth::add_directed_force_at_pos
//       Access: Published
//  Description: Applies a directed force (or impulse) at a
//               particular position. All vertices within radius
//               will be affected with a quadratic drop-off.  
////////////////////////////////////////////////////////////////////
void PhysxCloth::
add_directed_force_at_pos(LPoint3f const &pos, LVector3f const &force, float radius, PhysxForceMode mode) {

  nassertv(_error_type == ET_ok);
  _ptr->addDirectedForceAtPos(PhysxManager::point3_to_nxVec3(pos),
                              PhysxManager::vec3_to_nxVec3(force),
                              radius,
                              (NxForceMode) mode);
}

