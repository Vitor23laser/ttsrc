// Filename: mayaToEgg_server.h
// Adapted by: cbrunner (09Nov09)
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

#ifndef MAYATOEGGSERVER_H
#define MAYATOEGGSERVER_H

#include "pandatoolbase.h"
#include "somethingToEgg.h"
#include "mayaToEggConverter.h"
#include "queuedConnectionManager.h"
#include "queuedConnectionListener.h"
#include "queuedConnectionReader.h"
#include "connectionWriter.h"

////////////////////////////////////////////////////////////////////
//       Class : MayaToEggServer
// Description :
////////////////////////////////////////////////////////////////////
class MayaToEggServer : public SomethingToEgg {
public:
  MayaToEggServer();
  ~MayaToEggServer();

  void run();
  void poll();

  void add_reader(Connection *connection, QueuedConnectionReader *reader);
  void remove_reader(Connection *connection, QueuedConnectionReader *reader);
  QueuedConnectionManager *qManager;
  QueuedConnectionListener *qListener;
  QueuedConnectionReader *qReader;
  ConnectionWriter *cWriter;
  MayaToEggConverter *dummy;


protected:
  static bool dispatch_transform_type(const string &opt, const string &arg, void *var);
  typedef pset< PT(Connection) > Clients;
  Clients _clients;

  int _verbose;
  bool _polygon_output;
  double _polygon_tolerance;
  bool _respect_maya_double_sided;
  bool _suppress_vertex_color;
  bool _keep_all_uvsets;
  bool _round_uvs;
  bool _texture_copy;
  Filename _texture_out_dir;
  bool _legacy_shader;

  MayaToEggConverter::TransformType _transform_type;
  vector_string _subroots; 
  vector_string _subsets;
  vector_string _excludes;
  vector_string _ignore_sliders;
  vector_string _force_joints;
};

#endif
