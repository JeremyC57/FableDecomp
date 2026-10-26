// CRegionMinimap::GetWorldCoordinatesFromTGAPosition(const C2DVectorI& tgaPosition) const -- map a pixel of the region minimap TGA back to world coordinates (z = 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066C810, length 100 bytes (`ret 8`: hidden C3DVector result + the
// position). No callees.
// Layout: engine/CRegionMinimap.h (PDB names; the leading members agree with retail): RegionMinimapSize +0,
// RegionMapBox TLX +4, TLY +8, RegionMapSquare TLX +0x14, TLY +0x18, BRX +0x1c, BRY +0x20.
// Shape: the integer operands of `fild`/`fimul`/`fiadd` are frame copies. The square's extent comes from an inline
// by-value getter (GetSquareSize, a stand-in for the C2DBoxI size) evaluated once into a named local for X and again as a
// temporary for Y: each 8-byte object keeps its own frame slot with the unused half's store dead (retail 0x1c frame, Y at
// +0x10, X at +0x14, two unused words); the top-left local is declared before the size (its loads come first). The world
// position is built in the hidden result by an inline C3DVector(x, y, z) constructor (arguments evaluate z, y, x).
// The method is attached through a derived shell because the generated header is a plain layout struct.
// Leaf: GetWorldCoordinatesFromTGAPosition
#include "engine/CRegionMinimap.h"

struct C2DVectorI {
    long X, Y;
    C2DVectorI(long x, long y) : X(x), Y(y) {}
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

struct CRegionMinimap_GetWorldCoordinatesFromTGAPosition : CRegionMinimap {
    C3DVector GetWorldCoordinatesFromTGAPosition(const C2DVectorI& tgaPosition) const;
    C2DVectorI GetSquareSize() const { return C2DVectorI(RegionMapSquare_BRX - RegionMapSquare_TLX, RegionMapSquare_BRY - RegionMapSquare_TLY); }
};

C3DVector CRegionMinimap_GetWorldCoordinatesFromTGAPosition::GetWorldCoordinatesFromTGAPosition(const C2DVectorI& tgaPosition) const
{
    C2DVectorI topLeft(RegionMapBox_TLX, RegionMapBox_TLY);
    long size = RegionMinimapSize;
    C2DVectorI squareSize = GetSquareSize();
    return C3DVector(topLeft.X + (float)tgaPosition.X / (float)size * squareSize.X,
                     topLeft.Y + (float)tgaPosition.Y / (float)size * GetSquareSize().Y, 0.0f);
}