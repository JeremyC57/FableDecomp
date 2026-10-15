// CRegionMinimap::GetRegionMapExtents(C2DVectorI& topLeft, C2DExtentsI& extents) const -- report the region map box as its top-left corner and its width/height
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066C6A0, length 45 bytes (`ret 8`). No callees.
// Layout: engine/CRegionMinimap.h (PDB names; the leading members agree with retail): RegionMapBox (C2DBoxI) TLX +0x04,
// TLY +0x08, BRX +0x0c, BRY +0x10.
// Shape: each result is a two-long value object assigned whole -- both words are loaded before either is stored, and
// the box is re-read for the extents because the first store may alias it (`mov edx,[ecx+4] ; ... mov esi,[ecx+8]`).
// The method is attached through a derived shell because the generated header is a plain layout struct.
// Leaf: GetRegionMapExtents
#include "engine/CRegionMinimap.h"

struct C2DVectorI {
    long X, Y;
    C2DVectorI(long x, long y) : X(x), Y(y) {}
};

struct C2DExtentsI {
    long Width, Height;
    C2DExtentsI(long width, long height) : Width(width), Height(height) {}
};

struct CRegionMinimap_GetRegionMapExtents : CRegionMinimap {
    void GetRegionMapExtents(C2DVectorI& topLeft, C2DExtentsI& extents) const;
};

void CRegionMinimap_GetRegionMapExtents::GetRegionMapExtents(C2DVectorI& topLeft, C2DExtentsI& extents) const
{
    topLeft = C2DVectorI(RegionMapBox_TLX, RegionMapBox_TLY);
    extents = C2DExtentsI(RegionMapBox_BRX - RegionMapBox_TLX, RegionMapBox_BRY - RegionMapBox_TLY);
}
