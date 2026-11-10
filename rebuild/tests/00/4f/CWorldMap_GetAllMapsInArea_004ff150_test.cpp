// Behaviour test for CWorldMap::GetAllMapsInArea @ 0x004FF150: the area is clamped to the world, the result cleared; an area
// inside one level gives that level from the placement grid, otherwise every used map (from 1) whose area overlaps it
#include <stdio.h>
#include <vector>
struct Box { long TLX, TLY, BRX, BRY; };
static Box MakeBox(long a, long b, long c, long d) { Box r; r.TLX = a; r.TLY = b; r.BRX = c; r.BRY = d; return r; }
static bool Overlapping(const Box& a, const Box& b) { return a.TLX < b.BRX && b.TLX < a.BRX && a.TLY < b.BRY && b.TLY < a.BRY; }
struct MapInfo { Box Area; bool Used; };
struct World {
    std::vector<MapInfo> Maps; Box Area; long Shift; long Pitch; std::vector<unsigned short> Placement;
    long MapAt(long x, long y) const { return Placement[(y >> Shift) * Pitch + (x >> Shift)]; }
    bool AreaCoversOneLevel(const Box& a) const {
        long m = MapAt(a.TLX, a.TLY);
        return m == MapAt(a.BRX, a.TLY) && m == MapAt(a.TLX, a.BRY) && m == MapAt(a.BRX, a.BRY) && m != 0;
    }
    void GetAllMapsInArea(const Box& area, std::vector<unsigned long>& maps) const {
        Box box = area;
        long max_x = Area.BRX - 1;
        if (box.TLX < 0) box.TLX = 0; else if (box.TLX > max_x) box.TLX = max_x;
        if (box.BRX < 0) box.BRX = 0; else if (box.BRX > max_x) box.BRX = max_x;
        long max_y = Area.BRY - 1;
        if (box.TLY < 0) box.TLY = 0; else if (box.TLY > max_y) box.TLY = max_y;
        if (box.BRY < 0) box.BRY = 0; else if (box.BRY > max_y) box.BRY = max_y;
        maps.clear();
        if (AreaCoversOneLevel(box)) {
            maps.push_back(Placement[(box.TLY >> Shift) * Pitch + (box.TLX >> Shift)]);
            return;
        }
        for (long i = 1; i < (long)Maps.size(); ++i)
            if (Maps[i].Used && Overlapping(Maps[i].Area, box)) maps.push_back(i);
    }
};
int main()
{
    World w; w.Area = MakeBox(0, 0, 64, 32); w.Shift = 4; w.Pitch = 4;
    MapInfo m0 = { MakeBox(0, 0, 0, 0), false }, m1 = { MakeBox(0, 0, 32, 32), true }, m2 = { MakeBox(32, 0, 64, 32), true },
            m3 = { MakeBox(0, 0, 64, 32), false };
    w.Maps.push_back(m0); w.Maps.push_back(m1); w.Maps.push_back(m2); w.Maps.push_back(m3);
    for (long y = 0; y < 2; ++y) for (long x = 0; x < 4; ++x) w.Placement.push_back((unsigned short)(x < 2 ? 1 : 2));
    std::vector<unsigned long> r; r.push_back(99);
    w.GetAllMapsInArea(MakeBox(2, 2, 10, 10), r);
    int ok = r.size() == 1 && r[0] == 1;
    w.GetAllMapsInArea(MakeBox(-5, -5, 100, 100), r);                // clamped, spans both levels
    ok = ok && r.size() == 2 && r[0] == 1 && r[1] == 2;
    w.GetAllMapsInArea(MakeBox(40, 3, 50, 20), r);
    ok = ok && r.size() == 1 && r[0] == 2;
    printf(ok ? "004ff150_TEST PASS\n" : "004ff150_TEST FAIL\n");
    return ok ? 0 : 1;
}