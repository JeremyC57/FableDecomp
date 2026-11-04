// Behaviour test for CRegionMinimap::InitialiseRegionBox (0x0066C7B0): a model of the row -- the square keeps the map box's
// top-left corner and takes the larger of its width and height for both sides.
#include <stdio.h>
struct Box { long TLX, TLY, BRX, BRY; };
struct Minimap { long Size; Box RegionMapBox; Box RegionMapSquare;
    void InitialiseRegionBox()
    {
        long x = RegionMapBox.TLX, y = RegionMapBox.TLY;
        long w = RegionMapBox.BRX - x, h = RegionMapBox.BRY - y;
        long bx, by;
        if (w == h) { bx = x + w; by = y + h; }
        else if (w > h) { bx = x + w; by = y + w; }
        else { bx = x + h; by = y + h; }
        RegionMapSquare.TLX = x; RegionMapSquare.TLY = y; RegionMapSquare.BRX = bx; RegionMapSquare.BRY = by;
    }
};
int main()
{
    Minimap m = { 0, { -10, 20, 90, 50 }, { 0, 0, 0, 0 } };
    m.InitialiseRegionBox();
    int ok = m.RegionMapSquare.TLX == -10 && m.RegionMapSquare.TLY == 20 && m.RegionMapSquare.BRX == 90 && m.RegionMapSquare.BRY == 120;
    Box tall = { 5, 5, 15, 45 }; m.RegionMapBox = tall; m.InitialiseRegionBox();
    ok = ok && m.RegionMapSquare.BRX == 45 && m.RegionMapSquare.BRY == 45;
    Box sq = { 1, 2, 11, 12 }; m.RegionMapBox = sq; m.InitialiseRegionBox();
    ok = ok && m.RegionMapSquare.BRX == 11 && m.RegionMapSquare.BRY == 12 && m.RegionMapSquare.TLX == 1;
    printf(ok ? "0066c7b0_TEST PASS\n" : "0066c7b0_TEST FAIL\n");
    return ok ? 0 : 1;
}