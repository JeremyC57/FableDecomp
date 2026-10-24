#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
// Model of NCameraModes::GetCameraHeightfieldHeightAtPos(map, pos, includeWater) @ 0x00874F40: the ground height at pos, or the water surface when
// water counts and lies above the ground.
struct C3DVector { float X, Y, Z; };
class CWorldMap {
public:
    float Ground;
    float Water;
    float GetGroundSizeZAt(const C3DVector& pos) const { return Ground + pos.X * 0.0f; }   // 0x0063A300
    float GetWaterSizeZAt(const C3DVector& pos) const { return Water + pos.Y * 0.0f; }     // 0x0063A420
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
int main()
{
    bool ok = true;
    CWorldMap map;
    C3DVector pos = { 1.0f, 2.0f, 3.0f };
    map.Ground = 10.0f;
    map.Water = 12.5f;
    if (GetCameraHeightfieldHeightAtPos(map, pos, false) != 10.0f) ok = false;   // water ignored
    if (GetCameraHeightfieldHeightAtPos(map, pos, true) != 12.5f) ok = false;    // flooded: the water surface
    map.Water = 4.0f;
    if (GetCameraHeightfieldHeightAtPos(map, pos, true) != 10.0f) ok = false;    // water below ground
    map.Water = 10.0f;
    if (GetCameraHeightfieldHeightAtPos(map, pos, true) != 10.0f) ok = false;
    if (!ok) { std::printf("00874f40_TEST FAIL\n"); return 1; }
    std::printf("00874f40_TEST PASS\n");
    return 0;
}