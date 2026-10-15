// Behaviour test for CRegionMinimap::GetRegionMapExtents @ 0x0066C6A0: top-left corner and width/height of the map box.
#include <cstdio>

struct C2DVectorI { long X, Y; C2DVectorI(long x, long y) : X(x), Y(y) {} };
struct C2DExtentsI { long Width, Height; C2DExtentsI(long w, long h) : Width(w), Height(h) {} };

struct CRegionMinimap {
    long RegionMinimapSize, RegionMapBox_TLX, RegionMapBox_TLY, RegionMapBox_BRX, RegionMapBox_BRY;
    void GetRegionMapExtents(C2DVectorI& topLeft, C2DExtentsI& extents) const
    {
        topLeft = C2DVectorI(RegionMapBox_TLX, RegionMapBox_TLY);
        extents = C2DExtentsI(RegionMapBox_BRX - RegionMapBox_TLX, RegionMapBox_BRY - RegionMapBox_TLY);
    }
};

int main()
{
    CRegionMinimap m; m.RegionMinimapSize = 256; m.RegionMapBox_TLX = -10; m.RegionMapBox_TLY = 20; m.RegionMapBox_BRX = 90; m.RegionMapBox_BRY = 50;
    C2DVectorI tl(0, 0); C2DExtentsI ext(0, 0);
    m.GetRegionMapExtents(tl, ext);
    if (tl.X != -10 || tl.Y != 20 || ext.Width != 100 || ext.Height != 30) { std::printf("0066c6a0_TEST FAIL\n"); return 1; }
    std::printf("0066c6a0_TEST PASS\n");
    return 0;
}