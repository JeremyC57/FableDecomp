// CRegionMinimap::GetRegionMapExtents(C2DCoordI& topLeft, C2DCoordI& bottomRight) const -- report the region map box as its two corners
// Leaf: ?GetRegionMapExtents@CRegionMinimap@@QBEXAAVC2DCoordI@@0@Z
// Retail: Fable.exe @ 0x0066C6D0, 35 bytes, thiscall `ret 8`.
// Identity (proven): Ego_r.pdb and FableWin.pdb both hold the two GetRegionMapExtents overloads in this order,
// (C2DCoordI&, C2DExtentsI&) then (C2DCoordI&, C2DCoordI&); the first is the landed 0x0066C6A0 directly before this body,
// and this one copies RegionMapBox's TL corner (+0x04/+0x08) and BR corner (+0x0C/+0x10) out unchanged. No direct caller
// (the abs hit at 0x00592340 is the rel32 of a call, not a reference).
// Callees: none.
// Shape: each corner is a two-long value object assigned whole (both words loaded before either store, as 0x0066C6A0).
// Layout: engine/CRegionMinimap.h (RegionMapBox_TLX +0x04 .. RegionMapBox_BRY +0x10) through a donor include, so the method
// mangles as CRegionMinimap's own (the landed 0x0066C6A0 uses a CRegionMinimap_GetRegionMapExtents shell name).
#define CRegionMinimap CRegionMinimapDonor
#include "engine/CRegionMinimap.h"
#undef CRegionMinimap

class C2DCoordI {
public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};

class CRegionMinimap : public CRegionMinimapDonor {
public:
    void GetRegionMapExtents(C2DCoordI& topLeft, C2DCoordI& bottomRight) const;
};

void CRegionMinimap::GetRegionMapExtents(C2DCoordI& topLeft, C2DCoordI& bottomRight) const
{
    topLeft = C2DCoordI(RegionMapBox_TLX, RegionMapBox_TLY);
    bottomRight = C2DCoordI(RegionMapBox_BRX, RegionMapBox_BRY);
}