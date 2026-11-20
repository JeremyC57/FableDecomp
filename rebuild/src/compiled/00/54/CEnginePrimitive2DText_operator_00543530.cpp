// CEnginePrimitive2DText::operator=(const CEnginePrimitive2DText& other) -- member-wise copy of the 0x2C-byte 2D text primitive
// Leaf: ??4CEnginePrimitive2DText@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00543530, 91 bytes, thiscall, `ret 4`. Callee: CWideString::operator= 0x0099B7D0 (Text +0x24).
// Identity (two sources): Ego_r.pdb ??4CEnginePrimitive2DText@@QAEAAV0@ABV0@@Z (0x005BD000, beside the 2D-text constructor 0x005BD060 =
//   retail 0x00543910); the body copies exactly the Ego_r.pdb layout of CEnginePrimitive2DText (engine/CEnginePrimitive2DText.h: the
//   CEnginePrimitive base +0 (Type, InterpolationSet, ThemeChannelDesc), Pos +0xC, Layer +0x18, Scale +0x1C, Colour +0x20, Text +0x24,
//   PFont +0x28); its callers are the 2D-text draws CText::Draw 0x0054EF00 (landed, already binding this spelling: 3 calls) and three
//   calls at 0x00542D19 / 0x00542DB5 / 0x00542E1E in the CIconText code. No other reference: not a fold.
// Shape: the base and Pos are copied through their implicit assignments (the first with the source in eax, the second with `lea` to
//   both triples), then the three dwords, the string (out of line), the font pointer; `this` is returned.
// Inferred: the member-wise body (the PDB has the declaration only).
// Was: STextRecord_00543530::operator= on a local record model (rebuild/src/compiled/00/54/STextRecord00543530_operator_00543530.cpp).
// Header: engine/CEnginePrimitive2DText.h.
#include "engine/CEnginePrimitive2DText.h"

CEnginePrimitive2DText& CEnginePrimitive2DText::operator=(const CEnginePrimitive2DText& other)
{
    CEnginePrimitive::operator=(other);
    Pos = other.Pos;
    Layer = other.Layer;
    Scale = other.Scale;
    Colour = other.Colour;
    Text = other.Text;
    PFont = other.PFont;
    return *this;
}
