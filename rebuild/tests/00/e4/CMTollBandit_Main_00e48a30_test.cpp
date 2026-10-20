#include <cstdio>
#include <cstring>
struct Thing { bool dead; int pos; bool IsDead() const { return dead; } };
struct Game {
    bool banditDead; int created; int lastPos;
    Thing GetThingWithScriptName(const char* n) const { Thing t = { std::strcmp(n, "TollBandit") == 0 && banditDead, 0 }; return t; }
    Thing CreateCreature(const char* def, int pos, const char* name, bool b) { if (!std::strcmp(def, "CREATURE_TOLL_BANDIT") && !std::strcmp(name, "TollBandit") && !b) { ++created; lastPos = pos; } Thing t = { false, pos }; return t; }
};
struct Script {
    Game* G; Thing Me; bool done;
    bool IsDone() { return done; }
    void Main()
    {
        bool dead = G->GetThingWithScriptName("TollBandit").IsDead();
        if (dead && !IsDone()) G->CreateCreature("CREATURE_TOLL_BANDIT", Me.pos, "TollBandit", false);
    }
};
int main()
{
    Game g = { true, 0, 0 }; Script a = { &g, { false, 42 }, false }; a.Main();
    Game h = { false, 0, 0 }; Script b = { &h, { false, 42 }, false }; b.Main();
    Game k = { true, 0, 0 }; Script c = { &k, { false, 42 }, true }; c.Main();
    if (g.created != 1 || g.lastPos != 42 || h.created != 0 || k.created != 0) { std::printf("00e48a30_TEST FAIL\n"); return 1; }
    std::printf("00e48a30_TEST PASS\n");
    return 0;
}