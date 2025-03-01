// Filename: dxGraphicsStateGuardian8.I
// Created by:  mike (02Feb99)
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
//     Function: DXGraphicsStateGuardian8::Colorf_to_D3DCOLOR
//       Access: Public, Static
//  Description: Converts Panda's floating-point Colorf structure to
//               DirectX's D3DCOLOR packed structure.
////////////////////////////////////////////////////////////////////
INLINE DWORD DXGraphicsStateGuardian8::
Colorf_to_D3DCOLOR(const Colorf &cColorf) {
// MS VC defines _M_IX86 for x86.  gcc should define _X86_
#if defined(_M_IX86) || defined(_X86_)
  DWORD d3dcolor, tempcolorval=255;

  // note the default FPU rounding mode will give 255*0.5f=0x80, not 0x7F as VC would force it to by resetting rounding mode
  // dont think this makes much difference
  
  __asm {
        push ebx   ; want to save this in case this fn is inlined
        push ecx
        mov ecx, cColorf
        fild tempcolorval
        fld DWORD PTR [ecx]
        fmul ST(0), ST(1)
        fistp tempcolorval  ; no way to store directly to int register
        mov eax, tempcolorval
        shl eax, 16

        fld DWORD PTR [ecx+4]  ;grn
        fmul ST(0), ST(1)
        fistp tempcolorval
        mov ebx, tempcolorval
        shl ebx, 8
        or eax, ebx

        fld DWORD PTR [ecx+8]  ;blue
        fmul ST(0), ST(1)
        fistp tempcolorval
        or eax, tempcolorval

        fld DWORD PTR [ecx+12] ;alpha
        fmul ST(0), ST(1)
        fistp tempcolorval
        ; simulate pop 255.0 off FP stack w/o store, mark top as empty and increment stk ptr
        ffree ST(0)
        fincstp
        mov ebx, tempcolorval
        shl ebx, 24
        or eax, ebx
        mov d3dcolor, eax
        pop ecx
        pop ebx
  }

  //   dxgsg8_cat.debug() << (void*)d3dcolor << endl;
  return d3dcolor;
#else //!_X86_
  return MY_D3DRGBA(cColorf[0], cColorf[1], cColorf[2], cColorf[3]);
#endif //!_X86_
}

////////////////////////////////////////////////////////////////////
//     Function: DXGraphicsStateGuardian8::get_texture_wrap_mode
//       Access: Protected, Static
//  Description: Maps from the Texture's internal wrap mode symbols to
//               GL's.
////////////////////////////////////////////////////////////////////
INLINE D3DTEXTUREADDRESS DXGraphicsStateGuardian8::
get_texture_wrap_mode(Texture::WrapMode wm) {
  switch (wm) {
  case Texture::WM_clamp:
    return D3DTADDRESS_CLAMP;
  case Texture::WM_repeat:
    return D3DTADDRESS_WRAP;
  case Texture::WM_mirror:
    return D3DTADDRESS_MIRROR;
  case Texture::WM_mirror_once:
    return D3DTADDRESS_MIRRORONCE;
  case Texture::WM_border_color:
    return D3DTADDRESS_BORDER;
  }
  dxgsg8_cat.error() << "Invalid Texture::Mode value" << endl;
  return D3DTADDRESS_WRAP;
}

////////////////////////////////////////////////////////////////////
//     Function: DXGraphicsStateGuardian8::get_fog_mode_type
//       Access: Protected, Static
//  Description: Maps from the fog types to gl version
////////////////////////////////////////////////////////////////////
INLINE D3DFOGMODE DXGraphicsStateGuardian8::
get_fog_mode_type(Fog::Mode m) {
  switch (m) {
  case Fog::M_linear:
    return D3DFOG_LINEAR;
  case Fog::M_exponential:
    return D3DFOG_EXP;
  case Fog::M_exponential_squared:
    return D3DFOG_EXP2;
  }
  dxgsg8_cat.error() << "Invalid Fog::Mode value" << endl;
  return D3DFOG_EXP;
}

////////////////////////////////////////////////////////////////////
//     Function: DXGraphicsStateGuardian8::get_tex_mat_sym
//       Access: Protected, Static
//  Description: Returns the nth D3DTS_TEXTURE(n) constant.
////////////////////////////////////////////////////////////////////
INLINE D3DTRANSFORMSTATETYPE DXGraphicsStateGuardian8::
get_tex_mat_sym(int stage_index) {
  return (D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + stage_index);
}

////////////////////////////////////////////////////////////////////
//     Function: DXGraphicsStateGuardian8::get_safe_buffer_start
//       Access: Protected, Static
//  Description: Returns the address of a 64K buffer that is allocated
//               at the beginning of a 64K block.
////////////////////////////////////////////////////////////////////
INLINE unsigned char *DXGraphicsStateGuardian8::
get_safe_buffer_start() {
  if (_temp_buffer == NULL) {
    // Guarantee we get a buffer of size 0x10000 bytes that begins
    // on an even multiple of 0x10000.  We do this by allocating
    // double the required buffer, and then pointing to the first
    // multiple of 0x10000 within that buffer.
    _temp_buffer = new unsigned char[0x1ffff];
    _safe_buffer_start = (unsigned char *)(((long)_temp_buffer + 0xffff) & ~0xffff);
  }

  return _safe_buffer_start;
}
