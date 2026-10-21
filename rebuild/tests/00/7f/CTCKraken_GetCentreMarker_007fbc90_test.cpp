// Behaviour test for CTCKraken::GetCentreMarker (0x007FBC90): unless loading, look in the box [x-5, x+5] x [y-5, y+5]
// (truncated position) for the chamber building and the chamber-centre marker, and target the hero.
#include <cstdio>

struct Thing { long defIndex; const char* track; long x, y; };
struct Box { long minX, minY, maxX, maxY; };
static Thing g_things[3];
static Thing* FindFirst(const Box& b, long defIndex, const char* track)
{
    for (int i = 0; i < 3; ++i) {
        Thing& t = g_things[i];
        if (t.x < b.minX || t.x > b.maxX || t.y < b.minY || t.y > b.maxY) continue;
        if (track ? (t.track == track) : (t.defIndex == defIndex)) return &t;
    }
    return 0;
}
struct Kraken { float px, py; Thing* chamber; Thing* centre; Thing* target; };
static const char* const kCentre = "KRACKEN_CHAMBER_CENTRE";
static void GetCentreMarker(Kraken& k, bool loading, long chamberIndex, Thing* hero)
{
    if (loading) return;
    long x = (long)k.px, y = (long)k.py;
    Box b = { x - 5, y - 5, x + 5, y + 5 };
    k.chamber = FindFirst(b, chamberIndex, 0);
    k.centre = FindFirst(b, 0, kCentre);
    k.target = hero;
}
int main()
{
    g_things[0].defIndex = 42; g_things[0].track = 0; g_things[0].x = 13; g_things[0].y = 2;
    g_things[1].defIndex = 0; g_things[1].track = kCentre; g_things[1].x = 9; g_things[1].y = -2;
    g_things[2].defIndex = 42; g_things[2].track = 0; g_things[2].x = 30; g_things[2].y = 0;
    Thing hero; Kraken k; k.px = 8.9f; k.py = 0.5f; k.chamber = 0; k.centre = 0; k.target = 0;
    GetCentreMarker(k, true, 42, &hero);
    if (k.target) { std::printf("007fbc90_TEST FAIL\n"); return 1; }
    GetCentreMarker(k, false, 42, &hero);
    if (k.chamber != &g_things[0] || k.centre != &g_things[1] || k.target != &hero) { std::printf("007fbc90_TEST FAIL\n"); return 1; }
    k.px = 20.0f; GetCentreMarker(k, false, 42, &hero);
    if (k.chamber != 0 || k.centre != 0) { std::printf("007fbc90_TEST FAIL\n"); return 1; }
    std::printf("007fbc90_TEST PASS\n");
    return 0;
}