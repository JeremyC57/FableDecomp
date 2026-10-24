// NCameraModes::GetCameraHeightfieldHeightAtPos(const CWorldMap& map, const C3DVector& pos, bool includeWater) -- the height the camera must stay above at pos: the ground height, or the water surface when it is higher and water counts
// Leaf: ?GetCameraHeightfieldHeightAtPos@NCameraModes@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00874F40, length 56 bytes (+ int3 padding), `__fastcall` (map in ecx, pos
//   in edx, the flag on the stack), `ret 4`.
// Identity: Ego_r.pdb `?GetCameraHeightfieldHeightAtPos@NCameraModes@@YIMABVCWorldMap@@ABVC3DVector@@_N@Z` (a namespace
//   function, both by const reference), the catalog name; called twice by NCameraModes::IsLandscapeToBlockCameraBetween
//   0x00874F80 (this chunk) for both segment ends.
// Callees: CWorldMap::GetGroundSizeZAt(const C3DVector&) const @ 0x0063A300, CWorldMap::GetWaterSizeZAt(const C3DVector&)
//   const @ 0x0063A420 (as the landed CTools::GetLandscapePointIntersectionFromTo 0x00875550).
// Shape: the ground height is a named float in the `push ecx` frame word; the water height stays on the x87 stack and
//   wins only when strictly higher (`fcom ; test ah,41h ; je` keeps it, else `fstp st(0)` and reload the ground).
class C3DVector {
public:
    float X, Y, Z;
};

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