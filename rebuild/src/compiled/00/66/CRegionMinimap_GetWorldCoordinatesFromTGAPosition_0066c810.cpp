// CRegionMinimap::GetWorldCoordinatesFromTGAPosition(C3DVector& world, const C2DCoordI& tga) const -- map a pixel of the region minimap TGA back to world coordinates (z = 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066C810, length 100 bytes (`ret 8`: hidden C3DVector result + the
// position). No callees.
// Layout: engine/CRegionMinimap.h (PDB names; the leading members agree with retail): RegionMinimapSize +0,
// RegionMapBox TLX +4, TLY +8, RegionMapSquare TLX +0x14, TLY +0x18, BRX +0x1c, BRY +0x20.
// Shape: the integer operands of `fild`/`fimul`/`fiadd` are frame copies. The square's extent comes from an inline
// by-value getter (GetSquareSize, a stand-in for the C2DBoxI size) evaluated once into a named local for X and again as a
// temporary for Y: each 8-byte object keeps its own frame slot with the unused half's store dead (retail 0x1c frame, Y at
// +0x10, X at +0x14, two unused words); the top-left local is declared before the size (its loads come first). The world
// position is built in the hidden result by an inline C3DVector(x, y, z) constructor (arguments evaluate z, y, x).
// Leaf: ?GetWorldCoordinatesFromTGAPosition@CRegionMinimap@@QBEXAAVC3DVector@@ABVC2DCoordI@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Port (NF40, round 4): the PDB member (Ego_r.pdb ?GetWorldCoordinatesFromTGAPosition@CRegionMinimap@@QBEXAAVC3DVector@@ABVC2DCoordI@@@Z;
//   the C3DVector is an out parameter, not a return) on engine/CRegionMinimap.h (FABLE_RETAIL_CREGIONMINIMAP + the overlay opt-in
//   FABLE_RETAIL_CREGIONMINIMAP_WORLDCOORDS_OUTOFLINE, declaration only), defined with the header's WORLDCOORDS_INITIALISE text
//   (wfD4: "compiles out of line to 0x0066C810 byte for byte"). Replaces the local derived shell and C2DVectorI stand-in.
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CREGIONMINIMAP
#define FABLE_RETAIL_CREGIONMINIMAP_WORLDCOORDS_OUTOFLINE
#include "engine/C2DCoordI.h"
#include "engine/C3DVector.h"
#include "engine/CRegionMinimap.h"

void CRegionMinimap::GetWorldCoordinatesFromTGAPosition(C3DVector& world, const C2DCoordI& tga) const
{
    C2DCoordI topLeft = RegionMapBox.GetTLPos();
    long size = RegionMinimapSize;
    C2DExtentsI squareSize = RegionMapSquare.GetExtents();
    world.Initialise(topLeft.X + (float)tga.X / (float)size * squareSize.X,
                     topLeft.Y + (float)tga.Y / (float)size * RegionMapSquare.GetExtents().Y, 0.0f);
}
