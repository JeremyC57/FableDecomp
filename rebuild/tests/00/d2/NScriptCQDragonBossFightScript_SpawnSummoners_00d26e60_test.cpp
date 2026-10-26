// Behaviour test for 00d26e60 (CQ_DragonBossFightScript::SpawnSummoners): one effect + summoner per missing
// summoner, spawn points used round robin (wrapping to 0), def chosen by rand() parity.
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct Vec { float X, Y, Z; };
struct Point { Vec pos; const Vec& GetPos() const { return pos; } };
struct Game {
    int effects, creatures; float xs[16]; int defs01;
    void CreateEffect(const char* d, const Vec&, const char* bone, float, bool, bool) { if (!std::strcmp(d, "SUMMON_CREATURE_SUMMON_EVIL") && !std::strcmp(bone, "")) ++effects; }
    void CreateCreature(const char* d, const Vec& p, const char* s, bool) { if (!std::strcmp(s, "DBSummoner")) { xs[creatures++] = p.X; if (!std::strcmp(d, "CREATURE_SUMMONER_01")) ++defs01; } }
};
struct Script {
    Game* game; Point* points; unsigned int npoints; long next, target, num;
    bool IsActiveThreadTerminating() { return false; }
    void SpawnSummoners()
    {
        long count = target - num;
        for (long i = 0; i < count; ++i) {
            if (IsActiveThreadTerminating()) return;
            const char* def = std::rand() % 2 == 0 ? "CREATURE_SUMMONER_01" : "CREATURE_SUMMONER_02";
            game->CreateEffect("SUMMON_CREATURE_SUMMON_EVIL", points[next].GetPos(), "", 0.0f, false, false);
            game->CreateCreature(def, points[next].GetPos(), "DBSummoner", false);
            ++next;
            if ((unsigned long)next >= npoints) {
                if (IsActiveThreadTerminating()) return;
                next = 0;
            }
        }
        if (IsActiveThreadTerminating()) return;
    }
};
int main()
{
    Point pts[3] = { { { 1, 0, 0 } }, { { 2, 0, 0 } }, { { 3, 0, 0 } } };
    Game g; std::memset(&g, 0, sizeof g);
    Script s = { &g, pts, 3, 1, 6, 1 };
    std::srand(7);
    s.SpawnSummoners();
    int ok = g.effects == 5 && g.creatures == 5 && g.xs[0] == 2 && g.xs[1] == 3 && g.xs[2] == 1 && g.xs[4] == 3 && s.next == 0;
    if (g.defs01 < 0 || g.defs01 > 5) ok = 0;
    if (!ok) { std::printf("00d26e60_TEST FAIL\n"); return 1; }
    std::printf("00d26e60_TEST PASS\n");
    return 0;
}