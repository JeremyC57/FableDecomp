// Behaviour test for CWorld::SetKickPos (0x0049FFD0): the same selection over a host model.
#include <cstdio>

struct Vec { float X, Y, Z; };
struct Thing { Vec pos; bool regionEntrance, teleporter, heroStats, scanned; float facing; };
static Thing g_things[8];
static int g_count;
struct Kick { bool set; Vec pos; float angle; bool teleporter; } g_kick;

static bool InRange(const Thing& t, const Vec& c, float r) { float dx = t.pos.X - c.X, dy = t.pos.Y - c.Y; return dx * dx + dy * dy <= r * r; }
static float Dist2(const Thing& t, const Vec& c) { float dx = t.pos.X - c.X, dy = t.pos.Y - c.Y, dz = t.pos.Z - c.Z; return dx * dx + dy * dy + dz * dz; }

static void SetKickPos(Thing* hero)
{
    if (hero->heroStats) {
        Thing* best = 0;
        for (int i = 0; i < g_count; ++i) {
            Thing* t = &g_things[i];
            if (t->scanned && (t->regionEntrance || t->teleporter) && InRange(*t, hero->pos, 30.0f))
                best = !best ? t : (Dist2(*best, hero->pos) < Dist2(*t, hero->pos) ? best : t);
        }
        if (best) { g_kick.set = true; g_kick.pos = best->pos; g_kick.angle = best->facing; g_kick.teleporter = best->teleporter; }
    }
}

int main()
{
    int fails = 0;
    Thing hero = { { 0, 0, 0 }, false, false, true, false, 0 };
    Thing a = { { 10, 0, 0 }, true, false, false, true, 1.5f };
    Thing b = { { 5, 5, 0 }, false, true, false, true, 2.5f };
    Thing c = { { 1, 1, 0 }, false, false, false, true, 3.5f };        // neither component
    Thing d = { { 40, 0, 0 }, true, false, false, true, 4.5f };        // out of range
    g_things[0] = a; g_things[1] = b; g_things[2] = c; g_things[3] = d; g_count = 4;
    SetKickPos(&hero);
    if (!g_kick.set || g_kick.pos.X != 5.0f || g_kick.angle != 2.5f || !g_kick.teleporter) ++fails;
    g_things[1].scanned = false; g_kick.set = false;
    SetKickPos(&hero);
    if (!g_kick.set || g_kick.pos.X != 10.0f || g_kick.teleporter) ++fails;
    hero.heroStats = false; g_kick.set = false;
    SetKickPos(&hero);
    if (g_kick.set) ++fails;
    hero.heroStats = true; g_count = 0;
    SetKickPos(&hero);
    if (g_kick.set) ++fails;
    if (fails) { std::printf("0049ffd0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0049ffd0_TEST PASS\n");
    return 0;
}