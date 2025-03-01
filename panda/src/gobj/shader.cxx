// Filename: shader.cxx
// Created by: jyelon (01Sep05)
// Updated by: fperazzi, PandaSE(06Apr10)
// Updated by: fperazzi, PandaSE(29Apr10) (added SAT_sampler2dArray)
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

#include "pandabase.h"
#include "shader.h"
#include "preparedGraphicsObjects.h"
#include "virtualFileSystem.h"

#ifdef HAVE_CG
#include "Cg/cg.h"
#define JCG_PROFILE_GLSLV ((CGprofile)7007)
#define JCG_PROFILE_GLSLF ((CGprofile)7008)
#endif

TypeHandle Shader::_type_handle;
Shader::ShaderTable Shader::_load_table;
Shader::ShaderTable Shader::_make_table;
Shader::ShaderCaps Shader::_default_caps;
int Shader::_shaders_generated;
ShaderUtilization Shader::_shader_utilization = SUT_UNSPECIFIED;

////////////////////////////////////////////////////////////////////
//     Function: Shader::cp_report_error
//       Access: Public
//  Description: Generate an error message including a description
//               of the specified parameter.
////////////////////////////////////////////////////////////////////
void Shader::
cp_report_error(ShaderArgInfo &p, const string &msg) { 
  
  string vstr;
  if (p._varying) {
    vstr = "varying ";
  } else {
    vstr = "uniform ";
  }

  string dstr = "unknown ";
  if (p._direction == SAD_in) {
    dstr = "in ";
  } else if (p._direction == SAD_out) {
    dstr = "out ";
  } else if (p._direction == SAD_inout) {
    dstr = "inout ";
  }

  string tstr = "invalid ";
  switch (p._type) {
  case SAT_scalar:    tstr = "scalar "; break; 
  case SAT_vec1:      tstr = "vec1 "; break;     
  case SAT_vec2:      tstr = "vec2 "; break;        
  case SAT_vec3:      tstr = "vec3 "; break;        
  case SAT_vec4:      tstr = "vec4 "; break;        
  case SAT_mat1x1:    tstr = "mat1x1 "; break;      
  case SAT_mat1x2:    tstr = "mat1x2 "; break;      
  case SAT_mat1x3:    tstr = "mat1x3 "; break;      
  case SAT_mat1x4:    tstr = "mat1x4 "; break;      
  case SAT_mat2x1:    tstr = "mat2x1 "; break;      
  case SAT_mat2x2:    tstr = "mat2x2 "; break;      
  case SAT_mat2x3:    tstr = "mat2x3 "; break;      
  case SAT_mat2x4:    tstr = "mat2x4 "; break;      
  case SAT_mat3x1:    tstr = "mat3x1 "; break;      
  case SAT_mat3x2:    tstr = "mat3x2 "; break;      
  case SAT_mat3x3:    tstr = "mat3x3 "; break;       
  case SAT_mat3x4:    tstr = "mat3x4 "; break;       
  case SAT_mat4x1:    tstr = "mat4x1 "; break;       
  case SAT_mat4x2:    tstr = "mat4x2 "; break;       
  case SAT_mat4x3:    tstr = "mat4x3 "; break;       
  case SAT_mat4x4:    tstr = "mat4x4 "; break;      
  case SAT_sampler1d: tstr = "sampler1d "; break;  
  case SAT_sampler2d: tstr = "sampler2d "; break;   
  case SAT_sampler3d: tstr = "sampler3d "; break;   
  case SAT_sampler2dArray: tstr = "sampler2dArray "; break; 
  case SAT_samplercube:    tstr = "samplercube "; break; 
  default:                 tstr = "unknown "; break; 
  }
  
  string cstr = "invalid";
  switch (p._class) {
  case SAC_scalar:  cstr = "scalar ";  break;              
  case SAC_vector:  cstr = "vector ";  break;       
  case SAC_matrix:  cstr = "matrix ";  break;        
  case SAC_sampler: cstr = "sampler "; break;       
  case SAC_array:   cstr = "array ";   break;       
  default:          cstr = "unknown "; break; 
  }    

  Filename fn = get_filename(p._id._type);
  p._cat->error() << fn << ": " << vstr << dstr << tstr << 
    p._id._name << ": " << msg << "\n";
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::cp_errchk_parameter_words
//       Access: Public, Static
//  Description: Make sure the provided parameter contains
//               the specified number of words.  If not, print
//               error message and return false.
////////////////////////////////////////////////////////////////////
bool Shader::
cp_errchk_parameter_words(ShaderArgInfo &p, int len)
{
  vector_string words;
  tokenize(p._id._name, words, "_");
  if ((int)words.size() != len) {
    cp_report_error(p, "parameter name has wrong number of words");
    return false;
  }
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::cp_errchk_parameter_in
//       Access: Public, Static
//  Description: Make sure the provided parameter has the
//               'in' direction.  If not, print
//               error message and return false.
////////////////////////////////////////////////////////////////////
bool Shader::
cp_errchk_parameter_in(ShaderArgInfo &p)
{
  if (p._direction != SAD_in) {
    cp_report_error(p, "parameter should be declared 'in'");
    return false;
  }
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::cp_errchk_parameter_varying
//       Access: Public, Static
//  Description: Make sure the provided parameter has the
//               correct variance.  If not, print
//               error message and return false.
////////////////////////////////////////////////////////////////////
bool Shader::
cp_errchk_parameter_varying(ShaderArgInfo &p)
{
  if (!p._varying) {
    cp_report_error(p, "parameter should be declared 'varying'");
    return false;
  }
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::cp_errchk_parameter_uniform
//       Access: Public, Static
//  Description: Make sure the provided parameter has the
//               correct variance.  If not, print
//               error message and return false.
////////////////////////////////////////////////////////////////////
bool Shader::
cp_errchk_parameter_uniform(ShaderArgInfo &p)
{
  if (p._varying) {
    cp_report_error(p, "parameter should be declared 'uniform'");
    return false;
  }
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::cp_errchk_parameter_float
//       Access: Public, Static
//  Description: Make sure the provided parameter has
//               a floating point type.  If not, print
//               error message and return false.
////////////////////////////////////////////////////////////////////
bool Shader::
cp_errchk_parameter_float(ShaderArgInfo &p, int lo, int hi)
{
  int nfloat;
  switch (p._type) {
  case SAT_scalar: nfloat = 1; break;
  case SAT_vec2: nfloat = 2; break;
  case SAT_vec3: nfloat = 3; break;
  case SAT_vec4: nfloat = 4; break;
  case SAT_mat4x4: nfloat = 16; break;
  default: nfloat = 0; break;
  }
  if ((nfloat < lo)||(nfloat > hi)) {
    string msg = "wrong type for parameter:";
    cp_report_error(p, msg);
    return false;
  }
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::cp_errchk_parameter_ptr
//       Access: Public, Static
//  Description: 
////////////////////////////////////////////////////////////////////
bool Shader::
cp_errchk_parameter_ptr(ShaderArgInfo &p) {
  switch (p._class) {
  case SAC_scalar: return true;
  case SAC_vector: return true;
  case SAC_matrix: return true;
  case SAC_array:
    switch (p._subclass){
    case SAC_scalar: return true;
    case SAC_vector: return true;
    case SAC_matrix: return true;
    default:
      string msg = "unsupported array subclass."; 
      cp_report_error(p,msg);
      return false;
    }
  default:
    string msg = "unsupported class.";
    cp_report_error(p,msg);
    return false;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::cp_errchk_parameter_sampler
//       Access: Public, Static
//  Description: Make sure the provided parameter has
//               a texture type.  If not, print
//               error message and return false.
////////////////////////////////////////////////////////////////////
bool Shader::
cp_errchk_parameter_sampler(ShaderArgInfo &p)
{
  if ((p._type!=SAT_sampler1d)&&
      (p._type!=SAT_sampler2d)&&
      (p._type!=SAT_sampler3d)&&
      (p._type!=SAT_sampler2dArray)&&
      (p._type!=SAT_samplercube)) {
    cp_report_error(p, "parameter should have a 'sampler' type");
    return false;
  }
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::cp_parse_eol
//       Access: Public
//  Description: Make sure the next thing on the word list is EOL
////////////////////////////////////////////////////////////////////
bool Shader::
cp_parse_eol(ShaderArgInfo &p, vector_string &words, int &next) {
  if (words[next] != "") {
    cp_report_error(p, "Too many words in parameter");
    return false;
  }
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::cp_parse_delimiter
//       Access: Public
//  Description: Pop a delimiter ('to' or 'rel') from the word list.
////////////////////////////////////////////////////////////////////
bool Shader::
cp_parse_delimiter(ShaderArgInfo &p, vector_string &words, int &next) {
  if ((words[next] != "to")&&(words[next] != "rel")) {
    cp_report_error(p, "Keyword 'to' or 'rel' expected");
    return false;
  }
  next += 1;
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::cp_parse_non_delimiter
//       Access: Public
//  Description: Pop a non-delimiter word from the word list.
//               Delimiters are 'to' and 'rel.'
////////////////////////////////////////////////////////////////////
string Shader::
cp_parse_non_delimiter(vector_string &words, int &next) {
  const string &nword = words[next];
  if ((nword == "")||(nword == "to")||(nword == "rel")) {
    return "";
  }
  next += 1;
  return nword;
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::cp_parse_coord_sys
//       Access: Public
//  Description: Convert a single-word coordinate system name into
//               a PART/ARG of a ShaderMatSpec.
////////////////////////////////////////////////////////////////////
bool Shader::
cp_parse_coord_sys(ShaderArgInfo &p,
                   vector_string &pieces, int &next,
                   ShaderMatSpec &bind, bool fromflag) {

  string word1 = cp_parse_non_delimiter(pieces, next);
  if (pieces[next] == "of") next++;
  string word2 = cp_parse_non_delimiter(pieces, next);

  ShaderMatInput from_single;
  ShaderMatInput from_double;
  ShaderMatInput to_single;
  ShaderMatInput to_double;

  if (word1 == "") {
    cp_report_error(p, "Could not parse coordinate system name");
    return false;
  } else if (word1 == "world") {
    from_single = SMO_world_to_view;
    from_double = SMO_INVALID;
    to_single   = SMO_view_to_world;
    to_double   = SMO_INVALID;
  } else if (word1 == "model") {
    from_single = SMO_model_to_view;
    from_double = SMO_view_x_to_view;
    to_single   = SMO_view_to_model;
    to_double   = SMO_view_to_view_x;
  } else if (word1 == "clip") {
    from_single = SMO_clip_to_view;
    from_double = SMO_clip_x_to_view;
    to_single   = SMO_view_to_clip;
    to_double   = SMO_view_to_clip_x;
  } else if (word1 == "view") {
    from_single = SMO_identity;
    from_double = SMO_view_x_to_view;
    to_single   = SMO_identity;
    to_double   = SMO_view_to_view_x;
  } else if (word1 == "apiview") {
    from_single = SMO_apiview_to_view;
    from_double = SMO_apiview_x_to_view;
    to_single   = SMO_view_to_apiview;
    to_double   = SMO_view_to_apiview_x;
  } else if (word1 == "apiclip") {
    from_single = SMO_apiclip_to_view;
    from_double = SMO_apiclip_x_to_view;
    to_single   = SMO_view_to_apiclip;
    to_double   = SMO_view_to_apiclip_x;
  } else {
    from_single = SMO_view_x_to_view;
    from_double = SMO_view_x_to_view;
    to_single   = SMO_view_to_view_x;
    to_double   = SMO_view_to_view_x;
    word2 = word1;
  }

  if (fromflag) {
    if (word2 == "") {
      bind._part[0] = from_single;
      bind._arg[0] = NULL;
    } else {
      if (from_double == SMO_INVALID) {
        cp_report_error(p, "Could not parse coordinate system name");
        return false;
      }
      bind._part[0] = from_double;
      bind._arg[0] = InternalName::make(word2);
    }
  } else {
    if (word2 == "") {
      bind._part[1] = to_single;
      bind._arg[1] = NULL;
    } else {
      if (to_double == SMO_INVALID) {
        cp_report_error(p, "Could not parse coordinate system name");
        return false;
      }
      bind._part[1] = to_double;
      bind._arg[1] = InternalName::make(word2);
    }
  }
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::cp_dependency
//       Access: Public
//  Description: Given ShaderMatInput, returns an indication of what
//               part or parts of the state_and_transform the
//               ShaderMatInput depends upon.
////////////////////////////////////////////////////////////////////
int Shader::
cp_dependency(ShaderMatInput inp) {

  int dep = SSD_general;

  if (inp == SMO_INVALID) {
    return SSD_NONE;
  }
  if (inp == SMO_attr_material) {
    dep |= SSD_material;
  }
  if (inp == SMO_attr_color) {
    dep |= SSD_color;
  }
  if (inp == SMO_attr_colorscale) {
    dep |= SSD_colorscale;
  }
  if ((inp == SMO_model_to_view)||
      (inp == SMO_view_to_model)) {
    dep |= SSD_transform;
  }
  if ((inp == SMO_texpad_x)||
      (inp == SMO_texpix_x)||
      (inp == SMO_alight_x)||
      (inp == SMO_dlight_x)||
      (inp == SMO_plight_x)||
      (inp == SMO_slight_x)||
      (inp == SMO_satten_x)||
      (inp == SMO_mat_constant_x)||
      (inp == SMO_vec_constant_x)||
      (inp == SMO_clipplane_x)||
      (inp == SMO_view_x_to_view)||
      (inp == SMO_view_to_view_x)||
      (inp == SMO_apiview_x_to_view)||
      (inp == SMO_view_to_apiview_x)||
      (inp == SMO_clip_x_to_view)||
      (inp == SMO_view_to_clip_x)||
      (inp == SMO_apiclip_x_to_view)||
      (inp == SMO_view_to_apiclip_x)) {
    dep |= SSD_shaderinputs;
  }

  return dep;
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::cp_optimize_mat_spec
//       Access: Public
//  Description: Analyzes a ShaderMatSpec and decides what it should
//               use its cache for.  It can cache the results of any
//               one opcode, or, it can cache the entire result.  This
//               routine needs to be smart enough to know which
//               data items can be correctly cached, and which cannot.
////////////////////////////////////////////////////////////////////
void Shader::
cp_optimize_mat_spec(ShaderMatSpec &spec) {

  // If we're composing with identity, simplify.

  if (spec._func == SMF_first) {
    spec._part[1] = SMO_INVALID;
    spec._arg[1] = 0;
  }
  if (spec._func == SMF_compose) {
    if (spec._part[1] == SMO_identity) {
      spec._func = SMF_first;
    }
  }
  if (spec._func == SMF_compose) {
    if (spec._part[0] == SMO_identity) {
      spec._func = SMF_first;
      spec._part[0] = spec._part[1];
      spec._arg[0] = spec._arg[1];
    }
  }

  // Calculate state and transform dependencies.

  spec._dep[0] = cp_dependency(spec._part[0]);
  spec._dep[1] = cp_dependency(spec._part[1]);
}

#ifdef HAVE_CG
////////////////////////////////////////////////////////////////////
//     Function: Shader::cg_recurse_parameters
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void Shader::cg_recurse_parameters(CGparameter parameter, 
    const ShaderType& type, bool& success) {

  if (parameter == 0)
    return;

  do {
    int                arg_dim[]    = {1,0,0};
    ShaderArgDir       arg_dir      = cg_parameter_dir(parameter);
    ShaderArgType      arg_type     = cg_parameter_type(parameter);
    ShaderArgClass     arg_class    = cg_parameter_class(parameter);
    ShaderArgClass     arg_subclass = arg_class;

    CGenum vbl = cgGetParameterVariability(parameter);
  
    if ((vbl==CG_VARYING)||(vbl==CG_UNIFORM)){
      switch (cgGetParameterType(parameter)) {
        case CG_STRUCT:
          cg_recurse_parameters(
            cgGetFirstStructParameter(parameter),type,success); break;
        
        case CG_ARRAY:
          arg_type     = cg_parameter_type(cgGetFirstStructParameter(parameter));
          arg_subclass = cg_parameter_class(cgGetFirstStructParameter(parameter));

          arg_dim[0]  = cgGetArraySize(parameter,0); 

          //Uncomment this to parse the array[n] as n separeted elements
          //recurse_program_parameters(
          //  cgGetFirstStructParameter(parameter),type,success); break;

        default:{
          arg_dim[1] = cgGetParameterRows(parameter);
          arg_dim[2] = cgGetParameterColumns(parameter);
          
          ShaderArgId id;
          id._name = cgGetParameterName(parameter);
          id._type  = type;
          id._seqno = -1;
          success &= compile_parameter(id, arg_class, arg_subclass, arg_type, 
              arg_dir, (vbl == CG_VARYING), arg_dim, gobj_cat.get_safe_ptr()); break;
        }
      }
    }
  } while((parameter = cgGetNextParameter(parameter))!= 0);
}
#endif  // HAVE_CG

////////////////////////////////////////////////////////////////////
//     Function: Shader::compile_parameter
//       Access: Public
//  Description: Analyzes a parameter and decides how to
//               bind the parameter to some part of panda's
//               internal state.  Updates one of the bind
//               arrays to cause the binding to occur.
//
//               If there is an error, this routine will append
//               an error message onto the error messages.
////////////////////////////////////////////////////////////////////
bool Shader::
compile_parameter(const ShaderArgId        &arg_id,
                  const ShaderArgClass     &arg_class,
                  const ShaderArgClass     &arg_subclass,
                  const ShaderArgType      &arg_type,
                  const ShaderArgDir       &arg_direction,
                  bool                      arg_varying,
                  int                      *arg_dim,
                  NotifyCategory           *arg_cat)
{
  ShaderArgInfo p;
  p._id         = arg_id;
  p._class      = arg_class;
  p._subclass   = arg_subclass;
  p._type       = arg_type;
  p._direction  = arg_direction;
  p._varying    = arg_varying;
  p._cat        = arg_cat;

  if (p._id._name.size() == 0) return true;
  if (p._id._name[0] == '$') return true;

  // It could be inside a struct, strip off
  // everything before the last dot.
  size_t loc = p._id._name.find_last_of('.');
  
  string basename (p._id._name);
  string struct_name  ("");
  
  if (loc < string::npos) { 
    basename = p._id._name.substr(loc + 1);
    struct_name =  p._id._name.substr(0,loc+1);
  }
  
  // Split it at the underscores.
  vector_string pieces;
  tokenize(basename, pieces, "_");

  if (basename.size() >= 2 && basename.substr(0, 2) == "__") {
    return true;
  }

  // Implement vtx parameters - the varying kind.
  if (pieces[0] == "vtx") {
    if ((!cp_errchk_parameter_in(p)) ||
        (!cp_errchk_parameter_varying(p)) ||
        (!cp_errchk_parameter_float(p, 1, 4))) {
      return false;
    }
    ShaderVarSpec bind;
    bind._id = arg_id;
    if (pieces.size() == 2) {
      if (pieces[1]=="position") {
        bind._name = InternalName::get_vertex();
        bind._append_uv = -1;
        _var_spec.push_back(bind);
        return true;
      }
      if (pieces[1].substr(0,8)=="texcoord") {
        bind._name = InternalName::get_texcoord();
        bind._append_uv = atoi(pieces[1].c_str()+8);
        _var_spec.push_back(bind);
        return true;
      }
      if (pieces[1].substr(0,7)=="tangent") {
        bind._name = InternalName::get_tangent();
        bind._append_uv = atoi(pieces[1].c_str()+7);
        _var_spec.push_back(bind);
        return true;
      }
      if (pieces[1].substr(0,8)=="binormal") {
        bind._name = InternalName::get_binormal();
        bind._append_uv = atoi(pieces[1].c_str()+8);
        _var_spec.push_back(bind);
        return true;
      }
    }
    bind._name = InternalName::get_root();
    bind._append_uv = -1;
    for (int i=1; i<(int)(pieces.size()-0); i++)
      bind._name = bind._name->append(pieces[i]);
    _var_spec.push_back(bind);
    return true;
  }

  // Implement some macros. Macros work by altering the
  // contents of the 'pieces' array, and then falling through.

  if (pieces[0] == "mstrans") {
    pieces[0] = "trans";
    pieces.push_back("to");
    pieces.push_back("model");
  }
  if (pieces[0] == "wstrans") {
    pieces[0] = "trans";
    pieces.push_back("to");
    pieces.push_back("world");
  }
  if (pieces[0] == "vstrans") {
    pieces[0] = "trans";
    pieces.push_back("to");
    pieces.push_back("view");
  }
  if (pieces[0] == "cstrans") {
    pieces[0] = "trans";
    pieces.push_back("to");
    pieces.push_back("clip");
  }
  if (pieces[0] == "mspos") {
    pieces[0] = "row3";
    pieces.push_back("to");
    pieces.push_back("model");
  }
  if (pieces[0] == "wspos") {
    pieces[0] = "row3";
    pieces.push_back("to");
    pieces.push_back("world");
  }
  if (pieces[0] == "vspos") {
    pieces[0] = "row3";
    pieces.push_back("to");
    pieces.push_back("view");
  }
  if (pieces[0] == "cspos") {
    pieces[0] = "row3";
    pieces.push_back("to");
    pieces.push_back("clip");
  }

  // Implement the modelview macros.

  if ((pieces[0] == "mat")||(pieces[0] == "inv")||
      (pieces[0] == "tps")||(pieces[0] == "itp")) {
    if (!cp_errchk_parameter_words(p, 2)) {
      return false;
    }
    string trans = pieces[0];
    string matrix = pieces[1];
    pieces.clear();
    if (matrix == "modelview") {
      tokenize("trans_model_to_apiview", pieces, "_");
    } else if (matrix == "projection") {
      tokenize("trans_apiview_to_apiclip", pieces, "_");
    } else if (matrix == "modelproj") {
      tokenize("trans_model_to_apiclip", pieces, "_");
    } else {
      cp_report_error(p,"unrecognized matrix name");
      return false;
    }
    if (trans=="mat") {
      pieces[0] = "trans";
    } else if (trans=="inv") {
      string t = pieces[1];
      pieces[1] = pieces[3];
      pieces[3] = t;
    } else if (trans=="tps") {
      pieces[0] = "tpose";
    } else if (trans=="itp") {
      string t = pieces[1];
      pieces[1] = pieces[3];
      pieces[3] = t;
      pieces[0] = "tpose";
    }
  }

  // Implement the transform-matrix generator.

  if ((pieces[0]=="trans")||
      (pieces[0]=="tpose")||
      (pieces[0]=="row0")||
      (pieces[0]=="row1")||
      (pieces[0]=="row2")||
      (pieces[0]=="row3")||
      (pieces[0]=="col0")||
      (pieces[0]=="col1")||
      (pieces[0]=="col2")||
      (pieces[0]=="col3")) {

    if ((!cp_errchk_parameter_in(p)) ||
        (!cp_errchk_parameter_uniform(p)))
      return false;

    ShaderMatSpec bind;
    bind._id = arg_id;
    bind._func = SMF_compose;

    int next = 1;
    pieces.push_back("");

    // Decide whether this is a matrix or vector.
    if      (pieces[0]=="trans")   bind._piece = SMP_whole;
    else if (pieces[0]=="tpose")   bind._piece = SMP_transpose;
    else if (pieces[0]=="row0")    bind._piece = SMP_row0;
    else if (pieces[0]=="row1")    bind._piece = SMP_row1;
    else if (pieces[0]=="row2")    bind._piece = SMP_row2;
    else if (pieces[0]=="row3")    bind._piece = SMP_row3;
    else if (pieces[0]=="col0")    bind._piece = SMP_col0;
    else if (pieces[0]=="col1")    bind._piece = SMP_col1;
    else if (pieces[0]=="col2")    bind._piece = SMP_col2;
    else if (pieces[0]=="col3")    bind._piece = SMP_col3;
    if ((bind._piece == SMP_whole)||(bind._piece == SMP_transpose)) {
      if (!cp_errchk_parameter_float(p, 16, 16)) return false;
    } else {
      if (!cp_errchk_parameter_float(p, 4, 4)) return false;
    }

    if (!cp_parse_coord_sys(p, pieces, next, bind, true)) {
      return false;
    }
    if (!cp_parse_delimiter(p, pieces, next)) {
      return false;
    }
    if (!cp_parse_coord_sys(p, pieces, next, bind, false)) {
      return false;
    }
    if (!cp_parse_eol(p, pieces, next)) {
      return false;
    }
    cp_optimize_mat_spec(bind);
    _mat_spec.push_back(bind);
    return true;
  }

  // Special parameter: attr_material or attr_color

  if (pieces[0] == "attr") {
    if ((!cp_errchk_parameter_words(p,2)) ||
        (!cp_errchk_parameter_in(p)) ||
        (!cp_errchk_parameter_uniform(p))) {
      return false;
    }
    ShaderMatSpec bind;
    if (pieces[1] == "material") {
      if (!cp_errchk_parameter_float(p,16,16)) {
        return false;
      }
      bind._id = arg_id;
      bind._piece = SMP_transpose;
      bind._func = SMF_first;
      bind._part[0] = SMO_attr_material;
      bind._arg[0] = NULL;
      bind._part[1] = SMO_identity;
      bind._arg[1] = NULL;
    } else if (pieces[1] == "color") {
      if (!cp_errchk_parameter_float(p,3,4)) {
        return false;
      }
      bind._id = arg_id;
      bind._piece = SMP_row3;
      bind._func = SMF_first;
      bind._part[0] = SMO_attr_color;
      bind._arg[0] = NULL;
      bind._part[1] = SMO_identity;
      bind._arg[1] = NULL;
    } else if (pieces[1] == "colorscale") {
      if (!cp_errchk_parameter_float(p,3,4)) {
        return false;
      }
      bind._id = arg_id;
      bind._piece = SMP_row3;
      bind._func = SMF_first;
      bind._part[0] = SMO_attr_colorscale;
      bind._arg[0] = NULL;
      bind._part[1] = SMO_identity;
      bind._arg[1] = NULL;
    } else {
      cp_report_error(p,"Unknown attr parameter.");
      return false;
    }

    cp_optimize_mat_spec(bind);
    _mat_spec.push_back(bind);
    return true;
  }

  if (pieces[0] == "color") {
    if ((!cp_errchk_parameter_words(p,1)) ||
        (!cp_errchk_parameter_in(p)) ||
        (!cp_errchk_parameter_uniform(p))) {
      return false;
    }
    ShaderMatSpec bind;

    cp_optimize_mat_spec(bind);
    _mat_spec.push_back(bind);
    return true;
  }

  // Keywords to access light properties.

  if (pieces[0] == "alight") {
    if ((!cp_errchk_parameter_words(p,2))||
        (!cp_errchk_parameter_in(p)) ||
        (!cp_errchk_parameter_uniform(p))||
        (!cp_errchk_parameter_float(p,3,4))) {
      return false;
    }
    ShaderMatSpec bind;
    bind._id = arg_id;
    bind._piece = SMP_row3;
    bind._func = SMF_first;
    bind._part[0] = SMO_alight_x;
    bind._arg[0] = InternalName::make(pieces[1]);
    bind._part[1] = SMO_identity;
    bind._arg[1] = NULL;

    cp_optimize_mat_spec(bind);
    _mat_spec.push_back(bind);
    return true;
  }

  if (pieces[0] == "satten") {
    if ((!cp_errchk_parameter_words(p,2))||
        (!cp_errchk_parameter_in(p)) ||
        (!cp_errchk_parameter_uniform(p))||
        (!cp_errchk_parameter_float(p,4,4))) {
      return false;
    }
    ShaderMatSpec bind;
    bind._id = arg_id;
    bind._piece = SMP_row3;
    bind._func = SMF_first;
    bind._part[0] = SMO_satten_x;
    bind._arg[0] = InternalName::make(pieces[1]);
    bind._part[1] = SMO_identity;
    bind._arg[1] = NULL;

    cp_optimize_mat_spec(bind);
    _mat_spec.push_back(bind);
    return true;
  }

  if ((pieces[0]=="dlight")||(pieces[0]=="plight")||(pieces[0]=="slight")) {
    if ((!cp_errchk_parameter_in(p)) ||
        (!cp_errchk_parameter_uniform(p))||
        (!cp_errchk_parameter_float(p,16,16))) {
      return false;
    }
    ShaderMatSpec bind;
    bind._id = arg_id;
    bind._piece = SMP_transpose;
    int next = 1;
    pieces.push_back("");
    if (pieces[next] == "") {
      cp_report_error(p, "Light name expected");
      return false;
    }
    if (pieces[0] == "dlight") {
      bind._func = SMF_transform_dlight;
      bind._part[0] = SMO_dlight_x;
    } else if (pieces[0] == "plight") {
      bind._func = SMF_transform_plight;
      bind._part[0] = SMO_plight_x;
    } else if (pieces[0] == "slight") {
      bind._func = SMF_transform_slight;
      bind._part[0] = SMO_slight_x;
    }
    bind._arg[0] = InternalName::make(pieces[next]);
    next += 1;
    if (!cp_parse_delimiter(p, pieces, next)) {
      return false;
    }
    if (!cp_parse_coord_sys(p, pieces, next, bind, false)) {
      return false;
    }
    if (!cp_parse_eol(p, pieces, next)) {
      return false;
    }
    cp_optimize_mat_spec(bind);
    _mat_spec.push_back(bind);
    return true;
  }

  if (pieces[0] == "texmat") {
    if ((!cp_errchk_parameter_words(p,2))||
        (!cp_errchk_parameter_in(p)) ||
        (!cp_errchk_parameter_uniform(p))||
        (!cp_errchk_parameter_float(p,16,16))) {
      return false;
    }
    ShaderMatSpec bind;
    bind._id = arg_id;
    bind._piece = SMP_whole;
    bind._func = SMF_first;
    bind._part[0] = SMO_texmat_x;
    bind._arg[0] = InternalName::make(pieces[1]);
    bind._part[1] = SMO_identity;
    bind._arg[1] = NULL;

    cp_optimize_mat_spec(bind);
    _mat_spec.push_back(bind);
    return true;
  }

  if (pieces[0] == "plane") {
    if ((!cp_errchk_parameter_words(p,2))||
        (!cp_errchk_parameter_in(p)) ||
        (!cp_errchk_parameter_uniform(p))||
        (!cp_errchk_parameter_float(p,4,4))) {
      return false;
    }
    ShaderMatSpec bind;
    bind._id = arg_id;
    bind._piece = SMP_row3;
    bind._func = SMF_first;
    bind._part[0] = SMO_plane_x;
    bind._arg[0] = InternalName::make(pieces[1]);
    bind._part[1] = SMO_identity;
    bind._arg[1] = NULL;

    cp_optimize_mat_spec(bind);
    _mat_spec.push_back(bind);
    return true;
  }

  if (pieces[0] == "clipplane") {
    if ((!cp_errchk_parameter_words(p,2))||
        (!cp_errchk_parameter_in(p)) ||
        (!cp_errchk_parameter_uniform(p))||
        (!cp_errchk_parameter_float(p,4,4))) {
      return false;
    }
    ShaderMatSpec bind;
    bind._id = arg_id;
    bind._piece = SMP_row3;
    bind._func = SMF_first;
    bind._part[0] = SMO_clipplane_x;
    bind._arg[0] = InternalName::make(pieces[1]);
    bind._part[1] = SMO_identity;
    bind._arg[1] = NULL;

    cp_optimize_mat_spec(bind);
    _mat_spec.push_back(bind);
    return true;
  }

  // Keywords to access unusual parameters.

  if (pieces[0] == "sys") {
    if ((!cp_errchk_parameter_words(p,2)) ||
        (!cp_errchk_parameter_in(p)) ||
        (!cp_errchk_parameter_uniform(p))) {
      return false;
    }
    ShaderMatSpec bind;
    bind._id = arg_id;
    bind._piece = SMP_row3;
    bind._func = SMF_first;
    bind._part[1] = SMO_identity;
    bind._arg[1] = NULL;
    if (pieces[1] == "pixelsize") {
      if (!cp_errchk_parameter_float(p, 2, 2)) {
        return false;
      }
      bind._part[0] = SMO_pixel_size;
      bind._arg[0] = NULL;
    } else if (pieces[1] == "windowsize") {
      if (!cp_errchk_parameter_float(p, 2, 2)) {
        return false;
      }
      bind._part[0] = SMO_window_size;
      bind._arg[0] = NULL;
    } else {
      cp_report_error(p,"unknown system parameter");
      return false;
    }

    cp_optimize_mat_spec(bind);
    _mat_spec.push_back(bind);
    return true;
  }

  // Keywords to access textures.

  if (pieces[0] == "tex") {
    if ((!cp_errchk_parameter_in(p)) ||
        (!cp_errchk_parameter_uniform(p)) ||
        (!cp_errchk_parameter_sampler(p)))
      return false;
    if ((pieces.size() != 2)&&(pieces.size() != 3)) {
      cp_report_error(p, "Invalid parameter name");
      return false;
    }
    ShaderTexSpec bind;
    bind._id = arg_id;
    bind._name = 0;
    bind._stage = atoi(pieces[1].c_str());
    switch (p._type) {
    case SAT_sampler1d:      bind._desired_type = Texture::TT_1d_texture; break;
    case SAT_sampler2d:      bind._desired_type = Texture::TT_2d_texture; break;
    case SAT_sampler3d:      bind._desired_type = Texture::TT_3d_texture; break;
    case SAT_sampler2dArray: bind._desired_type = Texture::TT_2d_texture_array; break;
    case SAT_samplercube:    bind._desired_type = Texture::TT_cube_map; break;
    default:
      cp_report_error(p, "Invalid type for a tex-parameter");
      return false;
    }
    if (pieces.size()==3) {
      bind._suffix = InternalName::make(((string)"-") + pieces[2]);
    }
    _tex_spec.push_back(bind);
    return true;
  }

  // Keywords to fetch texture parameter data.

  if (pieces[0] == "texpad") {
    if ((!cp_errchk_parameter_words(p,2)) ||
        (!cp_errchk_parameter_in(p)) ||
        (!cp_errchk_parameter_uniform(p))||
        (!cp_errchk_parameter_float(p,3,4))) {
      return false;
    }
    ShaderMatSpec bind;
    bind._id = arg_id;
    bind._piece = SMP_row3;
    bind._func = SMF_first;
    bind._part[0] = SMO_texpad_x;
    bind._arg[0] = InternalName::make(pieces[1]);
    bind._part[1] = SMO_identity;
    bind._arg[1] = NULL;
    cp_optimize_mat_spec(bind);
    _mat_spec.push_back(bind);
    return true;
  }

  if (pieces[0] == "texpix") {
    if ((!cp_errchk_parameter_words(p,2)) ||
        (!cp_errchk_parameter_in(p)) ||
        (!cp_errchk_parameter_uniform(p))||
        (!cp_errchk_parameter_float(p,2,4))) {
      return false;
    }
    ShaderMatSpec bind;
    bind._id = arg_id;
    bind._piece = SMP_row3;
    bind._func = SMF_first;
    bind._part[0] = SMO_texpix_x;
    bind._arg[0] = InternalName::make(pieces[1]);
    bind._part[1] = SMO_identity;
    bind._arg[1] = NULL;
    cp_optimize_mat_spec(bind);
    _mat_spec.push_back(bind);
    return true;
  }

  if (pieces[0] == "l") {
    // IMPLEMENT THE ERROR CHECKING
    return true; // Cg handles this automatically.
  }

  if (pieces[0] == "o") {
    // IMPLEMENT THE ERROR CHECKING
    return true; // Cg handles this automatically.
  }

  // Fetch uniform parameters without prefix
  
  if ((!cp_errchk_parameter_in(p)) ||
      (!cp_errchk_parameter_uniform(p))) {
    return false;
  }
  bool k_prefix  = false;
  
  // solve backward compatibility issue
  if (pieces[0] == "k") {
    k_prefix = true;
    basename = basename.substr(2);
  }
  
  PT(InternalName) kinputname = InternalName::make(struct_name + basename);
  
  switch (p._class) {
  case SAC_vector:
  case SAC_matrix:
  case SAC_scalar:
  case SAC_array: {
    if (!cp_errchk_parameter_ptr(p))
      return false;
  
    ShaderPtrSpec bind;
    bind._id      = arg_id;
    bind._arg     = kinputname;
    bind._info    = p;
    bind._dep[0]  = SSD_general | SSD_shaderinputs;
    bind._dep[1]  = SSD_general | SSD_NONE;

    memcpy(bind._dim,arg_dim,sizeof(int)*3);

    // if dim[0] = -1,  glShaderContext will not check the param size
    if (k_prefix) bind._dim[0] = -1;
    _ptr_spec.push_back(bind);
    return true;
  }

  case SAC_sampler: {
    switch (p._type) {
    case SAT_sampler1d: {
      ShaderTexSpec bind;
      bind._id = arg_id;
      bind._name = kinputname;
      bind._desired_type = Texture::TT_1d_texture;
      _tex_spec.push_back(bind);
      return true;
    }
    case SAT_sampler2d: {
      ShaderTexSpec bind;
      bind._id = arg_id;
      bind._name = kinputname;
      bind._desired_type = Texture::TT_2d_texture;
      _tex_spec.push_back(bind);
      return true;
    }
    case SAT_sampler3d: {
      ShaderTexSpec bind;
      bind._id = arg_id;
      bind._name = kinputname;
      bind._desired_type = Texture::TT_3d_texture;
      _tex_spec.push_back(bind);
      return true;
    }
    case SAT_sampler2dArray: {
      ShaderTexSpec bind;
      bind._id = arg_id;
      bind._name = kinputname;
      bind._desired_type = Texture::TT_2d_texture_array;
      _tex_spec.push_back(bind);
      return true;
    }
    case SAT_samplercube: {
      ShaderTexSpec bind;
      bind._id = arg_id;
      bind._name = kinputname;
      bind._desired_type = Texture::TT_cube_map;
      _tex_spec.push_back(bind);
      return true;
    }
    default:
      cp_report_error(p, "invalid type for non-prefix parameter");
      return false;
    }
  }
  default:
    cp_report_error(p, "invalid class for non-prefix parameter");
    return false;
  }

  cp_report_error(p, "unrecognized parameter name");
  return false;
}


////////////////////////////////////////////////////////////////////
//     Function: Shader::clear_parameters
//       Access: Private
//  Description:
////////////////////////////////////////////////////////////////////
void Shader::
clear_parameters() {
  _mat_spec.clear();
  _var_spec.clear();
  _tex_spec.clear();
}

#ifdef HAVE_CG
////////////////////////////////////////////////////////////////////
//     Function: Shader::cg_parameter_type
//       Access: Private
//  Description:
////////////////////////////////////////////////////////////////////
Shader::ShaderArgType Shader::
cg_parameter_type(CGparameter p) {
  
  switch (cgGetParameterClass(p)) {
  case CG_PARAMETERCLASS_SCALAR: return SAT_scalar;
  case CG_PARAMETERCLASS_VECTOR:
    switch (cgGetParameterColumns(p)){
    case 1:  return SAT_vec1;
    case 2:  return SAT_vec2;
    case 3:  return SAT_vec3;
    case 4:  return SAT_vec4;
    default: return SAT_unknown;
    } 
  case CG_PARAMETERCLASS_MATRIX:
    switch (cgGetParameterRows(p)){
    case 1:
      switch (cgGetParameterColumns(p)){
      case 1:  return SAT_mat1x1;
      case 2:  return SAT_mat1x2;
      case 3:  return SAT_mat1x3;
      case 4:  return SAT_mat1x4;
      default: return SAT_unknown;
      }
    case 2:
      switch (cgGetParameterColumns(p)){
      case 1:  return SAT_mat2x1;
      case 2:  return SAT_mat2x2;
      case 3:  return SAT_mat2x3;
      case 4:  return SAT_mat2x4;
      default: return SAT_unknown;
      }
    case 3:
      switch (cgGetParameterColumns(p)){
      case 1:  return SAT_mat3x1;
      case 2:  return SAT_mat3x2;
      case 3:  return SAT_mat3x3;
      case 4:  return SAT_mat3x4;
      default: return SAT_unknown;
      }
    case 4:
      switch (cgGetParameterColumns(p)){
      case 1:  return SAT_mat4x1;
      case 2:  return SAT_mat4x2;
      case 3:  return SAT_mat4x3;
      case 4:  return SAT_mat4x4;
      default: return SAT_unknown;
      }
    default: return SAT_unknown;
    }
  case CG_PARAMETERCLASS_SAMPLER:
    switch (cgGetParameterType(p)){
    case CG_SAMPLER1D:      return Shader::SAT_sampler1d;
    case CG_SAMPLER2D:      return Shader::SAT_sampler2d;
    case CG_SAMPLER3D:      return Shader::SAT_sampler3d;
    case CG_SAMPLER2DARRAY: return Shader::SAT_sampler2dArray;
    case CG_SAMPLERCUBE:    return Shader::SAT_samplercube;
    default: return SAT_unknown;
    }
  case CG_PARAMETERCLASS_ARRAY: return SAT_unknown;
  default: return SAT_unknown;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::cg_parameter_class
//       Access: Private
//  Description:
////////////////////////////////////////////////////////////////////
Shader::ShaderArgClass Shader::cg_parameter_class(CGparameter p) {
  switch (cgGetParameterClass(p)) {
  case CG_PARAMETERCLASS_SCALAR:  return Shader::SAC_scalar;
  case CG_PARAMETERCLASS_VECTOR:  return Shader::SAC_vector;
  case CG_PARAMETERCLASS_MATRIX:  return Shader::SAC_matrix;
  case CG_PARAMETERCLASS_SAMPLER: return Shader::SAC_sampler;
  case CG_PARAMETERCLASS_ARRAY:   return Shader::SAC_array;
  default:                        return Shader::SAC_unknown;           
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::cg_parameter_dir
//       Access: Private
//  Description:
////////////////////////////////////////////////////////////////////
Shader::ShaderArgDir Shader::
cg_parameter_dir(CGparameter p) {
  switch (cgGetParameterDirection(p)) {
  case CG_IN:    return Shader::SAD_in;
  case CG_OUT:   return Shader::SAD_out;
  case CG_INOUT: return Shader::SAD_inout;
  default:       return Shader::SAD_unknown;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::cg_release_resources
//       Access: Private
//  Description: xyz
////////////////////////////////////////////////////////////////////
void Shader::
cg_release_resources() {
  if (_cg_vprogram != 0) {
    cgDestroyProgram(_cg_vprogram);
    _cg_vprogram = 0;
  }
  if (_cg_fprogram != 0) {
    cgDestroyProgram(_cg_fprogram);
    _cg_fprogram = 0;
  }
  if (_cg_gprogram != 0) {
    cgDestroyProgram(_cg_gprogram);
    _cg_gprogram = 0;
  }
  if (_cg_context != 0) {
    cgDestroyContext(_cg_context);
    _cg_context = 0;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::cg_compile_entry_point
//       Access: Private
//  Description: xyz
////////////////////////////////////////////////////////////////////
CGprogram Shader::
cg_compile_entry_point(const char *entry, const ShaderCaps &caps, ShaderType type) {
  CGprogram prog;
  CGerror err;
  const char *compiler_args[100];
  const string text = get_text(type);
  int nargs = 0;

  int active, ultimate;

  switch (type) {
  case ST_vertex:
    active   = caps._active_vprofile;
    ultimate = caps._ultimate_vprofile;
    break;

  case ST_fragment:
    active   = caps._active_fprofile;
    ultimate = caps._ultimate_fprofile;
    break;

  case ST_geometry:
    active   = caps._active_gprofile;
    ultimate = caps._ultimate_gprofile;
    break;

  case ST_none:
    active   = CG_PROFILE_UNKNOWN;
    ultimate = CG_PROFILE_UNKNOWN;
  };

  cgGetError();

  if (type == ST_fragment && caps._bug_list.count(SBUG_ati_draw_buffers)) {
    compiler_args[nargs++] = "-po";
    compiler_args[nargs++] = "ATI_draw_buffers";
  }
  compiler_args[nargs] = 0;

  if ((active != (int)CG_PROFILE_UNKNOWN) && (active != ultimate)) {
    prog = cgCreateProgram(_cg_context, CG_SOURCE, text.c_str(),
                           (CGprofile)active, entry, (const char **)compiler_args);
    err = cgGetError();
    if (err == CG_NO_ERROR) {
      return prog;
    }
    if (prog != 0) {
      cgDestroyProgram(prog);
    }
  }

  prog = cgCreateProgram(_cg_context, CG_SOURCE, text.c_str(),
                         (CGprofile)ultimate, entry, (const char **)NULL);
  err = cgGetError();
  if (err == CG_NO_ERROR) {
    return prog;
  }
  if (err == CG_COMPILER_ERROR) {
    string listing = cgGetLastListing(_cg_context);
    vector_string errlines;
    tokenize(listing, errlines, "\n");
    for (int i=0; i<(int)errlines.size(); i++) {
      string line = trim(errlines[i]);
      if (line != "") {
        gobj_cat.error() << get_filename(type) << ": " << errlines[i] << "\n";
      }
    }
  } else {
    gobj_cat.error() << get_filename(type) << ": " << cgGetErrorString(err) << "\n";
  }
  if (prog != 0) {
    cgDestroyProgram(prog);
  }

  return 0;
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::cg_compile_shader
//       Access: Private
//  Description: Compiles a Cg shader for a given set of capabilities.
//               If successful, the shader is stored in the instance
//               variables _cg_context, _cg_vprogram, _cg_fprogram.
////////////////////////////////////////////////////////////////////
bool Shader::
cg_compile_shader(const ShaderCaps &caps) {

  // If we already tried compiling for this set of caps, there's no point
  // trying again.  Just return the results of the previous compile.
  if (caps == _cg_last_caps) {
    if (_cg_context == 0) {
      return false;
    } else {
      return true;
    }
  }

  _cg_last_caps = caps;

  _cg_context = cgCreateContext();

  if (!_text->_separate) {
    gobj_cat.debug() << "Compiling Shader: \n" << _text << "\n";
  }

  if (_cg_context == 0) {
    gobj_cat.error() << "could not create a Cg context object.\n";
    return false;
  }

  if (!_text->_separate || !_text->_vertex.empty()) {
    _cg_vprogram = cg_compile_entry_point("vshader", caps, ST_vertex);
    if (_cg_vprogram == 0) {
      cg_release_resources();
      return false;
    }
  }

  if (!_text->_separate || !_text->_fragment.empty()) {
    _cg_fprogram = cg_compile_entry_point("fshader", caps, ST_fragment);
    if (_cg_fprogram == 0) {
      cg_release_resources();
      return false;
    }
  }

  if ((_text->_separate && !_text->_geometry.empty()) || (!_text->_separate && _text->_shared.find("gshader") != -1)) {
    _cg_gprogram = cg_compile_entry_point("gshader", caps, ST_geometry);
    if (_cg_gprogram == 0) {
      cg_release_resources();
      return false;
    }
  }

  if (_cg_vprogram == 0 && _cg_fprogram == 0 && _cg_gprogram == 0) {
    gobj_cat.error() << "Shader must at least have one program!\n";
    cg_release_resources();
    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::cg_analyze_entry_point
//       Access: Private
//  Description:
////////////////////////////////////////////////////////////////////
bool Shader::
cg_analyze_entry_point(CGprogram prog, ShaderType type) {
  CGparameter parameter;
  bool success = true;

  cg_recurse_parameters(cgGetFirstParameter(prog, CG_PROGRAM),type,success);
  return success;
}


////////////////////////////////////////////////////////////////////
//     Function: Shader::cg_analyze_shader
//       Access: Private
//  Description: This subroutine analyzes the parameters of a Cg
//               shader. The output is stored in instance variables:
//               _mat_spec, _var_spec, and _tex_spec.
//
//               In order to do this, it is necessary to compile the
//               shader.  It would be a waste of CPU time to compile
//               the shader, analyze the parameters, and then discard
//               the compiled shader.  This would force us to compile it
//               again later, when we need to build the ShaderContext.
//               Instead, we cache the compiled Cg program in instance
//               variables.  Later, a ShaderContext can pull the
//               compiled shader from these instance vars.
//
//               To compile a shader, you need to first choose a profile.
//               There are two contradictory objectives:
//
//               1. If you don't use the gsg's active profile,
//               then the cached compiled shader will not be useful to
//               the ShaderContext.
//
//               2. If you use too weak a profile, then the shader may
//               not compile.  So to guarantee success, you should use
//               the ultimate profile.
//
//               To resolve this conflict, we try the active profile
//               first, and if that doesn't work, we try the ultimate
//               profile.
//
////////////////////////////////////////////////////////////////////
bool Shader::
cg_analyze_shader(const ShaderCaps &caps) {

  if (!cg_compile_shader(caps)) {
    return false;
  }

  if (_cg_fprogram != 0) {
     if (!cg_analyze_entry_point(_cg_fprogram, ST_fragment)) {
      cg_release_resources();
      clear_parameters();
      return false;
    }
  }

  if (_var_spec.size() != 0) {
    gobj_cat.error() << "Cannot use vtx parameters in an fshader\n";
    cg_release_resources();
    clear_parameters();
    return false;
  }

  if (_cg_vprogram != 0) {
    if (!cg_analyze_entry_point(_cg_vprogram, ST_vertex)) {
      cg_release_resources();
      clear_parameters();
      return false;
    }
  }

  if (_cg_gprogram != 0) {
    if (!cg_analyze_entry_point(_cg_gprogram, ST_geometry)) {
      cg_release_resources();
      clear_parameters();
      return false;
    }
  }

  // Assign sequence numbers to all parameters.
  int seqno = 0;
  for (int i=0; i<(int)_mat_spec.size(); i++) {
    _mat_spec[i]._id._seqno = seqno++;
  }
  for (int i=0; i<(int)_tex_spec.size(); i++) {
    _tex_spec[i]._id._seqno = seqno++;
  }
  for (int i=0; i<(int)_var_spec.size(); i++) {
    _var_spec[i]._id._seqno = seqno++;
  }

  for (int i=0; i<(int)_ptr_spec.size(); i++) {
    _ptr_spec[i]._id._seqno = seqno++; 
    _ptr_spec[i]._info._id = _ptr_spec[i]._id;
  }

  // DEBUG: output the generated program
  if (gobj_cat.is_debug()) {
    const char *vertex_program;
    const char *pixel_program;
    const char *geometry_program;

    if (_cg_vprogram != 0) {
      vertex_program   = cgGetProgramString (_cg_vprogram, CG_COMPILED_PROGRAM);
      gobj_cat.debug() << vertex_program << "\n";
    }
    if (_cg_fprogram != 0) {
      pixel_program    = cgGetProgramString (_cg_fprogram, CG_COMPILED_PROGRAM);
      gobj_cat.debug() << pixel_program << "\n";
    }
    if (_cg_gprogram != 0) {
      geometry_program = cgGetProgramString (_cg_gprogram, CG_COMPILED_PROGRAM);
      gobj_cat.debug() << geometry_program << "\n";
    }
  }

  //  // The following code is present to work around a bug in the Cg compiler.
  //  // It does not generate correct code for shadow map lookups when using arbfp1.
  //  // This is a particularly onerous limitation, given that arbfp1 is the only
  //  // Cg target that works on radeons.  I suspect this is an intentional
  //  // omission on nvidia's part.  The following code fetches the output listing,
  //  // detects the error, repairs the code, and resumbits the repaired code to Cg.
  //  if ((_cg_fprofile == CG_PROFILE_ARBFP1) && (gsghint->_supports_shadow_filter)) {
  //    bool shadowunit[32];
  //    bool anyshadow = false;
  //    memset(shadowunit, 0, sizeof(shadowunit));
  //    vector_string lines;
  //    tokenize(cgGetProgramString(_cg_program[SHADER_type_frag],
  //                                CG_COMPILED_PROGRAM), lines, "\n");
  //    // figure out which texture units contain shadow maps.
  //    for (int lineno=0; lineno<(int)lines.size(); lineno++) {
  //      if (lines[lineno].compare(0,21,"#var sampler2DSHADOW ")) {
  //        continue;
  //      }
  //      vector_string fields;
  //      tokenize(lines[lineno], fields, ":");
  //      if (fields.size()!=5) {
  //        continue;
  //      }
  //      vector_string words;
  //      tokenize(trim(fields[2]), words, " ");
  //      if (words.size()!=2) {
  //        continue;
  //      }
  //      int unit = atoi(words[1].c_str());
  //      if ((unit < 0)||(unit >= 32)) {
  //        continue;
  //      }
  //      anyshadow = true;
  //      shadowunit[unit] = true;
  //    }
  //    // modify all TEX statements that use the relevant texture units.
  //    if (anyshadow) {
  //      for (int lineno=0; lineno<(int)lines.size(); lineno++) {
  //        if (lines[lineno].compare(0,4,"TEX ")) {
  //          continue;
  //        }
  //        vector_string fields;
  //        tokenize(lines[lineno], fields, ",");
  //        if ((fields.size()!=4)||(trim(fields[3]) != "2D;")) {
  //          continue;
  //        }
  //        vector_string texunitf;
  //        tokenize(trim(fields[2]), texunitf, "[]");
  //        if ((texunitf.size()!=3)||(texunitf[0] != "texture")||(texunitf[2]!="")) {
  //          continue;
  //        }
  //        int unit = atoi(texunitf[1].c_str());
  //        if ((unit < 0) || (unit >= 32) || (shadowunit[unit]==false)) {
  //          continue;
  //        }
  //        lines[lineno] = fields[0]+","+fields[1]+","+fields[2]+", SHADOW2D;";
  //      }
  //      string result = "!!ARBfp1.0\nOPTION ARB_fragment_program_shadow;\n";
  //      for (int lineno=1; lineno<(int)lines.size(); lineno++) {
  //        result += (lines[lineno] + "\n");
  //      }
  //      _cg_program[2] = _cg_program[SHADER_type_frag];
  //      _cg_program[SHADER_type_frag] =
  //        cgCreateProgram(_cg_context, CG_OBJECT, result.c_str(),
  //                        _cg_profile[SHADER_type_frag], "fshader", (const char**)NULL);
  //      cg_report_errors(s->get_name(), _cg_context);
  //      if (_cg_program[SHADER_type_frag]==0) {
  //        release_resources();
  //        return false;
  //      }
  //    }
  //  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::cg_program_from_shadertype
//       Access: Private
//  Description: Returns the CGprogram of the given shadertype
//               that belongs to this shader.
////////////////////////////////////////////////////////////////////
CGprogram Shader::
cg_program_from_shadertype(ShaderType type) {
  CGprogram prog;

  switch (type) {
    case ST_vertex:
      prog = _cg_vprogram;
      break;

    case ST_fragment:
      prog = _cg_fprogram;
      break;

    case ST_geometry:
      prog = _cg_gprogram;
      break;

    default:
      prog = 0;
  };

  return prog;
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::cg_compile_for
//       Access: Public
//  Description: This routine is used by the ShaderContext constructor
//               to compile the shader.  The CGprogram
//               objects are turned over to the ShaderContext, we no
//               longer own them.
////////////////////////////////////////////////////////////////////
bool Shader::
cg_compile_for(const ShaderCaps &caps,
               CGcontext &ctx,
               CGprogram &vprogram,
               CGprogram &fprogram,
               CGprogram &gprogram,
               pvector<CGparameter> &map) {

  // Initialize the return values to empty.

  ctx = 0;
  vprogram = 0;
  fprogram = 0;
  gprogram = 0;

  map.clear();

  // Make sure the shader is compiled for the target caps.
  // Most of the time, it will already be - this is usually a no-op.

  _default_caps = caps;
  if (!cg_compile_shader(caps)) {
    return false;
  }

  // If the compile routine used the ultimate profile instead of the
  // active one, it means the active one isn't powerful enough to
  // compile the shader.

  if (_filename->_separate) {
    if (_cg_vprogram != 0 && _cg_vprofile == CG_PROFILE_UNKNOWN && cgGetProgramProfile(_cg_vprogram) != caps._active_vprofile) {
      gobj_cat.error() << "Cg vprogram too complex for driver: "
        << get_filename(ST_vertex) << ". Try choosing a different profile.\n";
      return false;
    }
    if (_cg_fprogram != 0 && _cg_fprofile == CG_PROFILE_UNKNOWN && cgGetProgramProfile(_cg_fprogram) != caps._active_fprofile) {
      gobj_cat.error() << "Cg fprogram too complex for driver: "
        << get_filename(ST_fragment) << ". Try choosing a different profile.\n";
      return false;
    }
    if (_cg_gprogram != 0 && _cg_gprofile == CG_PROFILE_UNKNOWN && cgGetProgramProfile(_cg_gprogram) != caps._active_gprofile) {
      gobj_cat.error() << "Cg gprogram too complex for driver: "
        << get_filename(ST_geometry) << ". Try choosing a different profile.\n";
      return false;
    }
  } else {
    if ((_cg_vprogram != 0 && _cg_vprofile == CG_PROFILE_UNKNOWN && cgGetProgramProfile(_cg_vprogram) != caps._active_vprofile) ||
        (_cg_fprogram != 0 && _cg_fprofile == CG_PROFILE_UNKNOWN && cgGetProgramProfile(_cg_fprogram) != caps._active_fprofile) ||
        (_cg_gprogram != 0 && _cg_gprofile == CG_PROFILE_UNKNOWN && cgGetProgramProfile(_cg_gprogram) != caps._active_gprofile)) {
      gobj_cat.error() << "Cg program too complex for driver: "
        << get_filename() << ". Try choosing a different profile.\n";
      return false;
    }
  }

  // Build a parameter map.

  int n_mat = (int)_mat_spec.size();
  int n_tex = (int)_tex_spec.size();
  int n_var = (int)_var_spec.size();
  int n_ptr = (int)_ptr_spec.size();

  map.resize(n_mat + n_tex + n_var + n_ptr);

  for (int i=0; i<n_mat; i++) {
    const ShaderArgId &id = _mat_spec[i]._id;

    CGprogram prog = cg_program_from_shadertype(id._type);  // CG2 CHANGE
    map[id._seqno] = cgGetNamedParameter(prog, id._name.c_str());
  }
  for (int i=0; i<n_tex; i++) {
    const ShaderArgId &id = _tex_spec[i]._id;
    CGprogram prog = cg_program_from_shadertype(id._type);  // CG2 CHANGE
    map[id._seqno] = cgGetNamedParameter(prog, id._name.c_str());
    if (cgGetParameterBaseResource(map[id._seqno]) == CG_UNDEFINED) {
      map[id._seqno] = 0;
    }
  }
  for (int i=0; i<n_var; i++) {
    const ShaderArgId &id = _var_spec[i]._id;

    CGprogram prog = cg_program_from_shadertype(id._type);      // CG2 CHANGE
    map[id._seqno] = cgGetNamedParameter(prog, id._name.c_str());
    if (cgGetParameterBaseResource(map[id._seqno]) == CG_UNDEFINED) {
      map[id._seqno] = 0;
    }
  }
  for (int i=0; i<n_ptr; i++) { 
    const ShaderArgId &id = _ptr_spec[i]._id;
    
    CGprogram prog = cg_program_from_shadertype(id._type);  // CG2 CHANGE
    map[id._seqno] = cgGetNamedParameter(prog, id._name.c_str());
  }

  // Transfer ownership of the compiled shader.
  ctx = _cg_context;
  vprogram = _cg_vprogram;
  fprogram = _cg_fprogram;
  gprogram = _cg_gprogram;

  _cg_context = 0;
  _cg_vprogram = 0;
  _cg_fprogram = 0;
  _cg_gprogram = 0;

  _cg_last_caps.clear();

  return true;
}
#endif  // HAVE_CG

////////////////////////////////////////////////////////////////////
//     Function: Shader::Constructor
//       Access: Private
//  Description: Construct a Shader.
////////////////////////////////////////////////////////////////////
Shader::
Shader(CPT(ShaderFile) filename, CPT(ShaderFile) text, const ShaderLanguage &lang) :
  _filename(filename),
  _text(text),
  _error_flag(true),
  _parse(0),
  _loaded(false),
  _language(lang)
{
  _error_flag = false;
  
#ifdef HAVE_CG
  _cg_context = 0;
  _cg_vprogram = 0;
  _cg_fprogram = 0;
  _cg_gprogram = 0;
  _cg_vprofile = CG_PROFILE_UNKNOWN;
  _cg_fprofile = CG_PROFILE_UNKNOWN;
  _cg_gprofile = CG_PROFILE_UNKNOWN;
  if (_default_caps._ultimate_vprofile == 0 || _default_caps._ultimate_vprofile == CG_PROFILE_UNKNOWN) {
    _default_caps._active_vprofile = CG_PROFILE_UNKNOWN;
    _default_caps._active_fprofile = CG_PROFILE_UNKNOWN;
    _default_caps._ultimate_vprofile = cgGetProfile("glslv");
    _default_caps._ultimate_fprofile = cgGetProfile("glslf");
    _default_caps._ultimate_gprofile = cgGetProfile("gp4gp");
  }
#endif
  
  // Determine which language the shader is written in.
  if (_language == SL_none) {
    string header;
    parse_init();
    parse_line(header, true, true);
    if (header == "//Cg") {
      _language = SL_Cg;
    } else if (header == "//GLSL") {
      _language = SL_GLSL;
    }
  }
  
  if (_language == SL_Cg) {
#ifdef HAVE_CG
    if (!_text->_separate) {
      cg_get_profile_from_header(_default_caps);
    }

    if (!cg_analyze_shader(_default_caps)) {
      _error_flag = true;
    }
#else
    gobj_cat.error()
      << "Tried to load Cg shader, but no Cg support is enabled.\n";
#endif
  } else if (_language == SL_GLSL) {
    // All of the important stuff is done in glShaderContext,
    // to avoid gobj getting a dependency on OpenGL.
    if (!_text->_separate) {
      gobj_cat.error()
        << "GLSL shaders must have separate shader bodies!\n";
      _error_flag = true;
    }
  } else {
    gobj_cat.error()
      << "Shader is not in a supported shader-language.\n";
    _error_flag = true;
  }
  if (_error_flag) {
    gobj_cat.error()
      << "Shader encountered an error.\n";
  }
}

#ifdef HAVE_CG
////////////////////////////////////////////////////////////////////
//  Function: Shader::cg_get_profile_from_header
//  Access: Private
//  Description: Determines the appropriate active shader profile settings
//               based on any profile directives stored within the shader header
////////////////////////////////////////////////////////////////////
void Shader::
cg_get_profile_from_header(ShaderCaps& caps) {
  // Note this forces profile based on what is specified in the shader
  // header string.  Should probably be relying on card caps eventually.

  string buf;
  parse_init();

  // Assume that if parse doesn't extend after a parse line then
  // we've reached the end of _text
  int lastParse;

  do {
    lastParse = _parse;
    parse_line(buf, true, true);
    int profilePos = buf.find("//Cg profile");
    if (profilePos >= 0) {
      // Scan the line for known cg2 vertex program profiles
      if ((int)buf.find("gp4vp") >= 0)
        caps._active_vprofile = cgGetProfile("gp4vp");

      if ((int)buf.find("glslv") >= 0)
        caps._active_vprofile = cgGetProfile("glslv");

      if ((int)buf.find("arbvp1") >= 0)
        caps._active_vprofile = cgGetProfile("arbvp1");

      if ((int)buf.find("vp40") >= 0)
        caps._active_vprofile = cgGetProfile("vp40");

      if ((int)buf.find("vp30") >= 0)
        caps._active_vprofile = cgGetProfile("vp30");

      if ((int)buf.find("vp20") >= 0)
        caps._active_vprofile = cgGetProfile("vp20");

      if ((int)buf.find("vs_1_1") >= 0)
        caps._active_vprofile = cgGetProfile("vs_1_1");

      if ((int)buf.find("vs_2_0") >= 0)
        caps._active_vprofile = cgGetProfile("vs_2_0");

      if ((int)buf.find("vs_2_x") >= 0)
        caps._active_vprofile = cgGetProfile("vs_2_x");

      if ((int)buf.find("vs_3_0") >= 0)
        caps._active_vprofile = cgGetProfile("vs_3_0");

      if ((int)buf.find("vs_4_0") >= 0)
        caps._active_vprofile = cgGetProfile("vs_4_0");

      // Scan the line for known cg2 fragment program profiles
      if ((int)buf.find("gp4fp") >= 0)
        caps._active_fprofile = cgGetProfile("gp4fp");

      if ((int)buf.find("glslf") >= 0)
        caps._active_fprofile = cgGetProfile("glslf");

      if ((int)buf.find("arbfp1") >= 0)
        caps._active_fprofile = cgGetProfile("arbfp1");

      if ((int)buf.find("fp40") >= 0)
        caps._active_fprofile = cgGetProfile("fp40");

      if ((int)buf.find("fp30") >= 0)
        caps._active_fprofile = cgGetProfile("fp30");

      if ((int)buf.find("fp20") >= 0)
        caps._active_fprofile = cgGetProfile("fp20");

      if ((int)buf.find("ps_1_1") >= 0)
        caps._active_fprofile = cgGetProfile("ps_1_1");

      if ((int)buf.find("ps_1_2") >= 0)
        caps._active_fprofile = cgGetProfile("ps_1_2");

      if ((int)buf.find("ps_1_3") >= 0)
        caps._active_fprofile = cgGetProfile("ps_1_3");

      if ((int)buf.find("ps_2_0") >= 0)
        caps._active_fprofile = cgGetProfile("ps_2_0");

      if ((int)buf.find("ps_2_x") >= 0)
        caps._active_fprofile = cgGetProfile("ps_2_x");

      if ((int)buf.find("ps_3_0") >= 0)
        caps._active_fprofile = cgGetProfile("ps_3_0");

      if ((int)buf.find("ps_4_0") >= 0)
        caps._active_fprofile = cgGetProfile("ps_4_0");

      // Scan the line for known cg2 geometry program profiles
      if ((int)buf.find("gp4gp") >= 0)
        caps._active_gprofile = cgGetProfile("gp4gp");

      if ((int)buf.find("glslg") >= 0)
        caps._active_gprofile = cgGetProfile("glslg");

      if ((int)buf.find("gs_4_0") >= 0)
        caps._active_gprofile = cgGetProfile("gs_4_0");
    }
  } while(_parse > lastParse);

}
#endif

////////////////////////////////////////////////////////////////////
//     Function: Shader::Destructor
//       Access: Public
//  Description: Delete the compiled code, if it exists.
////////////////////////////////////////////////////////////////////
Shader::
~Shader() {
  release_all();
  if (_loaded) {
    _load_table.erase(_filename);
  } else {
    _make_table.erase(_text);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::load
//       Access: Published, Static
//  Description: Loads the shader with the given filename.
////////////////////////////////////////////////////////////////////
PT(Shader) Shader::
load(const Filename &file, const ShaderLanguage &lang) {
  PT(ShaderFile) sfile =  new ShaderFile(file);
  ShaderTable::const_iterator i = _load_table.find(sfile);
  if (i != _load_table.end() && (lang == SL_none || lang == i->second->_language)) {
    return i->second;
  }
  PT(ShaderFile) sbody = new ShaderFile("");

  VirtualFileSystem *vfs = VirtualFileSystem::get_global_ptr();
  if (!vfs->read_file(file, sbody->_shared, true)) {
    gobj_cat.error() << "Could not read shader file: " << file << "\n";
    return NULL;
  }
  PT(Shader) result = new Shader(sfile, sbody, lang);
  result->_loaded = true;
  _load_table[sfile] = result;
  return result;
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::load
//       Access: Published, Static
//  Description: This variant of Shader::load loads all shader
//               programs separately.
////////////////////////////////////////////////////////////////////
PT(Shader) Shader::
load(const ShaderLanguage &lang, const Filename &vertex, const Filename &fragment, const Filename &geometry) {
  PT(ShaderFile) sfile = new ShaderFile(vertex, fragment, geometry);
  ShaderTable::const_iterator i = _load_table.find(sfile);
  if (i != _load_table.end() && (lang == SL_none || lang == i->second->_language)) {
    return i->second;
  }
  PT(ShaderFile) sbody = new ShaderFile("", "", "");
  VirtualFileSystem *vfs = VirtualFileSystem::get_global_ptr();
  if (!vertex.empty() && !vfs->read_file(vertex, sbody->_vertex, true)) {
    gobj_cat.error() << "Could not read vertex shader file: " << vertex << "\n";
    return NULL;
  }
  if (!fragment.empty() && !vfs->read_file(fragment, sbody->_fragment, true)) {
    gobj_cat.error() << "Could not read fragment shader file: " << vertex << "\n";
    return NULL;
  }
  if (!geometry.empty() && !vfs->read_file(geometry, sbody->_geometry, true)) {
    gobj_cat.error() << "Could not read geometry shader file: " << vertex << "\n";
    return NULL;
  }
  PT(Shader) result = new Shader(sfile, sbody, lang);
  result->_loaded = true;
  _load_table[sfile] = result;
  return result;
}

//////////////////////////////////////////////////////////////////////
//     Function: Shader::make
//       Access: Published, Static
//  Description: Loads the shader, using the string as shader body.
//////////////////////////////////////////////////////////////////////
PT(Shader) Shader::
make(const string &body, const ShaderLanguage &lang) {
  PT(ShaderFile) sbody = new ShaderFile(body);
  ShaderTable::const_iterator i = _make_table.find(sbody);
  if (i != _make_table.end() && (lang == SL_none || lang == i->second->_language)) {
    return i->second;
  }
  PT(ShaderFile) sfile = new ShaderFile("created-shader");
  PT(Shader) result = new Shader(sfile, sbody, lang);
  _make_table[sbody] = result;
  if (dump_generated_shaders) {
    ostringstream fns;
    int index = _shaders_generated ++;
    fns << "genshader" << index;
    string fn = fns.str();
    gobj_cat.warning() << "Dumping shader: " << fn << "\n";
    pofstream s;
    s.open(fn.c_str());
    s << body;
    s.close();
  }
  return result;
}

//////////////////////////////////////////////////////////////////////
//     Function: Shader::make
//       Access: Published, Static
//  Description: Loads the shader, using the strings as shader bodies.
//////////////////////////////////////////////////////////////////////
PT(Shader) Shader::
make(const ShaderLanguage &lang, const string &vertex, const string &fragment, const string &geometry) {
  PT(ShaderFile) sbody = new ShaderFile(vertex, fragment, geometry);
  ShaderTable::const_iterator i = _make_table.find(sbody);
  if (i != _make_table.end() && (lang == SL_none || lang == i->second->_language)) {
    return i->second;
  }
  PT(ShaderFile) sfile = new ShaderFile("created-shader");
  PT(Shader) result = new Shader(sfile, sbody, lang);
  _make_table[sbody] = result;
  return result;
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::parse_init
//       Access: Public
//  Description: Set a 'parse pointer' to the beginning of the shader.
////////////////////////////////////////////////////////////////////
void Shader::
parse_init() {
  _parse = 0;
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::parse_line
//       Access: Public
//  Description: Parse a line of text. If 'lt' is true, trim blanks
//               from the left end of the line. If 'rt' is true, trim
//               blanks from the right end (the newline is always
//               trimmed).
////////////////////////////////////////////////////////////////////
void Shader::
parse_line(string &result, bool lt, bool rt) {
  nassertv(!_text->_separate);
  int len = _text->_shared.size();
  int head = _parse;
  int tail = head;
  while ((tail < len) && (_text->_shared[tail] != '\n')) {
    tail++;
  }
  if (tail < len) {
    _parse = tail+1;
  } else {
    _parse = tail;
  }
  if (lt) {
    while ((head < tail)&&(isspace(_text->_shared[head]))) head++;
    while ((tail > head)&&(isspace(_text->_shared[tail-1]))) tail--;
  }
  result = _text->_shared.substr(head, tail-head);
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::parse_upto
//       Access: Public
//  Description: Parse lines until you read a line that matches the
//               specified pattern.  Returns all the preceding lines,
//               and if the include flag is set, returns the final
//               line as well.
////////////////////////////////////////////////////////////////////
void Shader::
parse_upto(string &result, string pattern, bool include) {
  nassertv(!_text->_separate);
  GlobPattern endpat(pattern);
  int start = _parse;
  int last = _parse;
  while (_parse < (int)(_text->_shared.size())) {
    string t;
    parse_line(t, true, true);
    if (endpat.matches(t)) break;
    last = _parse;
  }
  if (include) {
    result = _text->_shared.substr(start, _parse - start);
  } else {
    result = _text->_shared.substr(start, last - start);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::parse_rest
//       Access: Public
//  Description: Returns the rest of the text from the current
//               parse location.
////////////////////////////////////////////////////////////////////
void Shader::
parse_rest(string &result) {
  nassertv(!_text->_separate);
  result = _text->_shared.substr(_parse, _text->_shared.size() - _parse);
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::parse_eof
//       Access: Public
//  Description: Returns true if the parse pointer is at the end of
//               the shader.
////////////////////////////////////////////////////////////////////
bool Shader::
parse_eof() {
  return (int)_text->_shared.size() == _parse;
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::prepare
//       Access: Published
//  Description: Indicates that the shader should be enqueued to be
//               prepared in the indicated prepared_objects at the
//               beginning of the next frame.  This will ensure the
//               texture is already loaded into texture memory if it
//               is expected to be rendered soon.
//
//               Use this function instead of prepare_now() to preload
//               textures from a user interface standpoint.
////////////////////////////////////////////////////////////////////
void Shader::
prepare(PreparedGraphicsObjects *prepared_objects) {
  prepared_objects->enqueue_shader(this);
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::is_prepared
//       Access: Published
//  Description: Returns true if the shader has already been prepared
//               or enqueued for preparation on the indicated GSG,
//               false otherwise.
////////////////////////////////////////////////////////////////////
bool Shader::
is_prepared(PreparedGraphicsObjects *prepared_objects) const {
  Contexts::const_iterator ci;
  ci = _contexts.find(prepared_objects);
  if (ci != _contexts.end()) {
    return true;
  }
  return prepared_objects->is_shader_queued(this);
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::release
//       Access: Published
//  Description: Frees the texture context only on the indicated object,
//               if it exists there.  Returns true if it was released,
//               false if it had not been prepared.
////////////////////////////////////////////////////////////////////
bool Shader::
release(PreparedGraphicsObjects *prepared_objects) {
  Contexts::iterator ci;
  ci = _contexts.find(prepared_objects);
  if (ci != _contexts.end()) {
    ShaderContext *sc = (*ci).second;
    if (sc != (ShaderContext *)NULL) {
      prepared_objects->release_shader(sc);
    } else {
      _contexts.erase(ci);
    }
    return true;
  }

  // Maybe it wasn't prepared yet, but it's about to be.
  return prepared_objects->dequeue_shader(this);
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::prepare_now
//       Access: Published
//  Description: Creates a context for the shader on the particular
//               GSG, if it does not already exist.  Returns the new
//               (or old) ShaderContext.  This assumes that the
//               GraphicsStateGuardian is the currently active
//               rendering context and that it is ready to accept new
//               textures.  If this is not necessarily the case, you
//               should use prepare() instead.
//
//               Normally, this is not called directly except by the
//               GraphicsStateGuardian; a shader does not need to be
//               explicitly prepared by the user before it may be
//               rendered.
////////////////////////////////////////////////////////////////////
ShaderContext *Shader::
prepare_now(PreparedGraphicsObjects *prepared_objects,
            GraphicsStateGuardianBase *gsg) {
  Contexts::const_iterator ci;
  ci = _contexts.find(prepared_objects);
  if (ci != _contexts.end()) {
    return (*ci).second;
  }

  ShaderContext *tc = prepared_objects->prepare_shader_now(this, gsg);
  _contexts[prepared_objects] = tc;

  return tc;
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::clear_prepared
//       Access: Private
//  Description: Removes the indicated PreparedGraphicsObjects table
//               from the Shader's table, without actually releasing
//               the texture.  This is intended to be called only from
//               PreparedGraphicsObjects::release_texture(); it should
//               never be called by user code.
////////////////////////////////////////////////////////////////////
void Shader::
clear_prepared(PreparedGraphicsObjects *prepared_objects) {
  Contexts::iterator ci;
  ci = _contexts.find(prepared_objects);
  if (ci != _contexts.end()) {
    _contexts.erase(ci);
  } else {
    // If this assertion fails, clear_prepared() was given a
    // prepared_objects which the texture didn't know about.
    nassertv(false);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::release_all
//       Access: Published
//  Description: Frees the context allocated on all objects for which
//               the texture has been declared.  Returns the number of
//               contexts which have been freed.
////////////////////////////////////////////////////////////////////
int Shader::
release_all() {
  // We have to traverse a copy of the _contexts list, because the
  // PreparedGraphicsObjects object will call clear_prepared() in response
  // to each release_texture(), and we don't want to be modifying the
  // _contexts list while we're traversing it.
  Contexts temp = _contexts;
  int num_freed = (int)_contexts.size();

  Contexts::const_iterator ci;
  for (ci = temp.begin(); ci != temp.end(); ++ci) {
    PreparedGraphicsObjects *prepared_objects = (*ci).first;
    ShaderContext *sc = (*ci).second;
    if (sc != (ShaderContext *)NULL) {
      prepared_objects->release_shader(sc);
    }
  }

  // There might still be some outstanding contexts in the map, if
  // there were any NULL pointers there.  Eliminate them.
  _contexts.clear();

  return num_freed;
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::ShaderCapabilities::clear()
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
void Shader::ShaderCaps::
clear() {
  _supports_glsl = false;
  
#ifdef HAVE_CG
  _active_vprofile = CG_PROFILE_UNKNOWN;
  _active_fprofile = CG_PROFILE_UNKNOWN;
  _active_gprofile = CG_PROFILE_UNKNOWN;
  _ultimate_vprofile = CG_PROFILE_UNKNOWN;
  _ultimate_fprofile = CG_PROFILE_UNKNOWN;
  _ultimate_gprofile = CG_PROFILE_UNKNOWN;
#endif
}

////////////////////////////////////////////////////////////////////
//     Function: Shader::register_with_read_factory
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
void Shader::
register_with_read_factory() {
  // IMPLEMENT ME
}

