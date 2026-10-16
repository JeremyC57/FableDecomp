// behaviour test for CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets::IsRunnable (0x0095E260)
#include <cstdio>
struct CThing { bool carriesProjectile; bool hasTC8; CThing* specificTarget; };
struct Group {
    CThing* creature; CThing* PTargetThing;
    bool IsRunnable()
    {
        if (creature->carriesProjectile && creature->hasTC8) {
            if (creature->specificTarget) { PTargetThing = creature->specificTarget; return true; }
        }
        return false;
    }
};
int main()
{
    CThing t = { false, false, 0 };
    CThing c = { true, true, &t };
    Group g = { &c, 0 };
    bool ok = g.IsRunnable() && g.PTargetThing == &t;
    c.specificTarget = 0; g.PTargetThing = 0;
    ok = ok && !g.IsRunnable() && g.PTargetThing == 0;
    c.specificTarget = &t; c.hasTC8 = false;
    ok = ok && !g.IsRunnable();
    if (!ok) { std::printf("0095e260_TEST FAIL\n"); return 1; }
    std::printf("0095e260_TEST PASS\n");
    return 0;
}
