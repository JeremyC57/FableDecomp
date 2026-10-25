// Behaviour test for 0x00864D00 NCreatureAI::IsToAttackTarget(CThingAICreature*, const CThingCreatureBase*)
// The model mirrors the source: alive tests, best-target test, the enemy / faction-override re-pick (UpdatePBest...
// then best-target again), then the targeting component's script override or awareness of the target.
#include <cstdio>

struct Thing { bool dead; };
struct Targeting { Thing* best; Thing* rePicked; Thing* scriptOverride; int updates; };
struct Creature : Thing {
    Targeting* targeting;            // 0 when the creature has no CTCTargetingAI
    bool enemies, overriding, aware;
};

static int g_awareCalls = 0;
static bool IsBestTargetForAttack(const Creature& c, const Thing* t)
{
    return t && !t->dead && c.targeting && c.targeting->best == t;
}
static bool IsToAttackTarget(Creature* creature, const Thing* target)
{
    if (creature == 0 || creature->dead) return false;
    if (target == 0 || target->dead) return false;
    if (IsBestTargetForAttack(*creature, target)) {
        if (!creature->enemies && !creature->overriding) {
            if (creature->targeting) { ++creature->targeting->updates; creature->targeting->best = creature->targeting->rePicked; }
            if (!IsBestTargetForAttack(*creature, target)) return false;
        }
        if (creature->targeting) {
            if (creature->targeting->scriptOverride != 0) return true;
            ++g_awareCalls;
            if (creature->aware) return true;
        }
    }
    return false;
}

int main()
{
    int fail = 0;
    Thing enemy = { false }, other = { false }, deadOne = { true };
    Targeting tai = { &enemy, &other, 0, 0 };
    Creature c; c.dead = false; c.targeting = &tai; c.enemies = true; c.overriding = false; c.aware = true;
    if (!IsToAttackTarget(&c, &enemy)) fail |= 1;                     // enemies, aware
    c.aware = false;
    if (IsToAttackTarget(&c, &enemy)) fail |= 2;                      // not aware, no override
    tai.scriptOverride = &other;
    int aw = g_awareCalls;
    if (!IsToAttackTarget(&c, &enemy) || g_awareCalls != aw) fail |= 4;   // override short-circuits awareness
    if (IsToAttackTarget(&c, &deadOne) || IsToAttackTarget(0, &enemy) || IsToAttackTarget(&c, 0)) fail |= 8;
    c.dead = true; if (IsToAttackTarget(&c, &enemy)) fail |= 16; c.dead = false;
    if (IsToAttackTarget(&c, &other)) fail |= 32;                     // not the best target
    c.enemies = false; c.overriding = false;                          // no longer enemies: re-pick moves best to `other`
    if (IsToAttackTarget(&c, &enemy) || tai.updates != 1) fail |= 64;
    tai.best = &enemy; tai.rePicked = &enemy; c.overriding = true;    // faction override keeps the target, no re-pick
    if (!IsToAttackTarget(&c, &enemy) || tai.updates != 1) fail |= 128;
    c.overriding = false;                                             // re-pick picks the same target again
    if (!IsToAttackTarget(&c, &enemy) || tai.updates != 2) fail |= 256;
    if (fail) { std::printf("00864d00_TEST FAIL %d\n", fail); return 1; }
    std::printf("00864d00_TEST PASS\n");
    return 0;
}