// CRegionMinimap::GetTGAPositionFromWorldCoordinates(const C3DVector&, C2DCoordI&) const -- world X/Y -> minimap pixel
// Grade: DIFFER(115v115) -- same instructions; retail gives the two fidiv integer temporaries their own frame slots (sub esp,14h) where the lane homes them in the dead pos argument slot (sub esp,0Ch), which reorders the TLX/TLY/size loads
// Leaf: GetTGAPositionFromWorldCoordinates
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066C880, 128 bytes; callee __ftol2 (0x00BFEA70) twice.
// The catalog label "C2DCoordI::operator+=" is propagated and wrong: the body reads the minimap's
// size (+0), RegionMapBox top-left (+4/+8) and RegionMapSquare (+0x14..+0x20) of CRegionMinimap and is
// the inverse of its neighbour GetWorldCoordinatesFromTGAPosition (0x0066C810). Args: [esp+4] = the
// world position (X/Y floats at +0/+4), [esp+8] = the output integer coordinate (`ret 8`).
// The members are read into named long locals (hence the stack homes for `fild`); the float size is
// loaded once and kept on the FPU stack for both axes: pixel = (pos - topLeft) / squareExtent * size.
#include "engine/CRegionMinimap.h"

struct C3DVector { float X; float Y; float Z; };
struct C2DCoordI { long X; long Y; };

struct CRegionMinimap_Methods : CRegionMinimap {
    void GetTGAPositionFromWorldCoordinates(const C3DVector& pos, C2DCoordI& out) const;
};

void CRegionMinimap_Methods::GetTGAPositionFromWorldCoordinates(const C3DVector& pos, C2DCoordI& out) const
{
    long size = RegionMinimapSize;
    C2DCoordI topLeft;
    topLeft.X = RegionMapBox_TLX;
    topLeft.Y = RegionMapBox_TLY;
    out.X = (long)((pos.X - topLeft.X) / (RegionMapSquare_BRX - RegionMapSquare_TLX) * size);
    out.Y = (long)((pos.Y - topLeft.Y) / (RegionMapSquare_BRY - RegionMapSquare_TLY) * size);
}
