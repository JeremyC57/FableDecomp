#include <cstdio>
struct C2DBoxI { long TLX, TLY, BRX, BRY; };
struct CWorldMap
{
    long Granularity;
    long MapPlacementPitch;
    C2DBoxI Area;
    unsigned short* MapPlacement_start;
    void ChangeMapPlacementValuesTo(long from, long to);
};
void CWorldMap::ChangeMapPlacementValuesTo(long from, long to)
{
    for (long y = 0; y < Area.BRY / Granularity; ++y)
    {
        unsigned short* row = MapPlacement_start + MapPlacementPitch * y;
        for (long x = 0; x < Area.BRX / Granularity; ++x)
        {
            if (row[x] == from)
                row[x] = (unsigned short)to;
        }
    }
}
int main()
{
    unsigned short grid[4 * 5];
    for (int i = 0; i < 20; ++i) grid[i] = (unsigned short)(i % 3);
    CWorldMap w; w.Granularity = 8; w.MapPlacementPitch = 5; w.Area.TLX = 0; w.Area.TLY = 0;
    w.Area.BRX = 32; w.Area.BRY = 24; w.MapPlacement_start = grid;   // 4 columns x 3 rows of a 5-wide grid
    w.ChangeMapPlacementValuesTo(1, 9);
    for (int y = 0; y < 4; ++y)
        for (int x = 0; x < 5; ++x)
        {
            int i = y * 5 + x;
            int expect = i % 3;
            if (y < 3 && x < 4 && expect == 1) expect = 9;
            if (grid[i] != expect) { std::printf("004fcac0_TEST FAIL\n"); return 1; }
        }
    std::printf("004fcac0_TEST PASS\n");
    return 0;
}