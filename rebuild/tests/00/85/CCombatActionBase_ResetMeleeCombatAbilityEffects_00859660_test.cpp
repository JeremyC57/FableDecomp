// Behaviour test for 0x00859660 CCombatActionBase::ResetMeleeCombatAbilityEffects(): a model of the row over stub haste / bullet
// time / environment calls; checks the early returns (already reset, no ability def, a negative knockdown-effects index), the
// haste multiplier dropped on the attacker and on a live target only, the bullet-time slowdown removed when the effects paused
// the entities, and the default-theme transition over the blend-out frames when an environment theme was set.
#include <cstdio>
struct Effects { long BlendOutFrames; long EnvironmentTheme; bool PauseEntities; };
struct World { int unsetAttacker, unsetTarget, removeSlowdown, transitions; long channels; float frames; };
static World w;
static void Reset(bool hasReset, bool hasDef, long index, const Effects* effects, bool targetAlive)
{
    if (hasReset || !hasDef || index < 0) return;
    if (effects->PauseEntities) {
        ++w.unsetAttacker;
        if (targetAlive) ++w.unsetTarget;
        ++w.removeSlowdown;
    }
    if (effects->EnvironmentTheme >= 0) { ++w.transitions; w.channels = 2; w.frames = (float)effects->BlendOutFrames; }
}
int main()
{
    int ok = 1;
    Effects e = { 12, 3, true };
    Reset(true, true, 1, &e, true); Reset(false, false, 1, &e, true); Reset(false, true, -1, &e, true);
    ok &= w.unsetAttacker == 0 && w.transitions == 0;
    Reset(false, true, 1, &e, true);
    ok &= w.unsetAttacker == 1 && w.unsetTarget == 1 && w.removeSlowdown == 1 && w.transitions == 1 && w.channels == 2 && w.frames == 12.0f;
    e.EnvironmentTheme = -1; Reset(false, true, 1, &e, false);
    ok &= w.unsetAttacker == 2 && w.unsetTarget == 1 && w.removeSlowdown == 2 && w.transitions == 1;
    e.PauseEntities = false; e.EnvironmentTheme = 0; Reset(false, true, 1, &e, true);
    ok &= w.unsetAttacker == 2 && w.removeSlowdown == 2 && w.transitions == 2;
    std::printf(ok ? "00859660_TEST PASS\n" : "00859660_TEST FAIL\n");
    return ok ? 0 : 1;
}