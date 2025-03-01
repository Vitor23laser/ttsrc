// Filename: physxMeshPool.cxx
// Created by:  enn0x (14Oct09)
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

#include "physxMeshPool.h"
#include "physxConvexMesh.h"
#include "physxTriangleMesh.h"
#include "physxClothMesh.h"

#include "physxFileStream.h"
#include "virtualFileSystem.h"

PhysxMeshPool::ConvexMeshes PhysxMeshPool::_convex_meshes;
PhysxMeshPool::TriangleMeshes PhysxMeshPool::_triangle_meshes;
PhysxMeshPool::ClothMeshes PhysxMeshPool::_cloth_meshes;
//PhysxMeshPool::SoftbodyMeshes PhysxMeshPool::_softbody_meshes;

////////////////////////////////////////////////////////////////////
//     Function: PhysxMeshPool::check_file
//       Access: Private
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxMeshPool::
check_filename(const Filename &fn) {

  if (!(VirtualFileSystem::get_global_ptr()->exists(fn))) {
    physx_cat.error() << "File does not exists: " << fn << endl;
    return false;
  }

  if (!(VirtualFileSystem::get_global_ptr()->is_regular_file(fn))) {
    physx_cat.error() << "Not a regular file: " << fn << endl;
    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxMeshPool::load_convex_mesh
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
PhysxConvexMesh *PhysxMeshPool::
load_convex_mesh(const Filename &fn) {

  if (!check_filename(fn)) return NULL;

  PhysxConvexMesh *mesh;

  ConvexMeshes::iterator it = _convex_meshes.find(fn);
  if (it == _convex_meshes.end()) {
    // Not found; load mesh.
    NxConvexMesh *meshPtr;
    PhysxFileStream stream = PhysxFileStream(fn, true);

    mesh = new PhysxConvexMesh();
    nassertr_always(mesh, NULL);

    NxPhysicsSDK *sdk = NxGetPhysicsSDK();
    nassertr_always(sdk, NULL);

    meshPtr = sdk->createConvexMesh(stream);
    nassertr_always(meshPtr, NULL);

    mesh->link(meshPtr);

    _convex_meshes.insert(ConvexMeshes::value_type(fn, mesh));
  }
  else {
    // Found; return previously loaded mesh.
    mesh = (*it).second;
  }

  return mesh;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxMeshPool::load_triangle_mesh
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
PhysxTriangleMesh *PhysxMeshPool::
load_triangle_mesh(const Filename &fn) {

  if (!check_filename(fn)) return NULL;

  PhysxTriangleMesh *mesh;

  TriangleMeshes::iterator it = _triangle_meshes.find(fn);
  if (it == _triangle_meshes.end()) {
    // Not found; load mesh.
    NxTriangleMesh *meshPtr;
    PhysxFileStream stream = PhysxFileStream(fn, true);

    mesh = new PhysxTriangleMesh();
    nassertr_always(mesh, NULL);

    NxPhysicsSDK *sdk = NxGetPhysicsSDK();
    nassertr_always(sdk, NULL);

    meshPtr = sdk->createTriangleMesh(stream);
    nassertr_always(meshPtr, NULL);

    mesh->link(meshPtr);

    _triangle_meshes.insert(TriangleMeshes::value_type(fn, mesh));
  }
  else {
    // Found; return previously loaded mesh.
    mesh = (*it).second;
  }

  return mesh;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxMeshPool::load_cloth_mesh
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
PhysxClothMesh *PhysxMeshPool::
load_cloth_mesh(const Filename &fn) {

  if (!check_filename(fn)) return NULL;

  PhysxClothMesh *mesh;

  ClothMeshes::iterator it = _cloth_meshes.find(fn);
  if (it == _cloth_meshes.end()) {
    // Not found; load mesh.
    NxClothMesh *meshPtr;
    PhysxFileStream stream = PhysxFileStream(fn, true);

    mesh = new PhysxClothMesh();
    nassertr_always(mesh, NULL);

    NxPhysicsSDK *sdk = NxGetPhysicsSDK();
    nassertr_always(sdk, NULL);

    meshPtr = sdk->createClothMesh(stream);
    nassertr_always(meshPtr, NULL);

    mesh->link(meshPtr);

    _cloth_meshes.insert(ClothMeshes::value_type(fn, mesh));
  }
  else {
    // Found; return previously loaded mesh.
    mesh = (*it).second;
  }

  return mesh;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxMeshPool::release_convex_mesh
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxMeshPool::
release_convex_mesh(PhysxConvexMesh *mesh) {

  ConvexMeshes::iterator it;
  for (it=_convex_meshes.begin(); it != _convex_meshes.end(); ++it) {
    if (mesh == (*it).second) {
      _convex_meshes.erase(it);
      return true;
    }
  }

  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxMeshPool::release_triangle_mesh
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxMeshPool::
release_triangle_mesh(PhysxTriangleMesh *mesh) {

  TriangleMeshes::iterator it;
  for (it=_triangle_meshes.begin(); it != _triangle_meshes.end(); ++it) {
    if (mesh == (*it).second) {
      _triangle_meshes.erase(it);
      return true;
    }
  }

  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxMeshPool::release_cloth_mesh
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxMeshPool::
release_cloth_mesh(PhysxClothMesh *mesh) {

  ClothMeshes::iterator it;
  for (it=_cloth_meshes.begin(); it != _cloth_meshes.end(); ++it) {
    if (mesh == (*it).second) {
      _cloth_meshes.erase(it);
      return true;
    }
  }

  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxMeshPool::list_content
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxMeshPool::
list_contents() {
  list_contents(nout);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxMeshPool::list_content
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxMeshPool::
list_contents(ostream &out) {

  out << "PhysX mesh pool contents:\n";

  // Convex meshes
  {
    ConvexMeshes::const_iterator it;
    for (it=_convex_meshes.begin(); it != _convex_meshes.end(); ++it) {
      Filename fn = (*it).first;
      PhysxConvexMesh *mesh = (*it).second;

      out << "  " << fn.get_fullpath()
          << " (convex mesh, " << mesh->ptr()->getReferenceCount() 
          << " references)" << endl;
    }
  }

  // Triangle meshes
  {
    TriangleMeshes::const_iterator it;
    for (it=_triangle_meshes.begin(); it != _triangle_meshes.end(); ++it) {
      Filename fn = (*it).first;
      PhysxTriangleMesh *mesh = (*it).second;

      out << "  " << fn.get_fullpath()
          << " (triangle mesh, " << mesh->ptr()->getReferenceCount() 
          << " references)\n";
    }
  }

  // Cloth meshes
  {
    ClothMeshes::const_iterator it;
    for (it=_cloth_meshes.begin(); it != _cloth_meshes.end(); ++it) {
      Filename fn = (*it).first;
      PhysxClothMesh *mesh = (*it).second;

      out << "  " << fn.get_fullpath()
          << " (cloth mesh, " << mesh->ptr()->getReferenceCount() 
          << " references)\n";
    }
  }

  // Summary
  NxPhysicsSDK *sdk = NxGetPhysicsSDK();

  out << "  Total number of convex meshes: " << sdk->getNbConvexMeshes()
      << " created, " << _convex_meshes.size() << " registred\n";

  out << "  Total number of triangle meshes: " << sdk->getNbTriangleMeshes() 
      << " created, " << _triangle_meshes.size() << " registred\n";

  out << "  Total number of cloth meshes: " << sdk->getNbClothMeshes() 
      << " created, " << _cloth_meshes.size() << " registred\n";
}

