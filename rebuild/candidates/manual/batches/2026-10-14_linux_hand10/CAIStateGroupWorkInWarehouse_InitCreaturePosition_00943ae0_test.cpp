#include <cstdio>
// Model of CAIStateGroup_WorkInWarehouse::InitCreaturePosition @ 0x00943AE0.
struct C3DVector { float X, Y, Z; };
static bool g_runnable, g_placed; static float g_radius; static const C3DVector* g_pos; static unsigned long g_seed = 11; static unsigned long* g_seedArg;
float GFFloatRandom(float range, unsigned long* seed) { g_seedArg = seed; return range * 0.5f; }
struct CThing { C3DVector pos; CThing* owner; const C3DVector& GetPosition() { return pos; } CThing* GetPWarehouse() { return owner; } };
struct Creature { bool AutoPlaceNear(const C3DVector& p, float r) { g_pos = &p; g_radius = r; return g_placed; } };
struct Group {
    Creature creature; CThing* crate;
    bool IsRunnable() { return g_runnable; }
    unsigned long* GetPRandomSeed() { return &g_seed; }
    bool InitCreaturePosition()
    {
        if (IsRunnable()) {
            CThing* warehouse = crate->GetPWarehouse();
            Creature* c = &creature;
            return c->AutoPlaceNear(warehouse->GetPosition(), GFFloatRandom(30.0f, GetPRandomSeed()));
        }
        return false;
    }
};
int main()
{
    CThing warehouse = { {1, 2, 3}, 0 }; CThing crate = { {9, 9, 9}, &warehouse };
    Group g; g.crate = &crate; g_runnable = false; g_placed = true; g_pos = 0;
    if (g.InitCreaturePosition() || g_pos != 0) { std::printf("00943ae0_TEST FAIL\n"); return 1; }
    g_runnable = true;
    if (!g.InitCreaturePosition() || g_pos != &warehouse.pos || g_radius != 15.0f || g_seedArg != &g_seed) { std::printf("00943ae0_TEST FAIL\n"); return 1; }
    g_placed = false; if (g.InitCreaturePosition()) { std::printf("00943ae0_TEST FAIL\n"); return 1; }
    std::printf("00943ae0_TEST PASS\n"); return 0;
}
