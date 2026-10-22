// Behaviour test for CCreatureAction_ShowTrophy::PerformAction (0x008455C0): nothing happens without a live trophy;
// otherwise a live emitter is killed, the held-aloft effect respawned when the lookup succeeds, and the trophy told.
#include <cstdio>

struct Thing { bool dead; int killed; int shownBy; };
static int g_spawned;

static void PerformAction(Thing* trophy, Thing*& emitter, Thing* creature, bool effectKnown, Thing* newEffect)
{
    if (!(trophy && !trophy->dead))
        return;
    if (emitter && !emitter->dead)
        ++emitter->killed;
    long effectType = 0;
    if (creature && effectKnown) {
        effectType = 3;
        ++g_spawned;
        if (newEffect)
            emitter = newEffect;
    }
    ++trophy->shownBy;
}

int main()
{
    Thing trophy = { false, 0, 0 }, oldEmitter = { false, 0, 0 }, fresh = { false, 0, 0 }, hero = { false, 0, 0 };
    Thing* emitter = &oldEmitter;
    PerformAction(&trophy, emitter, &hero, true, &fresh);
    int ok = oldEmitter.killed == 1 && emitter == &fresh && trophy.shownBy == 1 && g_spawned == 1;
    trophy.dead = true;
    PerformAction(&trophy, emitter, &hero, true, &oldEmitter);
    ok = ok && emitter == &fresh && fresh.killed == 0 && trophy.shownBy == 1;
    trophy.dead = false; fresh.dead = true;
    PerformAction(&trophy, emitter, &hero, false, 0);
    ok = ok && fresh.killed == 0 && g_spawned == 1 && trophy.shownBy == 2;
    std::printf(ok ? "008455c0_TEST PASS\n" : "008455c0_TEST FAIL\n");
    return ok ? 0 : 1;
}