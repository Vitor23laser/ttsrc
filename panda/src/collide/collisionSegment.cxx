// Filename: collisionSegment.cxx
// Created by:  drose (30Jan01)
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


#include "collisionSegment.h"
#include "collisionHandler.h"
#include "collisionEntry.h"
#include "config_collide.h"
#include "geom.h"
#include "lensNode.h"
#include "geomNode.h"
#include "lens.h"
#include "geometricBoundingVolume.h"
#include "datagram.h"
#include "datagramIterator.h"
#include "bamReader.h"
#include "bamWriter.h"
#include "geom.h"
#include "geomLines.h"
#include "boundingHexahedron.h"
#include "geomVertexWriter.h"

TypeHandle CollisionSegment::_type_handle;


////////////////////////////////////////////////////////////////////
//     Function: CollisionSegment::make_copy
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
CollisionSolid *CollisionSegment::
make_copy() {
  return new CollisionSegment(*this);
}

////////////////////////////////////////////////////////////////////
//     Function: CollisionSegment::test_intersection
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
PT(CollisionEntry) CollisionSegment::
test_intersection(const CollisionEntry &entry) const {
  return entry.get_into()->test_intersection_from_segment(entry);
}

////////////////////////////////////////////////////////////////////
//     Function: CollisionSegment::xform
//       Access: Public, Virtual
//  Description: Transforms the solid by the indicated matrix.
////////////////////////////////////////////////////////////////////
void CollisionSegment::
xform(const LMatrix4f &mat) {
  _a = _a * mat;
  _b = _b * mat;

  CollisionSolid::xform(mat);
}

////////////////////////////////////////////////////////////////////
//     Function: CollisionSegment::get_collision_origin
//       Access: Public, Virtual
//  Description: Returns the point in space deemed to be the "origin"
//               of the solid for collision purposes.  The closest
//               intersection point to this origin point is considered
//               to be the most significant.
////////////////////////////////////////////////////////////////////
LPoint3f CollisionSegment::
get_collision_origin() const {
  return get_point_a();
}

////////////////////////////////////////////////////////////////////
//     Function: CollisionSegment::output
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
void CollisionSegment::
output(ostream &out) const {
  out << "segment, a (" << _a << "), b (" << _b << ")";
}

////////////////////////////////////////////////////////////////////
//     Function: CollisionSegment::set_from_lens
//       Access: Public
//  Description: Accepts a LensNode and a 2-d point in the range
//               [-1,1].  Sets the CollisionSegment so that it begins at
//               the LensNode's near plane and extends to the
//               far plane, making it suitable for picking objects
//               from the screen given a camera and a mouse location.
//
//               Returns true if the point was acceptable, false
//               otherwise.
////////////////////////////////////////////////////////////////////
bool CollisionSegment::
set_from_lens(LensNode *camera, const LPoint2f &point) {
  Lens *proj = camera->get_lens();

  bool success = true;
  if (!proj->extrude(point, _a, _b)) {
    _a = LPoint3f::origin();
    _b = _a + LVector3f::forward();
    success = false;
  }

  mark_internal_bounds_stale();
  mark_viz_stale();

  return success;
}

////////////////////////////////////////////////////////////////////
//     Function: CollisionSegment::compute_internal_bounds
//       Access: Protected, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
PT(BoundingVolume) CollisionSegment::
compute_internal_bounds() const {

  LVector3f pdelta = _b - _a;

  // If p1 and p2 are sufficiently close, just put a sphere around
  // them.
  float d2 = pdelta.length_squared();
  if (d2 < collision_parabola_bounds_threshold * collision_parabola_bounds_threshold) {
    LPoint3f pmid = (_a + _b) * 0.5f;
    return new BoundingSphere(pmid, csqrt(d2) * 0.5f);
  }

  LMatrix4f from_segment;
  look_at(from_segment, pdelta, LPoint3f(0,0,1), CS_zup_right);
  from_segment.set_row(3, _a);
  
  float max_y =  sqrt(d2) + 0.01;
  PT(BoundingHexahedron) volume = 
    new BoundingHexahedron(LPoint3f(-0.01, max_y, -0.01), LPoint3f(0.01, max_y, -0.01),
                           LPoint3f(0.01, max_y, 0.01), LPoint3f(-0.01, max_y,  0.01),
                           LPoint3f(-0.01, -0.01, -0.01), LPoint3f(0.01, 0.01, -0.01),
                           LPoint3f(0.01, -0.01, 0.01), LPoint3f(-0.01, -0.01, 0.01));

  volume->xform(from_segment);
  return volume.p();
}

////////////////////////////////////////////////////////////////////
//     Function: CollisionSegment::fill_viz_geom
//       Access: Protected, Virtual
//  Description: Fills the _viz_geom GeomNode up with Geoms suitable
//               for rendering this solid.
////////////////////////////////////////////////////////////////////
void CollisionSegment::
fill_viz_geom() {
  if (collide_cat.is_debug()) {
    collide_cat.debug()
      << "Recomputing viz for " << *this << "\n";
  }

  PT(GeomVertexData) vdata = new GeomVertexData
    ("collision", GeomVertexFormat::get_v3cp(),
     Geom::UH_static);
  GeomVertexWriter vertex(vdata, InternalName::get_vertex());
  
  vertex.add_data3f(_a);
  vertex.add_data3f(_b);
  
  PT(GeomLines) line = new GeomLines(Geom::UH_static);
  line->add_next_vertices(2);
  line->close_primitive();
  
  PT(Geom) geom = new Geom(vdata);
  geom->add_primitive(line);
  
  _viz_geom->add_geom(geom, get_other_viz_state());
  _bounds_viz_geom->add_geom(geom, get_other_bounds_viz_state());
}

////////////////////////////////////////////////////////////////////
//     Function: CollisionSegment::register_with_read_factory
//       Access: Public, Static
//  Description: Tells the BamReader how to create objects of type
//               CollisionSegment.
////////////////////////////////////////////////////////////////////
void CollisionSegment::
register_with_read_factory() {
  BamReader::get_factory()->register_factory(get_class_type(), make_from_bam);
}

////////////////////////////////////////////////////////////////////
//     Function: CollisionSegment::write_datagram
//       Access: Public, Virtual
//  Description: Writes the contents of this object to the datagram
//               for shipping out to a Bam file.
////////////////////////////////////////////////////////////////////
void CollisionSegment::
write_datagram(BamWriter *manager, Datagram &dg) {
  CollisionSolid::write_datagram(manager, dg);
  _a.write_datagram(dg);
  _b.write_datagram(dg);
}

////////////////////////////////////////////////////////////////////
//     Function: CollisionSegment::make_from_bam
//       Access: Protected, Static
//  Description: This function is called by the BamReader's factory
//               when a new object of type CollisionSegment is encountered
//               in the Bam file.  It should create the CollisionSegment
//               and extract its information from the file.
////////////////////////////////////////////////////////////////////
TypedWritable *CollisionSegment::
make_from_bam(const FactoryParams &params) {
  CollisionSegment *node = new CollisionSegment();
  DatagramIterator scan;
  BamReader *manager;

  parse_params(params, scan, manager);
  node->fillin(scan, manager);

  return node;
}

////////////////////////////////////////////////////////////////////
//     Function: CollisionSegment::fillin
//       Access: Protected
//  Description: This internal function is called by make_from_bam to
//               read in all of the relevant data from the BamFile for
//               the new CollisionSegment.
////////////////////////////////////////////////////////////////////
void CollisionSegment::
fillin(DatagramIterator &scan, BamReader *manager) {
  CollisionSolid::fillin(scan, manager);
  _a.read_datagram(scan);
  _b.read_datagram(scan);
}
