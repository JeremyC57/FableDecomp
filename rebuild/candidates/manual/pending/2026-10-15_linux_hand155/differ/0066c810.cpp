// CRegionMinimap::GetWorldCoordinatesFromTGAPosition(const C2DVectorI& tgaPosition) const -- map a pixel of the region minimap TGA back to world coordinates (z = 0)
// Grade: DIFFER(100v100) -- frame layout only: same instructions, but retail has a 0x1c frame with the box top-left at [esp+4]/[esp+8] and the square extents at [esp+10h]/[esp+14h] (two unused words), ours a 0x14 frame with extents at +4 and top-left at +0xc. Tried: a named float size (gives a reciprocal multiply), (float) casts at both uses, default-construct + assign-from-temporary value objects.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066C810, length 100 bytes (`ret 8`: hidden C3DVector result + the
// position). No callees.
// Layout: engine/CRegionMinimap.h (PDB names; the leading members agree with retail): RegionMinimapSize +0,
// RegionMapBox TLX +4, TLY +8, RegionMapSquare TLX +0x14, TLY +0x18, BRX +0x1c, BRY +0x20.
// Shape: the integer operands of `fild`/`fimul`/`fiadd` are frame copies, so the size, the box's top-left and the
// square's extents are locals; the world position is `topLeft + tga / size * extent` per axis, built in the hidden
// result by an inline C3DVector(x, y, z) constructor (arguments evaluate right to left: z, then y, then x).
// The method is attached through a derived shell because the generated header is a plain layout struct.
// Leaf: GetWorldCoordinatesFromTGAPosition
#include "engine/CRegionMinimap.h"

struct C2DVectorI {
    long X, Y;
    C2DVectorI(long x, long y) : X(x), Y(y) {}
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

struct CRegionMinimap_GetWorldCoordinatesFromTGAPosition : CRegionMinimap {
    C3DVector GetWorldCoordinatesFromTGAPosition(const C2DVectorI& tgaPosition) const;
};

C3DVector CRegionMinimap_GetWorldCoordinatesFromTGAPosition::GetWorldCoordinatesFromTGAPosition(const C2DVectorI& tgaPosition) const
{
    long size = RegionMinimapSize;
    C2DVectorI topLeft(RegionMapBox_TLX, RegionMapBox_TLY);
    C2DVectorI extent(RegionMapSquare_BRX - RegionMapSquare_TLX, RegionMapSquare_BRY - RegionMapSquare_TLY);
    return C3DVector(topLeft.X + (float)tgaPosition.X / (float)size * extent.X,
                     topLeft.Y + (float)tgaPosition.Y / (float)size * extent.Y, 0.0f);
}
