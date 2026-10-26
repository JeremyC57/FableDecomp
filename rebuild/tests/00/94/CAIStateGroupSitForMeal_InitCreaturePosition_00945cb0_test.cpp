#include <cstdio>
// Model of CAIStateGroup_SitForMeal::InitCreaturePosition @ 0x00945CB0.
class C3DVector { public: float X, Y, Z; };
class CThing { public: C3DVector pos; const C3DVector& GetPosition() { return pos; } };
static bool g_runnable, g_placed; static CThing g_pot = { {1, 2, 3} }; static bool g_havePot; static const C3DVector* g_arg; static float g_radius;
struct Creature { bool AutoPlaceNear(const C3DVector& p, float r) { g_arg = &p; g_radius = r; return g_placed; } };
struct Group {
    Creature creature;
    bool IsRunnable() { return g_runnable; }
    CThing* GetPCookingPot() { return g_havePot ? &g_pot : 0; }
    bool PlaceCreatureNear(CThing* pot) { Creature* c = &creature; return c->AutoPlaceNear(pot->GetPosition(), 3.0f); }
    bool InitCreaturePosition()
    {
        if (!IsRunnable()) return false;
        CThing* pot = GetPCookingPot();
        return pot != 0 && PlaceCreatureNear(pot);
    }
};
int main()
{
    Group g; g_runnable = false; g_placed = true; g_havePot = true; g_arg = 0;
    if (g.InitCreaturePosition() || g_arg) { std::printf("00945cb0_TEST FAIL\n"); return 1; }
    g_runnable = true; g_havePot = false;
    if (g.InitCreaturePosition() || g_arg) { std::printf("00945cb0_TEST FAIL\n"); return 1; }
    g_havePot = true;
    if (!g.InitCreaturePosition() || g_arg != &g_pot.pos || g_radius != 3.0f) { std::printf("00945cb0_TEST FAIL\n"); return 1; }
    g_placed = false; if (g.InitCreaturePosition()) { std::printf("00945cb0_TEST FAIL\n"); return 1; }
    std::printf("00945cb0_TEST PASS\n"); return 0;
}