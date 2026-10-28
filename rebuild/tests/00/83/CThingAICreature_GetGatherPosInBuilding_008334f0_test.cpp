// Behaviour test for 0x008334F0 CThingAICreature::GetGatherPosInBuilding: the GATHER_POSITION helper point wins; else
// (creature has navigation) the building position when navigable, else the nearest navigable position within the
// building's physics radius, else the first navigable cell of the 11x11 half-unit grid (x outer, y inner, -5..5).
#include <cstdio>
struct V { float x, y, z; };
static bool g_hasHelper, g_hasNav, g_nearestOk;
static V g_helper, g_nearest;
static float g_nearestRadius;
static int g_navCalls;
static bool (*g_navigable)(const V&);
static bool helper(V& out) { if (g_hasHelper) out = g_helper; return g_hasHelper; }
static bool nearest(const V&, float radius, V& out) { g_nearestRadius = radius; if (g_nearestOk) out = g_nearest; return g_nearestOk; }
static bool navigable(const V& p) { ++g_navCalls; return g_navigable(p); }
static V GetGatherPosInBuilding(const V& building, float radius)
{
    V pos = { 0.0f, 0.0f, 0.0f };
    bool found = helper(pos);
    if (!found && g_hasNav) {
        if (navigable(building)) {
            pos = building;
        } else {
            found = nearest(building, radius, pos);
            if (!found) {
                for (float x = -5.0f; !found && x <= 5.0f; x += 1.0f)
                    for (float y = -5.0f; !found && y <= 5.0f; y += 1.0f) {
                        pos.x = x * 0.5f + building.x;
                        pos.y = y * 0.5f + building.y;
                        pos.z = building.z;
                        found = navigable(pos);
                    }
            }
        }
    }
    return pos;
}
static bool never(const V&) { return false; }
static bool always(const V&) { return true; }
static bool cellOnly(const V& p) { return p.x == 11.0f && p.y == 18.0f; }   // building (10,20): x=+2, y=-4
int main()
{
    int bad = 0;
    V b = { 10.0f, 20.0f, 3.0f };
    g_hasHelper = true; g_helper.x = 1; g_helper.y = 2; g_helper.z = 3; g_hasNav = true; g_navigable = always;
    V r = GetGatherPosInBuilding(b, 4.0f);
    if (r.x != 1 || r.y != 2 || r.z != 3 || g_navCalls != 0) bad = 1;
    g_hasHelper = false; g_hasNav = false;
    r = GetGatherPosInBuilding(b, 4.0f);
    if (r.x != 0 || r.y != 0 || r.z != 0) bad = 1;
    g_hasNav = true;
    r = GetGatherPosInBuilding(b, 4.0f);
    if (r.x != 10 || r.y != 20 || r.z != 3) bad = 1;
    g_navigable = never; g_nearestOk = true; g_nearest.x = 7; g_nearest.y = 8; g_nearest.z = 9;
    r = GetGatherPosInBuilding(b, 4.5f);
    if (r.x != 7 || r.y != 8 || g_nearestRadius != 4.5f) bad = 1;
    g_nearestOk = false; g_navigable = cellOnly; g_navCalls = 0;
    r = GetGatherPosInBuilding(b, 4.0f);
    if (r.x != 11.0f || r.y != 18.0f || r.z != 3.0f) bad = 1;
    if (g_navCalls != 1 + 7 * 11 + 2) bad = 1;           // the building test, x=-5..+1 full columns, then y=-5..-4
    g_navigable = never; g_navCalls = 0;
    r = GetGatherPosInBuilding(b, 4.0f);
    if (g_navCalls != 1 + 121 || r.x != 12.5f || r.y != 22.5f) bad = 1;   // last cell tried stays in pos
    if (bad) { std::printf("008334f0_TEST FAIL\n"); return 1; }
    std::printf("008334f0_TEST PASS\n");
    return 0;
}