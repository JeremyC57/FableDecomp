// Behaviour test for 00d26cc0 (CQ_DragonBossFightScript::SpawnMinions): one effect + one war-dog per missing
// minion, at spawn point i; nothing when the count is already met; a terminating thread stops early.
#include <cstdio>
#include <cstring>

struct Vec { float X, Y, Z; };
struct Point { Vec pos; const Vec& GetPos() const { return pos; } };
struct Game {
    int effects, creatures; float lastX; int termAfter;
    void CreateEffect(const char* d, const Vec& p, const char* bone, float, bool, bool) { if (!std::strcmp(d, "SUMMON_CREATURE_SUMMON_EVIL") && !std::strcmp(bone, "")) { ++effects; lastX = p.X; } }
    void CreateCreature(const char* d, const Vec& p, const char* s, bool) { if (!std::strcmp(d, "CREATURE_MINION_WARDOG") && !std::strcmp(s, "DBMinion") && p.X == lastX) ++creatures; }
};
struct Script {
    Game* game; Point* points; long target, num;
    bool IsActiveThreadTerminating() { return game->creatures >= game->termAfter; }
    void SpawnMinions()
    {
        long count = target - num;
        for (long i = 0; i < count; ++i) {
            if (IsActiveThreadTerminating()) return;
            game->CreateEffect("SUMMON_CREATURE_SUMMON_EVIL", points[i].GetPos(), "", 0.0f, false, false);
            game->CreateCreature("CREATURE_MINION_WARDOG", points[i].GetPos(), "DBMinion", false);
        }
        if (IsActiveThreadTerminating()) return;
    }
};
int main()
{
    Point pts[4] = { { { 1, 0, 0 } }, { { 2, 0, 0 } }, { { 3, 0, 0 } }, { { 4, 0, 0 } } };
    int ok = 1;
    Game g = { 0, 0, 0, 100 }; Script s = { &g, pts, 5, 2 }; s.SpawnMinions();
    if (g.effects != 3 || g.creatures != 3 || g.lastX != 3) ok = 0;
    Game h = { 0, 0, 0, 100 }; Script t = { &h, pts, 2, 2 }; t.SpawnMinions();
    if (h.effects != 0) ok = 0;
    Game k = { 0, 0, 0, 2 }; Script u = { &k, pts, 4, 0 }; u.SpawnMinions();
    if (k.creatures != 2) ok = 0;
    if (!ok) { std::printf("00d26cc0_TEST FAIL\n"); return 1; }
    std::printf("00d26cc0_TEST PASS\n");
    return 0;
}
