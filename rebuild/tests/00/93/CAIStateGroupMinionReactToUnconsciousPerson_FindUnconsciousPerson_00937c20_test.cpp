// 00937c20 behaviour test: CAIStateGroup_MinionReactToUnconsciousPerson::FindUnconsciousPerson (final.cpp body, plain world model)
#include <stdio.h>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
struct C3DVector { float x, y, z; };
struct C2DBoxI { long x0, y0, x1, y1; C2DBoxI(long a, long b, long c, long d) : x0(a), y0(b), x1(c), y1(d) { } };
struct CThing { long long UID; C3DVector pos; bool unconscious, ally, aware, discovered; };
static long g_frame = 0;
static long g_framesPerSecond = 30;
static std::vector<CThing*> g_things;
static bool InBox(const CThing* t, const C2DBoxI& b) { return t->pos.x >= b.x0 && t->pos.x <= b.x1 && t->pos.y >= b.y0 && t->pos.y <= b.y1; }
static bool Passes(const CThing* t) { return t->aware && t->unconscious && t->ally; }
static bool GetPBestThingInRoughArea(const C2DBoxI& box, const C3DVector& from, CThing** out)
{
    CThing* best = 0; float bestDistance = 0.0f;
    for (size_t i = 0; i < g_things.size(); ++i) {
        CThing* t = g_things[i];
        if (!InBox(t, box) || !Passes(t)) continue;
        float dx = t->pos.x - from.x, dy = t->pos.y - from.y, dz = t->pos.z - from.z;
        float d = dx * dx + dy * dy + dz * dz;
        if (!best || d < bestDistance) { best = t; bestDistance = d; }
    }
    *out = best;
    return best != 0;
}
static CThing* Find(CThing* creature)
{
    long long uid = creature->UID;
    long framesPerSecond = g_framesPerSecond;
    CThing* found = 0;
    if ((g_frame + uid) % framesPerSecond == 0) {
        const C3DVector& pos = creature->pos;
        long x = (long)pos.x;
        long y = (long)pos.y;
        C2DBoxI box(x - 7, y - 7, x + 7, y + 7);
        GetPBestThingInRoughArea(box, creature->pos, &found);
    }
    return found;
}
int main()
{
    CThing me = { 5, { 10.5f, 20.5f, 0 }, false, true, false, false };
    CThing near1 = { 1, { 12, 21, 0 }, true, true, true, false };
    CThing near2 = { 2, { 11, 20, 0 }, true, true, true, true };
    CThing far1 = { 3, { 30, 20, 0 }, true, true, true, false };
    CThing awake = { 4, { 10, 20, 0 }, false, true, true, false };
    g_things.push_back(&near1); g_things.push_back(&near2); g_things.push_back(&far1); g_things.push_back(&awake);
    g_frame = 25;   // (25 + 5) % 30 == 0: this creature's frame
    CHECK(Find(&me) == &near2);
    g_frame = 26;   // staggered out
    CHECK(Find(&me) == 0);
    g_frame = 55; near1.ally = false; near2.aware = false;
    CHECK(Find(&me) == 0);
    if (!g_fail) printf("00937c20_TEST PASS\n");
    return g_fail;
}