// behaviour test for 0x0093C2F0 CAIStateGroup_StealthSeekEnemy::IsRunnable(): no perception component -> false; a dead,
// null, non-enemy, unwilling, unseen-and-unaware or targeting-rejected thing -> false; otherwise PEnemy is latched and
// DetectedPos copies the perceived position; the targeting component missing -> false
#include <cstdio>

struct V3 { float X, Y, Z; };
struct Thing { bool dead; bool enemy; bool willing; bool validTarget; };
struct Perceive { Thing* perceived; bool extendedLineOfSight; bool aware; V3 lastPos; int awareCalls; };
struct Targeting { int calls; };
struct Creature { Perceive* perceive; Targeting* targeting; };

static bool AreThingsEnemies(const Creature&, const Thing& t) { return t.enemy; }
static bool IsWillingToAttackTarget(const Creature*, const Thing* t) { return t->willing; }

struct Group {
    Creature* creature;
    Thing* PEnemy;
    V3 DetectedPos;
    bool IsRunnable() {
        Perceive* perceive = creature->perceive;
        if (perceive) {
            Thing* target = perceive->perceived;
            if (target && !target->dead && AreThingsEnemies(*creature, *target) && IsWillingToAttackTarget(creature, target)
                    && (perceive->extendedLineOfSight || (++perceive->awareCalls, perceive->aware))) {
                Targeting* targeting = creature->targeting;
                if (targeting && (++targeting->calls, target->validTarget)) {
                    PEnemy = target;
                    DetectedPos = perceive->lastPos;
                    return true;
                }
            }
        }
        return false;
    }
};

int main() {
    int fails = 0;
    Thing t = { false, true, true, true };
    Perceive p = { &t, true, false, { 1, 2, 3 }, 0 };
    Targeting tg = { 0 };
    Creature c = { &p, &tg };
    Group g = { &c, 0, { 0, 0, 0 } };
    if (!g.IsRunnable() || g.PEnemy != &t || g.DetectedPos.Z != 3 || p.awareCalls) { puts("seen"); ++fails; }
    g.PEnemy = 0; p.extendedLineOfSight = false; p.aware = true;
    if (!g.IsRunnable() || p.awareCalls != 1) { puts("aware"); ++fails; }
    p.aware = false; g.PEnemy = 0;
    if (g.IsRunnable() || g.PEnemy) { puts("unaware"); ++fails; }
    p.aware = true; t.dead = true;
    if (g.IsRunnable()) { puts("dead"); ++fails; }
    t.dead = false; t.enemy = false;
    if (g.IsRunnable()) { puts("friend"); ++fails; }
    t.enemy = true; t.willing = false;
    if (g.IsRunnable()) { puts("unwilling"); ++fails; }
    t.willing = true; t.validTarget = false;
    if (g.IsRunnable() || tg.calls == 0) { puts("rejected"); ++fails; }
    t.validTarget = true; c.targeting = 0;
    if (g.IsRunnable()) { puts("notargeting"); ++fails; }
    c.targeting = &tg; p.perceived = 0;
    if (g.IsRunnable()) { puts("null"); ++fails; }
    c.perceive = 0;
    if (g.IsRunnable()) { puts("noperceive"); ++fails; }
    if (!fails) puts("0093c2f0_TEST PASS");
    return fails;
}