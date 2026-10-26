#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
// Model of NCameraModes::IsLandscapeToBlockCameraBetween(map, a, b, includeWater) @ 0x00874F80: an end outside the map or in an
// unloaded map blocks; otherwise the landscape blocks when a is above its camera height and b is below its own.
class C3DVector { public: float X, Y, Z; };
class CWorldMap {
public:
    float Ground;
    float Water;
    bool  MapLoaded[2];
    virtual ~CWorldMap() { }
    virtual bool IsMapLoaded(int map) const { return MapLoaded[map]; }
    virtual int GetMapNumberAt(const C3DVector& pos) const { return pos.X < 0.0f ? 0 : 1; }
    bool IsPosInMap(const C3DVector& pos) const { return pos.X > -100.0f && pos.X < 100.0f; }
    float GetGroundSizeZAt(const C3DVector&) const { return Ground; }
    float GetWaterSizeZAt(const C3DVector&) const { return Water; }
};
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
bool __fastcall IsLandscapeToBlockCameraBetween(const CWorldMap& map, const C3DVector& a, const C3DVector& b, bool includeWater)
{
    if (!map.IsPosInMap(a) || !map.IsMapLoaded(map.GetMapNumberAt(a)))
        return true;
    if (!map.IsPosInMap(b) || !map.IsMapLoaded(map.GetMapNumberAt(b)))
        return true;
    float heightA = GetCameraHeightfieldHeightAtPos(map, a, includeWater);
    float heightB = GetCameraHeightfieldHeightAtPos(map, b, includeWater);
    if (a.Z > heightA && heightB > b.Z)
        return true;
    return false;
}
int main()
{
    bool ok = true;
    CWorldMap map;
    map.Ground = 10.0f;
    map.Water = 15.0f;
    map.MapLoaded[0] = true;
    map.MapLoaded[1] = true;
    C3DVector high = { 5.0f, 0.0f, 20.0f };
    C3DVector low = { 6.0f, 0.0f, 12.0f };
    C3DVector off = { 500.0f, 0.0f, 20.0f };
    C3DVector west = { -5.0f, 0.0f, 20.0f };
    if (!IsLandscapeToBlockCameraBetween(map, off, high, false)) ok = false;    // a off the map
    if (!IsLandscapeToBlockCameraBetween(map, high, off, false)) ok = false;    // b off the map
    map.MapLoaded[0] = false;
    if (!IsLandscapeToBlockCameraBetween(map, west, high, false)) ok = false;   // a's map not loaded
    if (!IsLandscapeToBlockCameraBetween(map, high, west, false)) ok = false;   // b's map not loaded
    map.MapLoaded[0] = true;
    if (IsLandscapeToBlockCameraBetween(map, high, low, false)) ok = false;     // b above the ground
    if (!IsLandscapeToBlockCameraBetween(map, high, low, true)) ok = false;     // b under the water surface
    if (IsLandscapeToBlockCameraBetween(map, low, high, true)) ok = false;      // a below: not blocked
    if (IsLandscapeToBlockCameraBetween(map, high, west, true)) ok = false;     // both above
    if (!ok) { std::printf("00874f80_TEST FAIL\n"); return 1; }
    std::printf("00874f80_TEST PASS\n");
    return 0;
}