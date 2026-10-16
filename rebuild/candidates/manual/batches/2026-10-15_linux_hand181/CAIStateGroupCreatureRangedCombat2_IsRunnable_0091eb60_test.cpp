// behaviour test for CAIStateGroup_CreatureRangedCombat2::IsRunnable (0x0091EB60)
#include <cstdio>
struct CThing { bool carriesProjectile; bool hasTC8; CThing* attackTarget; bool best; };
struct Group {
    CThing* creature; CThing* PTargetThing; bool ableToFire;
    bool IsRunnable()
    {
        if (creature->carriesProjectile && creature->hasTC8) {
            CThing* target = creature->attackTarget;
            if (target && target->best) {
                PTargetThing = target;
                if (ableToFire) return true;
            }
        }
        return false;
    }
};
int main()
{
    CThing target = { false, false, 0, true };
    CThing c = { true, true, &target, false };
    Group g = { &c, 0, true };
    bool ok = g.IsRunnable() && g.PTargetThing == &target;
    g.ableToFire = false; g.PTargetThing = 0;
    ok = ok && !g.IsRunnable() && g.PTargetThing == &target;
    c.carriesProjectile = false; g.PTargetThing = 0;
    ok = ok && !g.IsRunnable() && g.PTargetThing == 0;
    if (!ok) { std::printf("0091eb60_TEST FAIL\n"); return 1; }
    std::printf("0091eb60_TEST PASS\n");
    return 0;
}
