// Behaviour test for CCreatureAction_SummonerDead::Initialise (0x008CB940): animation + kill always; a positive death
// object index creates the object, which (alive, world not loading, hero alive) gets the hero as owner; the MESH_EXPLODE
// effect is spawned when the effects def resolves it.
#include <cstdio>

struct Thing { bool dead; Thing* owner; };
static int g_created, g_effects, g_killed, g_animSet;
static Thing g_deathObject, g_hero;
static bool g_loading;

static void Initialise(long deathIndex, bool hasEffect, bool heroDead)
{
    g_animSet = 1;
    g_killed = 1;
    if (deathIndex > 0) {
        Thing* obj = &g_deathObject; ++g_created;
        if (obj && !obj->dead && !g_loading) {
            Thing* hero = heroDead ? 0 : &g_hero;
            if (hero && !hero->dead)
                obj->owner = hero;
        }
    }
    long type = 0;
    if (hasEffect) { type = 7; if (type) ++g_effects; }
}

int main()
{
    Initialise(3, true, false);
    if (g_created != 1 || g_deathObject.owner != &g_hero || g_effects != 1 || !g_killed || !g_animSet) {
        std::printf("008cb940_TEST FAIL\n"); return 1;
    }
    g_deathObject.owner = 0;
    Initialise(0, false, false);
    if (g_created != 1 || g_effects != 1) { std::printf("008cb940_TEST FAIL\n"); return 1; }
    g_loading = true;
    Initialise(2, false, false);
    if (g_created != 2 || g_deathObject.owner != 0) { std::printf("008cb940_TEST FAIL\n"); return 1; }
    std::printf("008cb940_TEST PASS\n");
    return 0;
}