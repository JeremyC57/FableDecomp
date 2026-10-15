// Behaviour test for 0x00E81AD0 CFishCompOrganiser::CheckForFish (FishDef, done -> false).
#include <cstdio>
struct CCharString { int id; };
static int g_queries = 0;
struct Game {
    int carried;
    int GetHero() { return 77; }
    bool IsThingCarryingItem(const CCharString& def, int thing) { ++g_queries; return thing == 77 && def.id == carried; }
};
struct CFishCompOrganiser {
    Game* G; int FishTypesInGame; CCharString* FishDef; bool done;
    bool IsDone() { return done; }
    bool CheckForFish()
    {
        bool found = false;
        for (int i = 0; i < FishTypesInGame && !found; ++i) {
            if (G->IsThingCarryingItem(*(FishDef + i), G->GetHero())) {
                if (IsDone())
                    return false;
                found = true;
            }
        }
        return found;
    }
};
static int Fail() { std::printf("00e81ad0_TEST FAIL\n"); return 1; }
int main()
{
    CCharString defs[3] = { {1}, {2}, {3} };
    Game g; g.carried = 2;
    CFishCompOrganiser o; o.G = &g; o.FishTypesInGame = 3; o.FishDef = defs; o.done = false;
    if (!o.CheckForFish() || g_queries != 2) return Fail();
    g.carried = 9; g_queries = 0;
    if (o.CheckForFish() || g_queries != 3) return Fail();
    o.FishTypesInGame = 0; g_queries = 0;
    if (o.CheckForFish() || g_queries != 0) return Fail();
    o.FishTypesInGame = 3; g.carried = 1; o.done = true;
    if (o.CheckForFish() != false) return Fail();
    std::printf("00e81ad0_TEST PASS\n");
    return 0;
}