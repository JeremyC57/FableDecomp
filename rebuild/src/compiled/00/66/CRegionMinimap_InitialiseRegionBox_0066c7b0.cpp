// CRegionMinimap::InitialiseRegionBox() -- derive the square region-map box: the region map box's top-left corner with both sides set to the larger of its width and height
// Leaf: ?InitialiseRegionBox@CRegionMinimap@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066C7B0, 94 bytes, thiscall `ret`. No callees.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?InitialiseRegionBox@CRegionMinimap@@QAEXXZ; the same TU holds the two
//   GetRegionMapExtents overloads 0x0066C6A0 / 0x0066C6D0 (PDB order), the first expanded here.
// Shape: GetRegionMapExtents(top_left, extents) through the inline C2DBoxI::GetTLPos / GetExtents (Ego_r.pdb names; the
//   by-value results make VC7.1 copy TLX/TLY through edx/ebx before the subtraction, as retail does), then three arms
//   (equal, wider, taller) that build the bottom-right corner as a C2DCoordI and store the square's four corners.
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CREGIONMINIMAP
#include "engine/CRegionMinimap.h"
void CRegionMinimap::InitialiseRegionBox()
{
    C2DCoordI top_left;
    C2DExtentsI extents;
    GetRegionMapExtents(top_left, extents);
    C2DCoordI bottom_right;
    if (extents.X == extents.Y)
        bottom_right = C2DCoordI(top_left.X + extents.X, top_left.Y + extents.Y);
    else if (extents.X > extents.Y)
        bottom_right = C2DCoordI(top_left.X + extents.X, top_left.Y + extents.X);
    else
        bottom_right = C2DCoordI(top_left.X + extents.Y, top_left.Y + extents.Y);
    RegionMapSquare.TLX = top_left.X;
    RegionMapSquare.TLY = top_left.Y;
    RegionMapSquare.BRX = bottom_right.X;
    RegionMapSquare.BRY = bottom_right.Y;
}