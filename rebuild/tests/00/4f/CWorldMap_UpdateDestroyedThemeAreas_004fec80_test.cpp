// Behaviour test for CWorldMap::UpdateDestroyedThemeAreas @ 0x004FEC80: the destroyed-theme positions are merged into boxes
// no wider or taller than 5 blocks; each box, grown by one block, is reported to HeightAreaChanged; the list is then emptied
#include <stdio.h>
#include <vector>
struct Coord { long X, Y; Coord(long x, long y) : X(x), Y(y) {} };
struct Box {
    long TLX, TLY, BRX, BRY;
    Box(const Coord& tl, const Coord& br) : TLX(tl.X), TLY(tl.Y), BRX(br.X), BRY(br.Y) {}
    void ExtendToContain(const Coord& p) {                      // stub of 0x00A39E20 (inclusive corners)
        if (p.X < TLX) TLX = p.X; if (p.X > BRX) BRX = p.X;
        if (p.Y < TLY) TLY = p.Y; if (p.Y > BRY) BRY = p.Y;
    }
    void ExtendBordersBy(long n) { TLX -= n; TLY -= n; BRX += n; BRY += n; }   // stub of 0x00A39E10
};
static std::vector<Box> g_Changed;
static void HeightAreaChanged(const Box* area) { g_Changed.push_back(*area); }
static void UpdateDestroyedThemeAreas(std::vector<Coord>& positions)
{
    long count = (long)positions.size();
    if (count == 0)
        return;
    Coord pos = positions[0];
    Box area(pos, pos);
    for (long i = 0; i < count; ++i) {
        pos = positions[i];
        Box extended = area;
        extended.ExtendToContain(pos);
        if (extended.BRX - extended.TLX > 5 || extended.BRY - extended.TLY > 5) {
            area.ExtendBordersBy(1);
            HeightAreaChanged(&area);
            area = Box(pos, pos);
        } else
            area = extended;
    }
    area.ExtendBordersBy(1);
    HeightAreaChanged(&area);
    positions.clear();
}
int main()
{
    std::vector<Coord> p;
    UpdateDestroyedThemeAreas(p);
    int ok = g_Changed.empty();
    p.push_back(Coord(10, 10)); p.push_back(Coord(12, 14)); p.push_back(Coord(30, 10)); p.push_back(Coord(31, 11));
    UpdateDestroyedThemeAreas(p);
    ok = ok && p.empty() && g_Changed.size() == 2;
    ok = ok && g_Changed[0].TLX == 9 && g_Changed[0].TLY == 9 && g_Changed[0].BRX == 13 && g_Changed[0].BRY == 15;
    ok = ok && g_Changed[1].TLX == 29 && g_Changed[1].BRX == 32 && g_Changed[1].BRY == 12;
    printf(ok ? "004fec80_TEST PASS\n" : "004fec80_TEST FAIL\n");
    return ok ? 0 : 1;
}