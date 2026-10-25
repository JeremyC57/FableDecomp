// Behaviour test (0x004fc080): CWorldMap::GetMapNumberAt looks a world coordinate up in the placement grid, 0 outside the area
#include <cstdio>
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class C2DCoordI { public: long X, Y; };
class CWorldMap {
public:
    long            GranularityShift;
    long            MapPlacementPitch;
    C2DBoxI         Area;
    unsigned short* MapPlacement;
    long GetMapNumberAt(const C2DCoordI& pos) const
    {
        if (pos.X >= Area.TLX && pos.X < Area.BRX && pos.Y >= Area.TLY && pos.Y < Area.BRY)
            return MapPlacement[(pos.Y >> GranularityShift) * MapPlacementPitch + (pos.X >> GranularityShift)];
        return 0;
    }
};
int main()
{
    unsigned short grid[12];
    for (int i = 0; i < 12; ++i) grid[i] = (unsigned short)(100 + i);
    CWorldMap wm; wm.GranularityShift = 4; wm.MapPlacementPitch = 4;
    wm.Area.TLX = 0; wm.Area.TLY = 0; wm.Area.BRX = 64; wm.Area.BRY = 48; wm.MapPlacement = grid;
    C2DCoordI a; a.X = 17; a.Y = 33;            // cell (1, 2) -> 2 * 4 + 1 = 9
    C2DCoordI b; b.X = 64; b.Y = 0;             // right edge excluded
    C2DCoordI c; c.X = 0; c.Y = -1;             // above the area
    C2DCoordI d; d.X = 63; d.Y = 47;            // last cell (3, 2) -> 11
    if (wm.GetMapNumberAt(a) != 109 || wm.GetMapNumberAt(b) != 0 || wm.GetMapNumberAt(c) != 0 || wm.GetMapNumberAt(d) != 111)
        { std::printf("004fc080_TEST FAIL\n"); return 1; }
    std::printf("004fc080_TEST PASS\n");
    return 0;
}