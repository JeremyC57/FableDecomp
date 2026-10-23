// 00964ca0 behaviour test: CAIStateGroup_StealStealableItem::FindPEscapePoint (final.cpp body, plain world model)
#include <stdio.h>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
typedef unsigned int u32;
struct C3DVector { float x, y, z; };
struct CThing { long type; C3DVector pos; };
static std::vector<CThing*> g_things;
static u32 g_seed = 7;
static u32 GFROR13(u32 v) { return (v >> 13) | (v << 19); }
static bool GetPBestThing(long type, const C3DVector& from, CThing** out)
{
    CThing* best = 0; float bestDistance = 0.0f;
    for (size_t i = 0; i < g_things.size(); ++i) {
        CThing* t = g_things[i];
        if (t->type != type) continue;
        float dx = t->pos.x - from.x, dy = t->pos.y - from.y, dz = t->pos.z - from.z;
        float d = dx * dx + dy * dy + dz * dz;
        if (!best || d < bestDistance) { best = t; bestDistance = d; }
    }
    *out = best;
    return best != 0;
}
static void GetAllLeafThings(long type, std::vector<CThing*>& out)
{
    for (size_t i = 0; i < g_things.size(); ++i) if (g_things[i]->type == type) out.push_back(g_things[i]);
}
static CThing* FindPEscapePoint(CThing* creature, bool nearest)
{
    CThing* result = 0;
    if (nearest) {
        GetPBestThing(1, creature->pos, &result);
        return result;
    } else {
        std::vector<CThing*> points;
        GetAllLeafThings(1, points);
        if (!points.empty()) {
            u32 count = (u32)points.size();
            u32 next = g_seed * 0x24A1U + 0x24DFU;
            g_seed = next;
            u32 rotated = GFROR13(next);
            g_seed = rotated;
            u32 index = count ? rotated % count : 0;
            result = points[index];
        }
        return result;
    }
}
int main()
{
    CThing me = { 0, { 0, 0, 0 } };
    CHECK(FindPEscapePoint(&me, true) == 0);
    CHECK(FindPEscapePoint(&me, false) == 0 && g_seed == 7);
    CThing a = { 1, { 10, 0, 0 } }, b = { 1, { 0, 2, 0 } }, c = { 2, { 0, 0, 1 } };
    g_things.push_back(&a); g_things.push_back(&b); g_things.push_back(&c);
    CHECK(FindPEscapePoint(&me, true) == &b);
    u32 next = 7u * 0x24A1U + 0x24DFU, rot = GFROR13(next);
    CThing* pick = FindPEscapePoint(&me, false);
    CHECK(g_seed == rot && pick == (rot % 2 ? &b : &a));
    if (!g_fail) printf("00964ca0_TEST PASS\n");
    return g_fail;
}