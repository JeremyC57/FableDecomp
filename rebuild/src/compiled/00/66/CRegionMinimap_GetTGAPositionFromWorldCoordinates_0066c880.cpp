// CRegionMinimap::GetTGAPositionFromWorldCoordinates -- map world coordinates to minimap pixels.
// Retail: 0x0066C880, 115 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?GetTGAPositionFromWorldCoordinates@CRegionMinimap@@QBEXABVC3DVector@@AAVC2DCoordI@@@Z
// Flags: none
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CREGIONMINIMAP
#define FABLE_RETAIL_CREGIONMINIMAP_TGA_COORDS
#include "engine/CRegionMinimap.h"
void CRegionMinimap::GetTGAPositionFromWorldCoordinates(const C3DVector& worldCoord,C2DCoordI& tgaPos) const
{
    C2DCoordI regionOrigin;
    C2DExtentsI regionExtents;
    GetRegionMapExtents(regionOrigin,regionExtents);
    long minimapPixelSize=GetRegionMinimapSize();
    tgaPos.X=(long)((worldCoord.X-regionOrigin.X)/RegionMapSquare.GetExtents().X*minimapPixelSize);
    tgaPos.Y=(long)((worldCoord.Y-regionOrigin.Y)/RegionMapSquare.GetExtents().Y*minimapPixelSize);
}