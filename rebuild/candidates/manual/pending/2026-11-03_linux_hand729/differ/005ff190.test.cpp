// Behaviour test for CTCInventoryMap::CreateMapMarkers 0x005FF190 (parked): models the two pieces of arithmetic the row owns --
// the region-path key (min(from,to) * 1000 + max(from,to), looked up in CUIMiscThingsDef::MapPaths) with the path sprite at the
// midpoint of the two regions' world-map offsets, and the per-region quest-marker spreading (no shift for a single quest; otherwise
// ++index selects -X, +X, -Y, +Y by 15 units for 0..3).
#include <cstdio>
struct V2 { float X, Y; };
static unsigned long PathKey(long from, long to) { long lo = to < from ? to : from; long hi = from < to ? to : from; return lo * 1000 + hi; }
static V2 Midpoint(const V2& a, const V2& b) { V2 r; r.X = a.X + (b.X - a.X) * 0.5f; r.Y = a.Y + (b.Y - a.Y) * 0.5f; return r; }
static void Spread(long count, long& index, V2& p)
{
    if (count != 1 || index != 0) {
        switch (++index) {
        case 0: p.X -= 15.0f; break;
        case 1: p.X += 15.0f; break;
        case 2: p.Y -= 15.0f; break;
        case 3: p.Y += 15.0f; break;
        }
    }
}
static bool Near(float a, float b) { return a - b < 0.001f && b - a < 0.001f; }
int main()
{
    if (PathKey(7, 3) != 3007 || PathKey(3, 7) != 3007 || PathKey(5, 5) != 5005) { std::printf("005ff190_TEST FAIL\n"); return 1; }
    V2 a = { 10.0f, 20.0f }, b = { 30.0f, -20.0f };
    V2 m = Midpoint(a, b);
    if (!Near(m.X, 20.0f) || !Near(m.Y, 0.0f)) { std::printf("005ff190_TEST FAIL\n"); return 1; }
    long index = 0; V2 p = { 100.0f, 100.0f };
    Spread(1, index, p);                                   // single quest: unchanged
    if (index != 0 || !Near(p.X, 100.0f)) { std::printf("005ff190_TEST FAIL\n"); return 1; }
    Spread(3, index, p);                                   // first of three: +X
    if (index != 1 || !Near(p.X, 115.0f)) { std::printf("005ff190_TEST FAIL\n"); return 1; }
    V2 q = { 0.0f, 0.0f }; Spread(3, index, q);            // second: -Y
    if (index != 2 || !Near(q.Y, -15.0f)) { std::printf("005ff190_TEST FAIL\n"); return 1; }
    std::printf("005ff190_TEST PASS\n");
    return 0;
}
