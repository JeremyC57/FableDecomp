#include <cstdio>
#include <cstring>
struct Thing { int id; bool alive; };
struct Game {
    Thing lady; Thing hero; bool limbo; int limboSets; int teleports; int persist;
    Thing GetThingWithScriptName(const char* n) const { Thing t = { 0, false }; if (std::strcmp(n, "LadyGrey") == 0) t = lady; return t; }
    Thing& GetHero() { return hero; }
    bool IsEntityInLimbo(const Thing&) const { return limbo; }
    void EntitySetInLimbo(const Thing&, bool a, bool b) { if (a && !b) { limbo = true; ++limboSets; } }
    void EntityTeleportToPosition(const Thing& t, int pos) { if (t.id == lady.id && pos == hero.id) ++teleports; }
    void SetThingPersistent(const Thing&, bool p) { if (p) ++persist; }
};
struct Script {
    Game* GSI; Thing LadyGrey;
    void Init()
    {
        LadyGrey = GSI->GetThingWithScriptName("LadyGrey");
        if (LadyGrey.alive) {
            if (!GSI->IsEntityInLimbo(LadyGrey)) GSI->EntitySetInLimbo(LadyGrey, true, false);
            GSI->EntityTeleportToPosition(LadyGrey, GSI->GetHero().id);
            GSI->SetThingPersistent(LadyGrey, true);
        }
    }
};
int main()
{
    Game g = { { 5, true }, { 9, true }, false, 0, 0, 0 }; Script s = { &g, { 0, false } }; s.Init();
    Game h = { { 5, false }, { 9, true }, false, 0, 0, 0 }; Script t = { &h, { 0, false } }; t.Init();
    if (s.LadyGrey.id != 5 || g.limboSets != 1 || g.teleports != 1 || g.persist != 1 || h.limboSets != 0 || h.teleports != 0) { std::printf("00cf8970_TEST FAIL\n"); return 1; }
    std::printf("00cf8970_TEST PASS\n");
    return 0;
}