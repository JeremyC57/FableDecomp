// Behaviour test for CTCFireballSpell::FrameUpdate (007c9180): target adoption rules, the two finish-action forms, the timer reset and the noise
#include <cstdio>

struct Thing { bool dying, canHaveEnemies, enemy, valid; };
struct Spell {
    const Thing* target;
    const Thing* itemRemoved;
    bool oneAnim;
    long timerState;
    int actionsStarted, actionWithTarget, lookAtSet, noises;
};

static bool Alive(const Thing* t) { return t && !t->dying; }

static void FrameUpdate(Spell& s, const Thing* combatTarget, bool hasTargeting, bool autoTarget, bool actionAccepted)
{
    if (hasTargeting && Alive(combatTarget)) {
        if (!combatTarget->canHaveEnemies || combatTarget->enemy)
            s.target = combatTarget;
        else if (autoTarget)
            s.target = combatTarget;
    }
    if (!s.oneAnim) {
        bool withTarget = Alive(s.target) && s.target->valid;
        s.actionWithTarget = withTarget ? 1 : 0;
        s.lookAtSet = Alive(s.itemRemoved) ? 1 : 0;
        if (!actionAccepted) {
            s.timerState = 0;
            return;
        }
        ++s.actionsStarted;
    }
    ++s.noises;
}

int main()
{
    int fails = 0;
    Thing friendNpc = { false, true, false, true };
    Thing enemyNpc = { false, true, true, true };
    Thing barrel = { false, false, false, false };
    Thing dead = { true, false, false, true };
    Spell s = { 0, 0, false, 5, 0, 0, 0, 0 };
    FrameUpdate(s, &friendNpc, true, false, true);
    if (s.target != 0 || s.actionWithTarget != 0 || s.actionsStarted != 1 || s.noises != 1) ++fails;
    FrameUpdate(s, &friendNpc, true, true, true);
    if (s.target != &friendNpc || s.actionWithTarget != 1) ++fails;
    FrameUpdate(s, &enemyNpc, true, false, true);
    if (s.target != &enemyNpc) ++fails;
    FrameUpdate(s, &barrel, true, false, true);
    if (s.target != &barrel || s.actionWithTarget != 0) ++fails;
    FrameUpdate(s, &dead, true, true, true);
    if (s.target != &barrel) ++fails;
    s.itemRemoved = &enemyNpc;
    FrameUpdate(s, 0, false, false, false);
    if (s.timerState != 0 || s.lookAtSet != 1 || s.noises != 5) ++fails;
    s.oneAnim = true;
    s.timerState = 3;
    FrameUpdate(s, 0, false, false, false);
    if (s.timerState != 3 || s.noises != 6 || s.actionsStarted != 5) ++fails;
    if (fails) {
        printf("007c9180_TEST FAIL\n");
        return 1;
    }
    printf("007c9180_TEST PASS\n");
    return 0;
}