// Filename: eggVertex.I
// Created by:  drose (16Jan99)
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



////////////////////////////////////////////////////////////////////
//     Function: EggVertex::get_pool
//       Access: Published
//  Description: Returns the vertex pool this vertex belongs in.  This
//               may be NULL if the vertex has not been added to a
//               pool.
////////////////////////////////////////////////////////////////////
INLINE EggVertexPool *EggVertex::
get_pool() const {
  return _pool;
}

////////////////////////////////////////////////////////////////////
//     Function: EggVertex::is_forward_reference
//       Access: Published
//  Description: Returns true if the vertex is a forward reference to
//               some vertex that hasn't been defined yet.  In this
//               case, the vertex may not have any properties filled
//               in yet.
//
//               This can only happen if you implicitly create a
//               vertex via EggVertexPool::get_forward_vertex().
//               Presumably, when the vertex pool is later filled in,
//               this vertex will be replaced with real data.
////////////////////////////////////////////////////////////////////
INLINE bool EggVertex::
is_forward_reference() const {
  return _forward_reference;
}

////////////////////////////////////////////////////////////////////
//     Function: EggVertex::set_pos
//       Access: Published
//  Description: Sets the vertex position.  This variant sets the
//               vertex to a one-dimensional value.
////////////////////////////////////////////////////////////////////
INLINE void EggVertex::
set_pos(double pos) {
  _num_dimensions = 1;
  _pos.set(pos, 0.0, 0.0, 1.0);
}


////////////////////////////////////////////////////////////////////
//     Function: EggVertex::set_pos
//       Access: Published
//  Description: Sets the vertex position.  This variant sets the
//               vertex to a two-dimensional value.
////////////////////////////////////////////////////////////////////
INLINE void EggVertex::
set_pos(const LPoint2d &pos) {
  _num_dimensions = 2;
  _pos.set(pos[0], pos[1], 0.0, 1.0);
}


////////////////////////////////////////////////////////////////////
//     Function: EggVertex::set_pos
//       Access: Published
//  Description: Sets the vertex position.  This variant sets the
//               vertex to a three-dimensional value.
////////////////////////////////////////////////////////////////////
INLINE void EggVertex::
set_pos(const LPoint3d &pos) {
  _num_dimensions = 3;
  _pos.set(pos[0], pos[1], pos[2], 1.0);
}


////////////////////////////////////////////////////////////////////
//     Function: EggVertex::set_pos
//       Access: Published
//  Description: Sets the vertex position.  This variant sets the
//               vertex to a four-dimensional value.
////////////////////////////////////////////////////////////////////
INLINE void EggVertex::
set_pos(const LPoint4d &pos) {
  _num_dimensions = 4;
  _pos = pos;
}


////////////////////////////////////////////////////////////////////
//     Function: EggVertex::set_pos4
//       Access: Published
//  Description: This special flavor of set_pos() sets the vertex as a
//               four-component value, but does not change the set
//               number of dimensions.  It's handy for retrieving the
//               vertex position via get_pos4, manipulating it, then
//               storing it back again, without worrying about the
//               number of dimensions it actually had.
////////////////////////////////////////////////////////////////////
INLINE void EggVertex::
set_pos4(const LPoint4d &pos) {
  _pos = pos;
}


////////////////////////////////////////////////////////////////////
//     Function: EggVertex::get_num_dimensions
//       Access: Published
//  Description: Returns the number of dimensions the vertex uses.
//               Usually this will be 3, but it may be 1, 2, 3, or 4.
////////////////////////////////////////////////////////////////////
INLINE int EggVertex::
get_num_dimensions() const {
  return _num_dimensions;
}


////////////////////////////////////////////////////////////////////
//     Function: EggVertex::get_pos1
//       Access: Published
//  Description: Only valid if get_num_dimensions() returns 1.
//               Returns the position as a one-dimensional value.
////////////////////////////////////////////////////////////////////
INLINE double EggVertex::
get_pos1() const {
  nassertr(_num_dimensions == 1, 0.0);
  return _pos[0];
}


////////////////////////////////////////////////////////////////////
//     Function: EggVertex::get_pos2
//       Access: Published
//  Description: Only valid if get_num_dimensions() returns 2.
//               Returns the position as a two-dimensional value.
////////////////////////////////////////////////////////////////////
INLINE LPoint2d EggVertex::
get_pos2() const {
  nassertr(_num_dimensions == 2, LPoint2d(0.0, 0.0));
  return LPoint2d(_pos[0], _pos[1]);
}


////////////////////////////////////////////////////////////////////
//     Function: EggVertex::get_pos3
//       Access: Published
//  Description: Valid if get_num_dimensions() returns 3 or 4.
//               Returns the position as a three-dimensional value.
////////////////////////////////////////////////////////////////////
INLINE Vertexd EggVertex::
get_pos3() const {
  nassertr(_num_dimensions == 3 || _num_dimensions == 4,
           LPoint3d(0.0, 0.0, 0.0));
  return Vertexd(_pos[0] / _pos[3], _pos[1] / _pos[3], _pos[2] / _pos[3]);
}


////////////////////////////////////////////////////////////////////
//     Function: EggVertex::get_pos4
//       Access: Published
//  Description: This is always valid, regardless of the value of
//               get_num_dimensions.  It returns the position as a
//               four-dimensional value.  If the pos has fewer than
//               four dimensions, this value represents the pos
//               extended into four-dimensional homogenous space,
//               e.g. by adding 1 as the fourth component.
////////////////////////////////////////////////////////////////////
INLINE LPoint4d EggVertex::
get_pos4() const {
  return _pos;
}

////////////////////////////////////////////////////////////////////
//     Function: EggVertex::has_uv
//       Access: Published
//  Description: Returns true if the vertex has an unnamed UV
//               coordinate pair, false otherwise.
//
//               This is the more restrictive interface, and is
//               generally useful only in the absence of
//               multitexturing; see has_uv(name) for the interface
//               that supports multitexturing.
////////////////////////////////////////////////////////////////////
INLINE bool EggVertex::
has_uv() const {
  return has_uv("");
}

////////////////////////////////////////////////////////////////////
//     Function: EggVertex::get_uv
//       Access: Published
//  Description: Returns the unnamed UV coordinate pair on the
//               vertex.  It is an error to call this if has_uv() has
//               returned false.
//
//               This is the more restrictive interface, and is
//               generally useful only in the absence of
//               multitexturing; see get_uv(name) for the interface
//               that supports multitexturing.
////////////////////////////////////////////////////////////////////
INLINE TexCoordd EggVertex::
get_uv() const {
  nassertr(has_uv(), TexCoordd::zero());
  return get_uv("");
}

////////////////////////////////////////////////////////////////////
//     Function: EggVertex::set_uv
//       Access: Published
//  Description: Replaces the unnamed UV coordinate pair on the vertex
//               with the indicated value.
//
//               This is the more restrictive interface, and is
//               generally useful only in the absence of
//               multitexturing; see set_uv(name, uv) for the
//               interface that supports multitexturing.
////////////////////////////////////////////////////////////////////
INLINE void EggVertex::
set_uv(const TexCoordd &uv) {
  set_uv("", uv);
}

////////////////////////////////////////////////////////////////////
//     Function: EggVertex::clear_uv
//       Access: Published
//  Description: Removes all UV coordinate pairs from the vertex.
////////////////////////////////////////////////////////////////////
INLINE void EggVertex::
clear_uv() {
  _uv_map.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: EggVertex::uv_begin
//       Access: Public
//  Description: Returns an iterator that allows walking through the
//               complete set of named UV's on the vertex.
//
//               This interface is not safe to use outside of
//               PANDAEGG.DLL.
////////////////////////////////////////////////////////////////////
INLINE EggVertex::const_uv_iterator EggVertex::
uv_begin() const {
  return _uv_map.begin();
}

////////////////////////////////////////////////////////////////////
//     Function: EggVertex::uv_end
//       Access: Public
//  Description: Returns an iterator that allows walking through the
//               complete set of named UV's on the vertex.
//
//               This interface is not safe to use outside of
//               PANDAEGG.DLL.
////////////////////////////////////////////////////////////////////
INLINE EggVertex::const_uv_iterator EggVertex::
uv_end() const {
  return _uv_map.end();
}

////////////////////////////////////////////////////////////////////
//     Function: EggVertex::uv_size
//       Access: Public
//  Description: Returns the number of named UV's on the vertex.
////////////////////////////////////////////////////////////////////
INLINE EggVertex::uv_size_type EggVertex::
uv_size() const {
  return _uv_map.size();
}

////////////////////////////////////////////////////////////////////
//     Function: EggVertex::get_index
//       Access: Published
//  Description: Returns the index number of the vertex within its
//               pool.
////////////////////////////////////////////////////////////////////
INLINE int EggVertex::
get_index() const {
  return _index;
}

////////////////////////////////////////////////////////////////////
//     Function: EggVertex::set_external_index
//       Access: Published
//  Description: Sets a special index number that is associated with
//               the EggVertex (but is not written to the egg file).
//               This number is not interpreted by any egg code; it is
//               simply maintained along with the vertex.  It *is*
//               used to differentiate otherwise identical vertices in
//               EggVertexPool::create_unique_vertex(), however.
//
//               The intention of this number is as an aid for file
//               converters, to associate an EggVertex back to the
//               index number of the original source vertex.
////////////////////////////////////////////////////////////////////
INLINE void EggVertex::
set_external_index(int external_index) {
  _external_index = external_index;
}

////////////////////////////////////////////////////////////////////
//     Function: EggVertex::get_external_index
//       Access: Published
//  Description: Returns the number set by set_external_index().  See
//               set_external_index().
////////////////////////////////////////////////////////////////////
INLINE int EggVertex::
get_external_index() const {
  return _external_index;
}

////////////////////////////////////////////////////////////////////
//     Function: EggVertex::sorts_less_than
//       Access: Published
//  Description: An ordering operator to compare two vertices for
//               sorting order.  This imposes an arbitrary ordering
//               useful to identify unique vertices.
////////////////////////////////////////////////////////////////////
INLINE bool EggVertex::
sorts_less_than(const EggVertex &other) const {
  return (compare_to(other) < 0);
}





////////////////////////////////////////////////////////////////////
//     Function: UniqueEggVertices::Function operator
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE bool UniqueEggVertices::
operator ()(const EggVertex *v1, const EggVertex *v2) const {
  return v1->sorts_less_than(*v2);
}

