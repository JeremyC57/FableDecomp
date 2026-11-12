// Behaviour test for NCameraModes::CTools::GetStandardThings (0x00877720): the list is cleared, then refilled with the things
// of type 3 and then of type 5 that overlap the sphere, both searched over the square (trunc(x) +- trunc(r), trunc(y) +- trunc(r)).
#include <cstdio>
#include <list>
struct V3 { float X, Y, Z; };
struct Thing { long Type; V3 Pos; float Radius; };
struct Box { long TLX, TLY, BRX, BRY; };
static Thing g_things[6];
static int g_searches; static Box g_boxes[2];
static bool Overlaps(const Thing& t, const V3& c, float r)
{
    float dx = t.Pos.X - c.X, dy = t.Pos.Y - c.Y, dz = t.Pos.Z - c.Z, rr = r + t.Radius;
    return dx * dx + dy * dy + dz * dz <= rr * rr;
}
static void Search(const Box& area, unsigned long type, const V3& c, float r, std::list<const Thing*>& out)
{
    g_boxes[g_searches++] = area;
    for (int i = 0; i < 6; ++i) {
        const Thing& t = g_things[i];
        if ((long)t.Pos.X < area.TLX || (long)t.Pos.X >= area.BRX || (long)t.Pos.Y < area.TLY || (long)t.Pos.Y >= area.BRY) continue;
        if (t.Type == (long)type && Overlaps(t, c, r)) out.push_back(&t);
    }
}
static void GetStandardThings(std::list<const Thing*>& things, const V3& origin, float radius)
{
    things.clear();
    long x = (long)origin.X, y = (long)origin.Y, r = (long)radius;
    Box area = { x - r, y - r, x + r, y + r };
    Search(area, 3, origin, radius, things);
    Search(area, 5, origin, radius, things);
}
int main()
{
    Thing init[6] = { {3, {10, 10, 0}, 0.5f}, {5, {11, 9, 0}, 0.5f}, {4, {10, 11, 0}, 0.5f}, {3, {40, 40, 0}, 0.5f}, {5, {10.5f, 10.5f, 0}, 0.5f}, {3, {12, 12, 0}, 0.5f} };
    for (int i = 0; i < 6; ++i) g_things[i] = init[i];
    std::list<const Thing*> out; out.push_back(&g_things[3]);        // stale entry: must be dropped by the clear
    V3 origin = { 10.7f, 10.2f, 0.0f };
    GetStandardThings(out, origin, 3.9f);
    const Thing* expect[5] = { &g_things[0], &g_things[5], &g_things[1], &g_things[4], 0 };   // type 3 hits first, then type 5
    bool ok = g_searches == 2 && out.size() == 4
           && g_boxes[0].TLX == 7 && g_boxes[0].TLY == 7 && g_boxes[0].BRX == 13 && g_boxes[0].BRY == 13
           && g_boxes[1].TLX == 7 && g_boxes[1].BRY == 13;
    int k = 0;
    for (std::list<const Thing*>::iterator it = out.begin(); ok && it != out.end(); ++it, ++k) ok = *it == expect[k];
    if (!ok) { std::printf("00877720_TEST FAIL\n"); return 1; }
    std::printf("00877720_TEST PASS\n");
    return 0;
}