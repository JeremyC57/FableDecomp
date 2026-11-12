// CPixelFormat::GetInit() const -- build the CPixelFormatInit description of this format from its CD3DFormatInfo table entry: pixel type, colour depth, R/G/B/A bit counts and the linear flag
// Leaf: ?GetInit@CPixelFormat@@QBE?AVCPixelFormatInit@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E3870, 208 bytes, thiscall, `ret 4` (hidden return pointer), 6-entry jump table
//   at 0x009E3928 (counted in the row).
// Identity (proven): Ego_r.pdb ?GetInit@CPixelFormat@@QBE?AVCPixelFormatInit@@XZ (const, CPixelFormatInit by value: the 6-byte
//   Ego_r CPixelFormatInit {Type:4, Linear:1 | ColourDepth | RBits | GBits | BBits | ABits} is exactly what the body fills and
//   copies out through the hidden pointer); `this` is CPixelFormat {FormatIndex} (engine/CPixelFormat.h) indexing the 36-byte
//   Ego_r CD3DFormatInfo table at 0x0129BA40 {D3DFormat, PFType +4, BitDepth +8, AlphaBits +0xc, RBits +0x10, GBits +0x14,
//   BBits +0x18, StencilBits +0x1c, Linear +0x20}. Callers 0x00993F6F, 0x009FB191, 0x00A68B7D.
// Globals: the CD3DFormatInfo table 0x0129BA40 under its bound spelling GD3DFormats (no PDB data symbol; NOTES.md).
// Shape: a switch over PFType 2..7 (3 has no arm) assigning the 4-bit Type, then the byte members in declaration order and
//   Linear last; every member re-indexes GD3DFormats[FormatIndex] (a `const CD3DFormatInfo&` local forms the entry
//   address with `lea` first: probe 2, 188v208). The arms sit in source case order (PFType 2, 5, 4, 7, 6 -> code order
//   or 1/3/2/5/4); the jump table pairing PFType->Type (2->1, 4->2, 5->3, 6->4, 7->5, 3->unchanged) was checked against
//   the retail table 0x009E3928 entry by entry (relocation masking cannot). The local is copied to the hidden return slot.
#define CPixelFormat CPixelFormat_Layout                  // layout-rename include: the generated header has no methods
#include "engine/CPixelFormat.h"
#undef CPixelFormat
#include "engine/CD3DFormatInfo.h"   // CD3DFormatInfo + GD3DFormats 0x0129BA40 (NF28: was a local struct model and g_D3DFormatInfo)
class CPixelFormatInit {                                 // Ego_r.pdb, 6 bytes
public:
    unsigned char Type : 4;                              // +0x00
    unsigned char Linear : 1;
    unsigned char ColourDepth;                           // +0x01
    unsigned char RBits;                                 // +0x02
    unsigned char GBits;                                 // +0x03
    unsigned char BBits;                                 // +0x04
    unsigned char ABits;                                 // +0x05
};
class CPixelFormat : public CPixelFormat_Layout {       // FormatIndex +0x00
public:
    CPixelFormatInit GetInit() const;
};
CPixelFormatInit CPixelFormat::GetInit() const
{
    CPixelFormatInit init;
    switch (GD3DFormats[FormatIndex].PFType) {
    case 2: init.Type = 1; break;
    case 5: init.Type = 3; break;
    case 4: init.Type = 2; break;
    case 7: init.Type = 5; break;
    case 6: init.Type = 4; break;
    }
    init.ColourDepth = (unsigned char)GD3DFormats[FormatIndex].BitDepth;
    init.RBits = (unsigned char)GD3DFormats[FormatIndex].RBits;
    init.GBits = (unsigned char)GD3DFormats[FormatIndex].GBits;
    init.BBits = (unsigned char)GD3DFormats[FormatIndex].BBits;
    init.ABits = (unsigned char)GD3DFormats[FormatIndex].AlphaBits;
    init.Linear = GD3DFormats[FormatIndex].Linear;
    return init;
}