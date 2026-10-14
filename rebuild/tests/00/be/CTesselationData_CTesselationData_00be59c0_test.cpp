// Behaviour test for CTesselationData::CTesselationData @ 0x00be59c0: base constructed, five members stored (map first argument lands at +0x14).
#include <cstdio>
#include <cstddef>
static int g_base = 0;
struct CEngineMap { int dummy; };
struct CBase { virtual void v0() { } CBase() { ++g_base; } };
struct CTesselationData : CBase {
    long PatchWorldX; long PatchWorldY; long Width; long Height; CEngineMap* EngineMap;
    CTesselationData(CEngineMap* map, long worldX, long worldY, unsigned short width, unsigned short height)
        : PatchWorldX(worldX), PatchWorldY(worldY), Width(width), Height(height), EngineMap(map) { }
};
int main()
{
    CEngineMap map;
    CTesselationData t(&map, -3, 7, 0xFFFF, 12);
    bool ok = g_base == 1 && t.PatchWorldX == -3 && t.PatchWorldY == 7 && t.Width == 65535 && t.Height == 12 && t.EngineMap == &map
        && offsetof(CTesselationData, EngineMap) == 0x14 && offsetof(CTesselationData, Width) == 0x0c;
    if (!ok) { std::printf("00be59c0_TEST FAIL\n"); return 1; }
    std::printf("00be59c0_TEST PASS\n");
    return 0;
}