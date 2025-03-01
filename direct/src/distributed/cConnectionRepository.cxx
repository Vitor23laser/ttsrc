// Filename: cConnectionRepository.cxx
// Created by:  drose (17May04)
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

#include "cConnectionRepository.h"
#include "dcmsgtypes.h"
#include "dcClass.h"
#include "dcPacker.h"

#include "config_distributed.h"
#include "config_downloader.h"
#include "httpChannel.h"
#include "urlSpec.h"
#include "datagramIterator.h"
#include "throw_event.h"
#include "pStatTimer.h"
#include "stringStream.h"

#ifdef HAVE_PYTHON
#ifndef CPPPARSER
#include "py_panda.h"  
IMPORT_THIS struct   Dtool_PyTypedObject Dtool_DatagramIterator;
IMPORT_THIS struct   Dtool_PyTypedObject Dtool_DCClass;
#endif
#endif

const string CConnectionRepository::_overflow_event_name = "CRDatagramOverflow";

#ifndef CPPPARSER
PStatCollector CConnectionRepository::_update_pcollector("App:Show code:readerPollTask:Update");
#endif  // CPPPARSER

////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::Constructor
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
CConnectionRepository::
CConnectionRepository(bool has_owner_view, bool threaded_net) :
  _lock("CConnectionRepository::_lock"),
#ifdef HAVE_PYTHON
  _python_repository(NULL),
  _python_ai_datagramiterator(NULL),
#endif
#ifdef HAVE_OPENSSL
  _http_conn(NULL),
#endif
#ifdef HAVE_NET
  _cw(&_qcm, threaded_net ? 1 : 0),
  _qcr(&_qcm, threaded_net ? 1 : 0),
#endif
#ifdef WANT_NATIVE_NET
  _bdc(4096000,4096000,1400),
  _native(false),
#endif
  _client_datagram(true),
  _handle_datagrams_internally(handle_datagrams_internally),
  _track_clsends(false),
  _simulated_disconnect(false),
  _verbose(distributed_cat.is_spam()),
  _time_warning(0.0),
//  _msg_channels(),
  _msg_sender(0),
  _msg_type(0),
  _has_owner_view(has_owner_view),
  _handle_c_updates(true),
  _want_message_bundling(true),
  _bundling_msgs(0),
  _in_quiet_zone(0)
{
#if defined(HAVE_NET) && defined(SIMULATE_NETWORK_DELAY)
  if (min_lag != 0.0 || max_lag != 0.0) {
    _qcr.start_delay(min_lag, max_lag);
  }
#endif
  _tcp_header_size = tcp_header_size;

#ifdef HAVE_PYTHON
  PyObject *  PyDitterator = DTool_CreatePyInstance(&_di,Dtool_DatagramIterator,false,false);
  if(PyDitterator != NULL)
      _python_ai_datagramiterator = Py_BuildValue("(O)",PyDitterator);
#endif

}

////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::Destructor
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
CConnectionRepository::
~CConnectionRepository() {
  disconnect();
}

////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::set_tcp_header_size
//       Access: Public
//  Description: Sets the header size of TCP packets.  At the present,
//               legal values for this are 0, 2, or 4; this specifies
//               the number of bytes to use encode the datagram length
//               at the start of each TCP datagram.  Sender and
//               receiver must independently agree on this.
////////////////////////////////////////////////////////////////////
void CConnectionRepository::
set_tcp_header_size(int tcp_header_size) {
  _tcp_header_size = tcp_header_size;

#ifdef HAVE_OPENSSL
  if (_http_conn != (SocketStream *)NULL) {
    _http_conn->set_tcp_header_size(tcp_header_size);
  }
#endif

#ifdef HAVE_NET
  _cw.set_tcp_header_size(tcp_header_size);
  _qcr.set_tcp_header_size(tcp_header_size);
#endif
}

#ifdef HAVE_OPENSSL
////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::set_connection_http
//       Access: Published
//  Description: Once a connection has been established via the HTTP
//               interface, gets the connection and uses it.  The
//               supplied HTTPChannel object must have a connection
//               available via get_connection().
////////////////////////////////////////////////////////////////////
void CConnectionRepository::
set_connection_http(HTTPChannel *channel) {
  ReMutexHolder holder(_lock);

  disconnect();
  nassertv(channel->is_connection_ready());
  _http_conn = channel->get_connection();
  _http_conn->set_tcp_header_size(_tcp_header_size);
#ifdef SIMULATE_NETWORK_DELAY
  if (min_lag != 0.0 || max_lag != 0.0) {
    _http_conn->start_delay(min_lag, max_lag);
  }
#endif
}
#endif  // HAVE_OPENSSL

#ifdef HAVE_OPENSSL
////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::get_stream
//       Access: Published
//  Description: Returns the SocketStream that internally represents
//               the already-established HTTP connection.  Returns
//               NULL if there is no current HTTP connection.
////////////////////////////////////////////////////////////////////
SocketStream *CConnectionRepository::
get_stream() {
  ReMutexHolder holder(_lock);

  return _http_conn;
}
#endif  // HAVE_OPENSSL


#ifdef HAVE_NET
////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::try_connect_net
//       Access: Published
//  Description: Uses Panda's "net" library to try to connect to the
//               server and port named in the indicated URL.  Returns
//               true if successful, false otherwise.
////////////////////////////////////////////////////////////////////
bool CConnectionRepository::
try_connect_net(const URLSpec &url) {
  ReMutexHolder holder(_lock);

  disconnect();

  _net_conn = 
    _qcm.open_TCP_client_connection(url.get_server(), url.get_port(),
                                    game_server_timeout_ms);

  if (_net_conn != (Connection *)NULL) {
    _net_conn->set_no_delay(true);
    _qcr.add_connection(_net_conn);
    return true;
  }

  return false;
}
#endif  // HAVE_NET

#ifdef WANT_NATIVE_NET
////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::connect_native
//       Access: Published
//  Description: Connects to the server using Panda's low-level and
//               fast "native net" library.
////////////////////////////////////////////////////////////////////
bool CConnectionRepository::
connect_native(const URLSpec &url) {
  ReMutexHolder holder(_lock);

  _native=true;
  Socket_Address addr;
  addr.set_host(url.get_server(),url.get_port());
  _bdc.ClearAddresses();
  _bdc.AddAddress(addr);
  return _bdc.DoConnect();
}

#endif //WANT NATIVE NET

#ifdef SIMULATE_NETWORK_DELAY
////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::start_delay
//       Access: Published
//  Description: Enables a simulated network latency.  All datagrams
//               received from this point on will be held for a random
//               interval of least min_delay seconds, and no more than
//               max_delay seconds, before being visible.  It is as if
//               datagrams suddenly took much longer to arrive.
//
//               This should *only* be called if the underlying socket
//               is non-blocking.  If you call this on a blocking
//               socket, it will force all datagrams to be held up
//               until the socket closes.
//
//               This has no effect if the connection method is via
//               the "native net" library.
////////////////////////////////////////////////////////////////////
void CConnectionRepository::
start_delay(double min_delay, double max_delay) {
  ReMutexHolder holder(_lock);

  if (min_delay != 0.0 || max_delay != 0.0) {
#ifdef HAVE_NET
    _qcr.start_delay(min_delay, max_delay);
#endif  // HAVE_NET
#ifdef HAVE_OPENSSL
    if (_http_conn != (SocketStream *)NULL) {
      _http_conn->start_delay(min_delay, max_delay);
    }
#endif  // HAVE_OPENSSL
  } else {
    stop_delay();
  }
}
#endif  // SIMULATE_NETWORK_DELAY

#ifdef SIMULATE_NETWORK_DELAY
////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::stop_delay
//       Access: Published
//  Description: Disables the simulated network latency started by a
//               previous call to start_delay().  Datagrams will once
//               again be visible as soon as they are received.
////////////////////////////////////////////////////////////////////
void CConnectionRepository::
stop_delay() {
  ReMutexHolder holder(_lock);

#ifdef HAVE_NET
  _qcr.stop_delay();
#endif  // HAVE_NET
#ifdef HAVE_OPENSSL
  if (_http_conn != (SocketStream *)NULL) {
    _http_conn->stop_delay();
  }
#endif  // HAVE_OPENSSL
}
#endif  // SIMULATE_NETWORK_DELAY

////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::check_datagram
//       Access: Published
//  Description: Returns true if a new datagram is available, false
//               otherwise.  If the return value is true, the new
//               datagram may be retrieved via get_datagram(), or
//               preferably, with get_datagram_iterator() and
//               get_msg_type().
////////////////////////////////////////////////////////////////////
bool CConnectionRepository::
check_datagram() {
  ReMutexHolder holder(_lock);

  if (_simulated_disconnect) {
    return false;
  }
  #ifdef WANT_NATIVE_NET
  if(_native)
    _bdc.Flush();
  #endif //WANT_NATIVE_NET

  while (do_check_datagram()) {
    if (get_verbose()) {
      describe_message(nout, "RECV", _dg);
    }

    // Start breaking apart the datagram.
    _di = DatagramIterator(_dg);

    if (!_client_datagram) {
      unsigned char  wc_cnt;
      wc_cnt = _di.get_uint8();
      _msg_channels.clear();
      for (unsigned char lp1 = 0; lp1 < wc_cnt; lp1++) {
        CHANNEL_TYPE  schan  = _di.get_uint64();
        _msg_channels.push_back(schan);
      }
      _msg_sender = _di.get_uint64();
      
#ifdef HAVE_PYTHON
      // For now, we need to stuff this field onto the Python
      // structure, to support legacy code that expects to find it
      // there.
      if (_python_repository != (PyObject *)NULL) {
#if defined(HAVE_THREADS) && !defined(SIMPLE_THREADS)
        PyGILState_STATE gstate;
        gstate = PyGILState_Ensure();
#endif
        PyObject *value = PyLong_FromUnsignedLongLong(_msg_sender);
        PyObject_SetAttrString(_python_repository, "msgSender", value);
        Py_DECREF(value);
#if defined(HAVE_THREADS) && !defined(SIMPLE_THREADS)
        PyGILState_Release(gstate);
#endif
      }
#endif  // HAVE_PYTHON
    }

    _msg_type = _di.get_uint16();
    // Is this a message that we can process directly?
    if (!_handle_datagrams_internally) {
      return true;
    }

    switch (_msg_type) {
#ifdef HAVE_PYTHON
    case CLIENT_OBJECT_UPDATE_FIELD:
    case STATESERVER_OBJECT_UPDATE_FIELD:
      if (_handle_c_updates) {
        if (_has_owner_view) {
          if (!handle_update_field_owner()) {
            return false;
          }
        } else {
          if (!handle_update_field()) {
            return false;
          }
        }
      } else {
        // Let the caller (Python) deal with this update.
        return true;
      }
      break;
#endif  // HAVE_PYTHON
      
    default:
      // Some unknown message; let the caller deal with it.
      return true;
    }
  }

  // No datagrams available.
  return false;
}




////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::is_connected
//       Access: Published
//  Description: Returns true if the connection to the gameserver is
//               established and still good, false if we are not
//               connected.  A false value means either (a) we never
//               successfully connected, (b) we explicitly called
//               disconnect(), or (c) we were connected, but the
//               connection was spontaneously lost.
////////////////////////////////////////////////////////////////////
bool CConnectionRepository::
is_connected() {
  ReMutexHolder holder(_lock);

#ifdef WANT_NATIVE_NET
  if(_native)
    return (_bdc.IsConnected());
#endif

#ifdef HAVE_NET
  if (_net_conn) {
    if (_qcm.reset_connection_available()) {
      PT(Connection) reset_connection;
      if (_qcm.get_reset_connection(reset_connection)) {
        _qcm.close_connection(reset_connection);
        if (reset_connection == _net_conn) {
          // Whoops, lost our connection.
          _net_conn = NULL;
          return false;
        }
      }
    }
    return true;
  }
#endif  // HAVE_NET

#ifdef HAVE_OPENSSL
  if (_http_conn) {
    if (!_http_conn->is_closed()) {
      return true;
    }

    // Connection lost.
    delete _http_conn;
    _http_conn = NULL;
  }
#endif  // HAVE_OPENSSL

  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::send_datagram
//       Access: Published
//  Description: Queues the indicated datagram for sending to the
//               server.  It may not get sent immediately if
//               collect_tcp is in effect; call flush() to guarantee
//               it is sent now.
////////////////////////////////////////////////////////////////////
bool CConnectionRepository::
send_datagram(const Datagram &dg) {
  ReMutexHolder holder(_lock);

  if (_simulated_disconnect) {
    distributed_cat.warning()
      << "Unable to send datagram during simulated disconnect.\n";
    return false;
  }

  if (get_verbose()) {
    describe_message(nout, "SEND", dg);
  }

  if (is_bundling_messages() && get_want_message_bundling()) {
    bundle_msg(dg);
    return true;
  }

#ifdef WANT_NATIVE_NET
  if(_native)
    return _bdc.SendMessage(dg);
#endif

#ifdef HAVE_NET
  if (_net_conn) {
    _cw.send(dg, _net_conn);
    return true;
  }
#endif  // HAVE_NET

#ifdef HAVE_OPENSSL
  if (_http_conn) {
    if (!_http_conn->send_datagram(dg)) {
      distributed_cat.warning()
        << "Could not send datagram.\n";
      return false;
    }

    return true;
  }
#endif  // HAVE_OPENSSL

  distributed_cat.warning()
    << "Unable to send datagram after connection is closed.\n";
  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::start_message_bundle
//       Access: Published
//  Description: Send a set of messages to the state server that will
//               be processed atomically.  For instance, you can do a
//               combined setLocation/setPos and prevent race
//               conditions where clients briefly get the setLocation
//               but not the setPos, because the state server hasn't
//               processed the setPos yet
////////////////////////////////////////////////////////////////////
void CConnectionRepository::
start_message_bundle() {
  ReMutexHolder holder(_lock);

  // store up network messages until sendMessageBundle is called
  // all updates in between must be sent from the same doId (updates
  // must all affect the same DistributedObject)
  // it is an error to call this again before calling sendMessageBundle
  if (get_verbose()) {
    nout << "CR::SEND:BUNDLE_START(" << _bundling_msgs << ")" << endl;
  }
  if (_bundling_msgs == 0) {
    _bundle_msgs.clear();
  }
  ++_bundling_msgs;
}

////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::send_message_bundle
//       Access: Published
//  Description: Send network messages queued up since
//               startMessageBundle was called.
////////////////////////////////////////////////////////////////////
void CConnectionRepository::
send_message_bundle(unsigned int channel, unsigned int sender_channel) {
  ReMutexHolder holder(_lock);
  nassertv(_bundling_msgs);

  --_bundling_msgs;

  if (get_verbose()) {
    nout << "CR::SEND:BUNDLE_FINISH(" << _bundling_msgs << ")" << endl;
  }

  // if _bundling_msgs ref count is zero, send the bundle out
  if (_bundling_msgs == 0 && get_want_message_bundling()) {
    Datagram dg;
    // add server header (see PyDatagram.addServerHeader)
    dg.add_int8(1);
    dg.add_uint64(channel);
    dg.add_uint64(sender_channel);
    dg.add_uint16(STATESERVER_BOUNCE_MESSAGE);
    // add each bundled message
    BundledMsgVector::const_iterator bmi;
    for (bmi = _bundle_msgs.begin(); bmi != _bundle_msgs.end(); bmi++) {
      dg.add_string(*bmi);
    }

    send_datagram(dg);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::abandon_message_bundles
//       Access: Published
//  Description: throw out any msgs that have been queued up for
//               message bundles
////////////////////////////////////////////////////////////////////
void CConnectionRepository::
abandon_message_bundles() {
  ReMutexHolder holder(_lock);

  nassertv(is_bundling_messages());
  _bundling_msgs = 0;
  _bundle_msgs.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::bundle_msg
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void CConnectionRepository::
bundle_msg(const Datagram &dg) {
  ReMutexHolder holder(_lock);

  nassertv(is_bundling_messages());
  _bundle_msgs.push_back(dg.get_message());
}

////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::consider_flush
//       Access: Published
//  Description: Sends the most recently queued data if enough time
//               has elapsed.  This only has meaning if
//               set_collect_tcp() has been set to true.
////////////////////////////////////////////////////////////////////
bool CConnectionRepository::
consider_flush() {
  ReMutexHolder holder(_lock);

  if (_simulated_disconnect) {
    return false;
  }

#ifdef WANT_NATIVE_NET
  if(_native)
    return true;  //Maybe we should just flush here for now?
#endif

#ifdef HAVE_NET
  if (_net_conn) {
    return _net_conn->consider_flush();
  }
#endif  // HAVE_NET

#ifdef HAVE_OPENSSL
  if (_http_conn) {
    return _http_conn->consider_flush();
  }
#endif  // HAVE_OPENSSL

  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::flush
//       Access: Published
//  Description: Sends the most recently queued data now.  This only
//               has meaning if set_collect_tcp() has been set to
//               true.
////////////////////////////////////////////////////////////////////
bool CConnectionRepository::
flush() {
  ReMutexHolder holder(_lock);

  if (_simulated_disconnect) {
    return false;
  }
  #ifdef WANT_NATIVE_NET
  if(_native)
    return _bdc.Flush();
  #endif

  #ifdef HAVE_NET
  if (_net_conn) {
    return _net_conn->flush();
  }
  #endif  // HAVE_NET

  #ifdef HAVE_OPENSSL
  if (_http_conn) {
    return _http_conn->flush();
  }
  #endif  // HAVE_OPENSSL

  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::disconnect
//       Access: Published
//  Description: Closes the connection to the server.
////////////////////////////////////////////////////////////////////
void CConnectionRepository::
disconnect() {
  ReMutexHolder holder(_lock);

  #ifdef WANT_NATIVE_NET
  if(_native) {
    _bdc.Reset();
    _bdc.ClearAddresses();
  }
  #endif
  #ifdef HAVE_NET
  if (_net_conn) {
    _qcm.close_connection(_net_conn);
    _net_conn = NULL;
  }
  #endif  // HAVE_NET

  #ifdef HAVE_OPENSSL
  if (_http_conn) {
    _http_conn->close();
    delete _http_conn;
    _http_conn = NULL;
  }
  #endif  // HAVE_OPENSSL

  _simulated_disconnect = false;
}

////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::shutdown
//       Access: Published
//  Description: May be called at application shutdown to ensure all
//               threads are cleaned up.
////////////////////////////////////////////////////////////////////
void CConnectionRepository::
shutdown() {
  disconnect();

  #ifdef HAVE_NET
  _cw.shutdown();
  _qcr.shutdown();
  #endif  // HAVE_NET
}

////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::do_check_datagram
//       Access: Private
//  Description: The private implementation of check_datagram(), this
//               gets one datagram if it is available.
////////////////////////////////////////////////////////////////////
bool CConnectionRepository::
do_check_datagram() {
  #ifdef WANT_NATIVE_NET
  if(_native) {
    return _bdc.GetMessage(_dg);
  }
  #endif
  #ifdef HAVE_NET
  if (_net_conn) {
    _net_conn->consider_flush();
    if (_qcr.get_overflow_flag()) {
      throw_event(get_overflow_event_name());
      _qcr.reset_overflow_flag();
    }
    return (_qcr.data_available() && _qcr.get_data(_dg));
  }
  #endif  // HAVE_NET

  #ifdef HAVE_OPENSSL
  if (_http_conn) {
    _http_conn->consider_flush();
    return _http_conn->receive_datagram(_dg);
  }
  #endif  // HAVE_OPENSSL


  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::handle_update_field
//       Access: Private
//  Description: Directly handles an update message on a field.
//               Python never touches the datagram; it just gets its
//               distributed method called with the appropriate
//               parameters.  Returns true if everything is ok, false
//               if there was an error processing the field's update
//               method.
////////////////////////////////////////////////////////////////////
bool CConnectionRepository::
handle_update_field() {
#ifdef HAVE_PYTHON
#if defined(HAVE_THREADS) && !defined(SIMPLE_THREADS)
  PyGILState_STATE gstate;
  gstate = PyGILState_Ensure();
#endif

  PStatTimer timer(_update_pcollector);
  unsigned int do_id = _di.get_uint32();
  if (_python_repository != (PyObject *)NULL) 
  {
    PyObject *doId2do =
      PyObject_GetAttrString(_python_repository, "doId2do");
    nassertr(doId2do != NULL, false);

    #ifdef USE_PYTHON_2_2_OR_EARLIER
    PyObject *doId = PyInt_FromLong(do_id);
    #else
    PyObject *doId = PyLong_FromUnsignedLong(do_id);
    #endif
    PyObject *distobj = PyDict_GetItem(doId2do, doId);
    Py_DECREF(doId);
    Py_DECREF(doId2do);

    if (distobj != NULL) {
      PyObject *dclass_obj = PyObject_GetAttrString(distobj, "dclass");
      nassertr(dclass_obj != NULL, false);


      PyObject *dclass_this = PyObject_GetAttrString(dclass_obj, "this");
      Py_DECREF(dclass_obj);
      nassertr(dclass_this != NULL, false);

      DCClass *dclass = (DCClass *)PyInt_AsLong(dclass_this);
      Py_DECREF(dclass_this);

      // If in quiet zone mode, throw update away unless distobj
      // has 'neverDisable' attribute set to non-zero
      if (_in_quiet_zone) {
        PyObject *neverDisable = PyObject_GetAttrString(distobj, "neverDisable");
        nassertr(neverDisable != NULL, false);

        unsigned int cNeverDisable = PyInt_AsLong(neverDisable);
        if (!cNeverDisable) {
          // in quiet zone and distobj is disable-able
          // drop update on the floor
#if defined(HAVE_THREADS) && !defined(SIMPLE_THREADS)
          PyGILState_Release(gstate);
#endif
          return true;
        }
      }

      // It's a good idea to ensure the reference count to distobj is
      // raised while we call the update method--otherwise, the update
      // method might get into trouble if it tried to delete the
      // object from the doId2do map.
      Py_INCREF(distobj);
      dclass->receive_update(distobj, _di); 
      Py_DECREF(distobj);
      
      if (PyErr_Occurred()) {
#if defined(HAVE_THREADS) && !defined(SIMPLE_THREADS)
        PyGILState_Release(gstate);
#endif
        return false;
      }
    }

  }

#if defined(HAVE_THREADS) && !defined(SIMPLE_THREADS)
  PyGILState_Release(gstate);
#endif
  #endif  // HAVE_PYTHON  
  return true;
}


////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::handle_update_field_owner
//       Access: Private
//  Description: Directly handles an update message on a field.
//               Supports 'owner' views of objects, separate from 'visible'
//               view, and forwards fields to the appropriate view(s) based
//               on DC flags.  Python never touches the datagram; it just
//               gets its distributed method called with the appropriate
//               parameters.  Returns true if everything is ok, false if
//               there was an error processing the field's update method.
////////////////////////////////////////////////////////////////////
bool CConnectionRepository::
handle_update_field_owner() {
#ifdef HAVE_PYTHON
#if defined(HAVE_THREADS) && !defined(SIMPLE_THREADS)
  PyGILState_STATE gstate;
  gstate = PyGILState_Ensure();
#endif

  PStatTimer timer(_update_pcollector);
  unsigned int do_id = _di.get_uint32();
  if (_python_repository != (PyObject *)NULL) {
    PyObject *doId2do =
      PyObject_GetAttrString(_python_repository, "doId2do");
    nassertr(doId2do != NULL, false);

    PyObject *doId2ownerView =
      PyObject_GetAttrString(_python_repository, "doId2ownerView");
    nassertr(doId2ownerView != NULL, false);

    #ifdef USE_PYTHON_2_2_OR_EARLIER
    PyObject *doId = PyInt_FromLong(do_id);
    #else
    PyObject *doId = PyLong_FromUnsignedLong(do_id);
    #endif

    // pass the update to the owner view first
    PyObject *distobjOV = PyDict_GetItem(doId2ownerView, doId);
    Py_DECREF(doId2ownerView);

    if (distobjOV != NULL) {
      PyObject *dclass_obj = PyObject_GetAttrString(distobjOV, "dclass");
      nassertr(dclass_obj != NULL, false);

      PyObject *dclass_this = PyObject_GetAttrString(dclass_obj, "this");
      Py_DECREF(dclass_obj);
      nassertr(dclass_this != NULL, false);

      DCClass *dclass = (DCClass *)PyInt_AsLong(dclass_this);
      Py_DECREF(dclass_this);

      // check if we should forward this update to the owner view
      DCPacker packer;
      packer.set_unpack_data(_di.get_remaining_bytes());
      int field_id = packer.raw_unpack_uint16();
      DCField *field = dclass->get_field_by_index(field_id);
      if (field->is_ownrecv()) {
        // It's a good idea to ensure the reference count to distobjOV is
        // raised while we call the update method--otherwise, the update
        // method might get into trouble if it tried to delete the
        // object from the doId2do map.
        Py_INCREF(distobjOV);
        // make a copy of the datagram iterator so that we can use the main
        // iterator for the non-owner update
        DatagramIterator _odi(_di);
        dclass->receive_update(distobjOV, _odi); 
        Py_DECREF(distobjOV);
      
        if (PyErr_Occurred()) {
#if defined(HAVE_THREADS) && !defined(SIMPLE_THREADS)
          PyGILState_Release(gstate);
#endif
          return false;
        }
      }
    }

    // now pass the update to the visible view
    PyObject *distobj = PyDict_GetItem(doId2do, doId);
    Py_DECREF(doId);
    Py_DECREF(doId2do);

    if (distobj != NULL) {
      PyObject *dclass_obj = PyObject_GetAttrString(distobj, "dclass");
      nassertr(dclass_obj != NULL, false);

      PyObject *dclass_this = PyObject_GetAttrString(dclass_obj, "this");
      Py_DECREF(dclass_obj);
      nassertr(dclass_this != NULL, false);

      DCClass *dclass = (DCClass *)PyInt_AsLong(dclass_this);
      Py_DECREF(dclass_this);

      // check if we should forward this update to the owner view
      DCPacker packer;
      packer.set_unpack_data(_di.get_remaining_bytes());
      int field_id = packer.raw_unpack_uint16();
      DCField *field = dclass->get_field_by_index(field_id);
      if (true) {//field->is_broadcast()) {
        // It's a good idea to ensure the reference count to distobj is
        // raised while we call the update method--otherwise, the update
        // method might get into trouble if it tried to delete the
        // object from the doId2do map.
        Py_INCREF(distobj);
        dclass->receive_update(distobj, _di); 
        Py_DECREF(distobj);
      
        if (PyErr_Occurred()) {
#if defined(HAVE_THREADS) && !defined(SIMPLE_THREADS)
          PyGILState_Release(gstate);
#endif
          return false;
        }
      }
    }
  }

#if defined(HAVE_THREADS) && !defined(SIMPLE_THREADS)
  PyGILState_Release(gstate);
#endif
#endif  // HAVE_PYTHON  

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: CConnectionRepository::describe_message
//       Access: Published
//  Description: Unpacks the message and reformats it for user
//               consumption, writing a description on the indicated
//               output stream.
////////////////////////////////////////////////////////////////////
void CConnectionRepository::
describe_message(ostream &out, const string &prefix, 
                 const Datagram &dg) const {
  DCPacker packer;
  
  packer.set_unpack_data(dg.get_message());
  CHANNEL_TYPE do_id;
  int msg_type;
  bool is_update = false;
  string full_prefix = "CR::" + prefix;

  if (!_client_datagram) 
  {
    unsigned char mcnt = packer.raw_unpack_uint8();
    for( ;mcnt > 0; mcnt--)
        packer.RAW_UNPACK_CHANNEL();  // msg_channel

    packer.RAW_UNPACK_CHANNEL();  // msg_sender
    msg_type = packer.raw_unpack_uint16();
    is_update = (msg_type == STATESERVER_OBJECT_UPDATE_FIELD);
    
  } else {
    msg_type = packer.raw_unpack_uint16();
    is_update = (msg_type == CLIENT_OBJECT_UPDATE_FIELD);
  }

  if (!is_update) {
    // figure out the name of the message
    // TODO: print out the arguments to the message
    string msgName;

    #ifdef HAVE_PYTHON
    if (_python_repository != (PyObject *)NULL) {
      PyObject *msgId = PyLong_FromLong(msg_type);
      nassertv(msgId != NULL);
      PyObject *methodName = PyString_FromString("_getMsgName");
      nassertv(methodName != NULL);

      PyObject *result = PyObject_CallMethodObjArgs(_python_repository, methodName,
						    msgId, NULL);
      nassertv(result != NULL);

      msgName += string(PyString_AsString(result));

      Py_DECREF(methodName);
      Py_DECREF(msgId);
      Py_DECREF(result);
    }
    #endif
    if (msgName.length() == 0) {
      msgName += "unknown message ";
      msgName += msg_type;
      msgName += "\n";
    }
    out << full_prefix << ":" << msgName << "\n";
    dg.dump_hex(out, 2);

  } else {
    // It's an update message.  Figure out what dclass the object is
    // based on its doId, so we can decode the rest of the message.
    do_id = packer.raw_unpack_uint32();

    int field_id = packer.raw_unpack_uint16();
    DCField *field = DCField::get_field_from_number(field_id);
    if (field == (DCField *)NULL) {
      out << "unknown field " << field_id << "\n";
    } else {
      DCClass *dclass = field->get_class();
      out << full_prefix <<
        ":" << dclass->get_name() << "(" << do_id << ").";
      out << field->get_name();
      packer.begin_unpack(field);
      packer.unpack_and_format(out);
      packer.end_unpack();
      out << "\n";
    }
  }
}




#ifdef HAVE_PYTHON
#ifdef WANT_NATIVE_NET

bool CConnectionRepository::network_based_reader_and_yielder(PyObject *PycallBackFunction,ClockObject &clock, float returnBy)
{
  ReMutexHolder holder(_lock);
    while(is_connected())
    {        
        check_datagram_ai(PycallBackFunction);
        if(is_connected())
            _bdc.Flush();
        float currentTime = clock.get_real_time();
        float dif_time = returnBy - currentTime;
        if(dif_time <= 0.001) // to avoi over runs..
            break;
        if(is_connected())
            _bdc.WaitForNetworkReadEvent(dif_time);
    }
    return false;
}

bool CConnectionRepository::check_datagram_ai(PyObject *PycallBackFunction)
{
  ReMutexHolder holder(_lock);
    // these could be static .. not 
  PyObject *doId2do = NULL; 
  float startTime =0;
  float endTime = 0;
  // this seems weird...here
  _bdc.Flush();
  while (_bdc.GetMessage(_dg))
  { 
      if (get_verbose()) 
          describe_message(nout, "RECV", _dg);

      if (_time_warning > 0) 
        startTime = ClockObject::get_global_clock()->get_real_time();

      // Start breaking apart the datagram.
      _di.assign(_dg);
      unsigned char  wc_cnt = _di.get_uint8();
      _msg_channels.clear();
      for(unsigned char lp1 = 0; lp1 < wc_cnt; lp1++)
          _msg_channels.push_back(_di.get_uint64());

      _msg_sender = _di.get_uint64();
      _msg_type = _di.get_uint16();

      if( _msg_type == STATESERVER_OBJECT_UPDATE_FIELD)
      {
          if(doId2do == NULL)
          {
              // this is my attemp to take it out of the inner loop  RHH
              doId2do =PyObject_GetAttrString(_python_repository, "doId2do");
              nassertr(doId2do != NULL, false);
          }

          if (!handle_update_field_ai(doId2do)) 
          {
              Py_XDECREF(doId2do);
              if (_time_warning > 0) {
                endTime = ClockObject::get_global_clock()->get_real_time(); 
                if ( _time_warning < (endTime - startTime)) {
                  nout << "msg " << _msg_type <<" from " << _msg_sender << " took "<<  (endTime-startTime) << "secs to process\n";
                  _dg.dump_hex(nout, 2);
                  describe_message(nout, "RECV", _dg);
                }
              }
              return false; 
          }
      }
      else
      {
          PyObject * result = PyEval_CallObject(PycallBackFunction, _python_ai_datagramiterator);
          if (PyErr_Occurred()) 
          {        
              Py_XDECREF(doId2do);
              if (_time_warning > 0) {
                endTime = ClockObject::get_global_clock()->get_real_time(); 
                if ( _time_warning < (endTime - startTime)) {
                  nout << "msg " << _msg_type <<" from " << _msg_sender << " took "<<  (endTime-startTime) << "secs to process\n";
                  _dg.dump_hex(nout, 2);
                  describe_message(nout, "RECV", _dg);
                }
              }
              return true;
          }
      }

      if (_time_warning > 0) {
        endTime = ClockObject::get_global_clock()->get_real_time(); 
        if ( _time_warning < (endTime - startTime)) {
          nout << "msg " << _msg_type <<" from " << _msg_sender << " took "<<  (endTime-startTime) << "secs to process\n";
          _dg.dump_hex(nout, 2);
          describe_message(nout, "RECV", _dg);
        }
      }
             
  }


  Py_XDECREF(doId2do);
  return false;
}

#endif  // #ifdef WANT_NATIVE_NET
#endif  // #ifdef HAVE_PYTHON


#ifdef HAVE_PYTHON
#ifdef WANT_NATIVE_NET


bool CConnectionRepository::handle_update_field_ai(PyObject *doId2do) 
{
  PStatTimer timer(_update_pcollector);
  unsigned int do_id = _di.get_uint32();
 
  PyObject *doId = PyLong_FromUnsignedLong(do_id);
  PyObject *distobj = PyDict_GetItem(doId2do, doId);
  Py_DECREF(doId);

  if (distobj != NULL)
  {
      PyObject *dclass_obj = PyObject_GetAttrString(distobj, "dclass");
      nassertr(dclass_obj != NULL, false);

      DCClass *dclass = NULL;
      DTOOL_Call_ExtractThisPointerForType(dclass_obj, &Dtool_DCClass, (void **) &dclass);
      if(dclass == NULL)
          return false;

      if (_track_clsends) {
        DatagramIterator di(_di);
        int field_id = di.get_uint16();
        DCField *field = dclass->get_field_by_index(field_id);
        if (field != (DCField *)NULL) {
          if (field->is_clsend() || field->is_ownsend()) {
            // need to look up sender avatar
            PyObject *senderId = PyLong_FromUnsignedLong(get_msg_sender() & 0xffffffff);
            PyObject *senderObj = PyDict_GetItem(doId2do, senderId);
            Py_DECREF(senderId);
            if (senderObj != NULL) {
              char *trackMethodName = "trackClientSendMsg";
              if (PyObject_HasAttrString(senderObj, trackMethodName)) {
                PyObject *func = PyObject_GetAttrString(senderObj, trackMethodName);
                if (func != (PyObject *)NULL) {
                  PyObject *args = Py_BuildValue("(s#)", _dg.get_message().c_str(), _dg.get_length());
                  if (args != (PyObject *)NULL) {
                    PyObject *result;
                    Py_INCREF(senderObj);
                    result = PyObject_CallObject(func, args);
                    Py_DECREF(senderObj);
                    Py_XDECREF(result);
                    Py_DECREF(args);
                  }
                  Py_DECREF(func);
                }
              }
            }
          }
        }
      }

      Py_INCREF(distobj);
      dclass->receive_update(distobj, _di); 
      Py_DECREF(distobj);

      if (PyErr_Occurred()) 
          return false;
  }
  return true;
}

#endif  // #ifdef WANT_NATIVE_NET
#endif  // #ifdef HAVE_PYTHON
