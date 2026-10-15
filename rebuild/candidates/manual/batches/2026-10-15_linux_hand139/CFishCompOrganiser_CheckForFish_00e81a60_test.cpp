// Behaviour test for 0x00E81A60 CFishCompOrganiser::CheckForFish (RodDef, done -> true).
#include <cstdio>
struct CCharString { int id; };
static int g_queries = 0;
struct Game {
    int carried;
    int GetHero() { return 77; }
    bool IsThingCarryingItem(const CCharString& def, int thing) { ++g_queries; return thing == 77 && def.id == carried; }
};
struct CFishCompOrganiser {
    Game* G; int FishingRodsInGame; CCharString* RodDef; bool done;
    bool IsDone() { return done; }
    bool CheckForFish()
    {
        bool found = false;
        for (int i = 0; i < FishingRodsInGame && !found; ++i) {
            if (G->IsThingCarryingItem(*(RodDef + i), G->GetHero())) {
                if (IsDone())
                    return true;
                found = true;
            }
        }
        return found;
    }
};
static int Fail() { std::printf("00e81a60_TEST FAIL\n"); return 1; }
int main()
{
    CCharString defs[3] = { {1}, {2}, {3} };
    Game g; g.carried = 2;
    CFishCompOrganiser o; o.G = &g; o.FishingRodsInGame = 3; o.RodDef = defs; o.done = false;
    if (!o.CheckForFish() || g_queries != 2) return Fail();
    g.carried = 9; g_queries = 0;
    if (o.CheckForFish() || g_queries != 3) return Fail();
    o.FishingRodsInGame = 0; g_queries = 0;
    if (o.CheckForFish() || g_queries != 0) return Fail();
    o.FishingRodsInGame = 3; g.carried = 1; o.done = true;
    if (o.CheckForFish() != true) return Fail();
    std::printf("00e81a60_TEST PASS\n");
    return 0;
}
