#include <cstdio>
// Model of InitCreaturePosition @ 0x009429c0: runnable -> AutoPlaceNearBuilding(building, 5.0f) -> 1/0.
struct CThingBuilding { int id; };
static bool g_runnable, g_placed; static CThingBuilding g_pub = { 3 }; static CThingBuilding* g_arg; static float g_radius;
struct CThingAICreature { bool AutoPlaceNearBuilding(CThingBuilding* b, float r) { g_arg = b; g_radius = r; return g_placed; } };
struct Group {
    CThingAICreature creature;
    bool IsRunnable() { return g_runnable; }
    CThingBuilding* GetPItem() { return &g_pub; }
    long InitCreaturePosition()
    {
        if (IsRunnable()) {
            CThingAICreature* c = &creature;
            if (c->AutoPlaceNearBuilding(GetPItem(), 5.0f)) return 1;
        }
        return 0;
    }
};
int main()
{
    Group g; g_runnable = false; g_placed = true; g_arg = 0;
    if (g.InitCreaturePosition() != 0 || g_arg != 0) { std::printf("009429c0_TEST FAIL\n"); return 1; }
    g_runnable = true;
    if (g.InitCreaturePosition() != 1 || g_arg != &g_pub || g_radius != 5.0f) { std::printf("009429c0_TEST FAIL\n"); return 1; }
    g_placed = false;
    if (g.InitCreaturePosition() != 0) { std::printf("009429c0_TEST FAIL\n"); return 1; }
    std::printf("009429c0_TEST PASS\n"); return 0;
}