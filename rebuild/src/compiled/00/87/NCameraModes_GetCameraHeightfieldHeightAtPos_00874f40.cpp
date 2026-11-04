// NCameraModes::GetCameraHeightfieldHeightAtPos(const CWorldMap& map, const C3DVector& pos, bool includeWater) -- the height the camera must stay above at pos: the ground height, or the water surface when it is higher and water counts
// Leaf: ?GetCameraHeightfieldHeightAtPos@NCameraModes@@YIMABVCWorldMap@@ABVC3DVector@@_N@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00874F40, length 56 bytes (+ int3 padding), `__fastcall` (map in ecx, pos
//   in edx, the flag on the stack), `ret 4`.
// Identity: Ego_r.pdb `?GetCameraHeightfieldHeightAtPos@NCameraModes@@YIMABVCWorldMap@@ABVC3DVector@@_N@Z` (a namespace
//   function, both by const reference), the catalog name; called twice by NCameraModes::IsLandscapeToBlockCameraBetween
//   0x00874F80 (this chunk) for both segment ends.
// Callees: CWorldMap::GetGroundSizeZAt(const C3DVector&) const @ 0x0063A300, CWorldMap::GetWaterSizeZAt(const C3DVector&)
//   const @ 0x0063A420 (as the landed CTools::GetLandscapePointIntersectionFromTo 0x00875550).
// Shape: the ground height is a named float in the `push ecx` frame word; the water height stays on the x87 stack and
//   wins only when strictly higher (`fcom ; test ah,41h ; je` keeps it, else `fstp st(0)` and reload the ground).
#include "engine/C3DVector.h"

class CWorldMap {
public:
    float GetGroundSizeZAt(const C3DVector& pos) const;    // 0x0063A300
    float GetWaterSizeZAt(const C3DVector& pos) const;     // 0x0063A420
};

namespace NCameraModes {
float __fastcall GetCameraHeightfieldHeightAtPos(const CWorldMap& map, const C3DVector& pos, bool includeWater)
{
    float ground = map.GetGroundSizeZAt(pos);
    if (includeWater) {
        float water = map.GetWaterSizeZAt(pos);
        if (water > ground)
            return water;
    }
    return ground;
}
}