// Behaviour test for CCreatureAction_ShowTrophy::Initialise (0x00845350): a live trophy learns its shower; the effect
// is spawned only when creature, sub def and "SHOW_TROPHY_BUILDUP" lookup all succeed; the emitter is attached to the
// right hand when the effect has a particle emitter; the base Initialise always runs.
#include <cstdio>

struct Thing { bool dead; bool hasEmitter; Thing* shower; };
static int g_baseInit, g_attached, g_spawned;
static Thing g_effect;

static void Initialise(Thing* trophy, Thing* creature, bool hasSubDef, bool knowsEffect, Thing** emitterOut)
{
    if (trophy && !trophy->dead)
        trophy->shower = creature;
    long effectType = 0;
    if (creature && hasSubDef && knowsEffect) {
        effectType = 5;
        Thing* effect = effectType ? &g_effect : 0;
        ++g_spawned;
        if (effect) {
            *emitterOut = effect;
            if (effect->hasEmitter)
                ++g_attached;
        }
    }
    ++g_baseInit;
}

int main()
{
    Thing trophy = { false, false, 0 }, hero = { false, false, 0 };
    Thing* emitter = 0;
    g_effect.hasEmitter = true;
    Initialise(&trophy, &hero, true, true, &emitter);
    int ok = trophy.shower == &hero && emitter == &g_effect && g_attached == 1 && g_baseInit == 1;
    trophy.shower = 0; trophy.dead = true; emitter = 0;
    Initialise(&trophy, &hero, true, false, &emitter);
    ok = ok && trophy.shower == 0 && emitter == 0 && g_spawned == 1 && g_baseInit == 2;
    Initialise(0, 0, true, true, &emitter);
    ok = ok && g_spawned == 1 && g_baseInit == 3;
    std::printf(ok ? "00845350_TEST PASS\n" : "00845350_TEST FAIL\n");
    return ok ? 0 : 1;
}