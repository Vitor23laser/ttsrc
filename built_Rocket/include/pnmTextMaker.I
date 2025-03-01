// Filename: pnmTextMaker.I
// Created by:  drose (07Sep03)
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
//     Function: PNMTextMaker::is_valid
//       Access: Public
//  Description: Returns true if the PNMTextMaker is valid and ready to
//               generate text, false otherwise.
////////////////////////////////////////////////////////////////////
INLINE bool PNMTextMaker::
is_valid() const {
  return _is_valid;
}

////////////////////////////////////////////////////////////////////
//     Function: PNMTextMaker::set_align
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE void PNMTextMaker::
set_align(PNMTextMaker::Alignment align_type) {
  _align = align_type;
}

////////////////////////////////////////////////////////////////////
//     Function: PNMTextMaker::get_align
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE PNMTextMaker::Alignment PNMTextMaker::
get_align() const {
  return _align;
}

////////////////////////////////////////////////////////////////////
//     Function: PNMTextMaker::set_interior_flag
//       Access: Published
//  Description: Sets the flag that indicates whether the interior of
//               hollow fonts is identified as a preprocess as each
//               glyph is loaded.  If this flag is true, you may
//               specify an interior color along with a fg and bg
//               color when you place text; if the flag is false, the
//               interior color is ignored.
//
//               It is generally best to set_native_antialias(0) when
//               using this feature.  Also, this works best when the
//               pixel size is not very small.
////////////////////////////////////////////////////////////////////
INLINE void PNMTextMaker::
set_interior_flag(bool interior_flag) {
  if (_interior_flag != interior_flag) {
    _interior_flag = interior_flag;
    empty_cache();
  }
}

////////////////////////////////////////////////////////////////////
//     Function: PNMTextMaker::get_interior_flag
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE bool PNMTextMaker::
get_interior_flag() const {
  return _interior_flag;
}

////////////////////////////////////////////////////////////////////
//     Function: PNMTextMaker::set_fg
//       Access: Published
//  Description: Sets the foreground color of text that will be
//               generated by future calls to generate_into().  This
//               is the color that all of the "on" pixels in the font
//               will show as.
////////////////////////////////////////////////////////////////////
INLINE void PNMTextMaker::
set_fg(const Colorf &fg) {
  _fg = fg;
}

////////////////////////////////////////////////////////////////////
//     Function: PNMTextMaker::get_fg
//       Access: Published
//  Description: Returns the foreground color of text that will be
//               generated by future calls to generate_into().
////////////////////////////////////////////////////////////////////
INLINE const Colorf &PNMTextMaker::
get_fg() const {
  return _fg;
}

////////////////////////////////////////////////////////////////////
//     Function: PNMTextMaker::set_interior
//       Access: Published
//  Description: Sets the color that will be used to render the
//               interior portions of hollow fonts in future calls to
//               generate_into().  This is respected only if
//               interior_flag is true.
////////////////////////////////////////////////////////////////////
INLINE void PNMTextMaker::
set_interior(const Colorf &interior) {
  _interior = interior;
}

////////////////////////////////////////////////////////////////////
//     Function: PNMTextMaker::get_interior
//       Access: Published
//  Description: Returns the color that will be used to render the
//               interior portions of hollow fonts.
////////////////////////////////////////////////////////////////////
INLINE const Colorf &PNMTextMaker::
get_interior() const {
  return _interior;
}

////////////////////////////////////////////////////////////////////
//     Function: PNMTextMaker::generate_into
//       Access: Public
//  Description: Generates a single line of text into the indicated
//               image at the indicated position; the return value is
//               the total width in pixels.
////////////////////////////////////////////////////////////////////
INLINE int PNMTextMaker::
generate_into(const string &text, PNMImage &dest_image, int x, int y) {
  TextEncoder encoder;
  encoder.set_text(text);
  return generate_into(encoder.get_wtext(), dest_image, x, y);
}

////////////////////////////////////////////////////////////////////
//     Function: PNMTextMaker::calc_width
//       Access: Public
//  Description: Returns the width in pixels of the indicated line of
//               text.
////////////////////////////////////////////////////////////////////
INLINE int PNMTextMaker::
calc_width(const string &text) {
  TextEncoder encoder;
  encoder.set_text(text);
  return calc_width(encoder.get_wtext());
}
