// Behaviour test for CAIStateGroup_Thunder::IsRunnable (0x0096C2E0): no creature target -> false; a fresh hit latches the arena centre and counts, reaching the def's threshold latches the target; otherwise the 0xAD value above the def limit latches the target; the def reference is always released.
#include <cstdio>
struct CThing { unsigned char ThingClass; };
struct CDef { long RefCount; long HitsBeforeTarget; long EnvironmentThreshold; };
struct CThunder {
    CThing* target; bool hitEvent; long envValue; CDef* def; CThing* arena;
    CThing* PArenaCentre; CThing* PTargetThing; long HitCount;
    bool IsRunnable()
    {
        if (target == 0 || target->ThingClass != 2) return false;
        CDef* d = def; ++d->RefCount;
        if (hitEvent) {
            PArenaCentre = arena;
            if (++HitCount == d->HitsBeforeTarget) { PTargetThing = target; --d->RefCount; return true; }
        }
        if (envValue > d->EnvironmentThreshold) { PTargetThing = target; --d->RefCount; return true; }
        --d->RefCount;
        return false;
    }
};
int main()
{
    CThing creature = { 2 }, object = { 5 }, arena = { 5 };
    CDef def = { 1, 2, 10 };
    CThunder t = { 0, false, 0, &def, &arena, 0, 0, 0 };
    bool ok = !t.IsRunnable();
    t.target = &object; ok = ok && !t.IsRunnable();
    t.target = &creature; t.hitEvent = true;
    ok = ok && !t.IsRunnable() && t.HitCount == 1 && t.PArenaCentre == &arena && t.PTargetThing == 0;
    ok = ok && t.IsRunnable() && t.HitCount == 2 && t.PTargetThing == &creature;
    t.PTargetThing = 0; t.hitEvent = false; t.envValue = 11;
    ok = ok && t.IsRunnable() && t.PTargetThing == &creature && def.RefCount == 1;
    t.envValue = 10; ok = ok && !t.IsRunnable() && def.RefCount == 1;
    if (!ok) { std::printf("0096c2e0_TEST FAIL\n"); return 1; }
    std::printf("0096c2e0_TEST PASS\n");
    return 0;
}