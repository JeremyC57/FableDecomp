// 0066c6d0 behaviour test for CRegionMinimap::GetRegionMapExtents(C2DCoordI&, C2DCoordI&) const
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __cdecl
#define __cdecl
#endif
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed at line %d\n", __LINE__); g_fail = 1; } } while (0)
class C2DCoordI {
public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
class CRegionMinimap {
public:
    long RegionMinimapSize;
    long RegionMapBox_TLX, RegionMapBox_TLY, RegionMapBox_BRX, RegionMapBox_BRY;
    void GetRegionMapExtents(C2DCoordI& topLeft, C2DCoordI& bottomRight) const;
};
void CRegionMinimap::GetRegionMapExtents(C2DCoordI& topLeft, C2DCoordI& bottomRight) const
{
    topLeft = C2DCoordI(RegionMapBox_TLX, RegionMapBox_TLY);
    bottomRight = C2DCoordI(RegionMapBox_BRX, RegionMapBox_BRY);
}
static void run()
{
    CRegionMinimap m = { 256, -10, 20, 300, 400 };
    C2DCoordI tl(0, 0), br(0, 0);
    m.GetRegionMapExtents(tl, br);
    CHECK(tl.X == -10 && tl.Y == 20);
    CHECK(br.X == 300 && br.Y == 400);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("0066c6d0_TEST FAIL\n");
        return 1;
    }
    std::printf("0066c6d0_TEST PASS\n");
    return 0;
}