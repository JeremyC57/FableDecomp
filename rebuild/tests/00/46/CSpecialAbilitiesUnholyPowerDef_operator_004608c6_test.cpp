// Behaviour test for 0x004608C6 CSpecialAbilitiesUnholyPowerDef::operator=: base part, every vector (through its own
// assignment) and every scalar are copied, in declaration order, and *this is returned.
#include <cstdio>

static int g_order[32]; static int g_n;
class CParentDefClassBase { public: int base; CParentDefClassBase& operator=(const CParentDefClassBase& o) { base = o.base; g_order[g_n++] = 0; return *this; } };
struct CVec { int id; int value; CVec& operator=(const CVec& o) { value = o.value; g_order[g_n++] = id; return *this; } };
struct CDef : CParentDefClassBase {
    CVec StartPowerupSound, LoopingPowerupSound, ReleaseSound, ReleaseLoopSound, FinishSound, CancelSound;
    float RandomFactorForNumSecsBetweenDamageHit;
    CVec NumSecsBetweenDamageHit, DamagePerHit, DurationOfSpellSecs, RadiusOfEffect, EnvChangeSecs, EnvReturnSecs;
    bool DoLifting; float VelocityOfLift, DistanceToLift;
    CVec StaminaCostPerSec, DamageTakenMultiplier;
    float SmallRingRadius, LargeRingRadius;
    CDef& operator=(const CDef& other)
    {
        CParentDefClassBase::operator=(other);
        StartPowerupSound = other.StartPowerupSound; LoopingPowerupSound = other.LoopingPowerupSound;
        ReleaseSound = other.ReleaseSound; ReleaseLoopSound = other.ReleaseLoopSound;
        FinishSound = other.FinishSound; CancelSound = other.CancelSound;
        RandomFactorForNumSecsBetweenDamageHit = other.RandomFactorForNumSecsBetweenDamageHit;
        NumSecsBetweenDamageHit = other.NumSecsBetweenDamageHit; DamagePerHit = other.DamagePerHit;
        DurationOfSpellSecs = other.DurationOfSpellSecs; RadiusOfEffect = other.RadiusOfEffect;
        EnvChangeSecs = other.EnvChangeSecs; EnvReturnSecs = other.EnvReturnSecs;
        DoLifting = other.DoLifting; VelocityOfLift = other.VelocityOfLift; DistanceToLift = other.DistanceToLift;
        StaminaCostPerSec = other.StaminaCostPerSec; DamageTakenMultiplier = other.DamageTakenMultiplier;
        SmallRingRadius = other.SmallRingRadius; LargeRingRadius = other.LargeRingRadius;
        return *this;
    }
};
static void Ids(CDef& d)
{
    CVec* v[14] = { &d.StartPowerupSound, &d.LoopingPowerupSound, &d.ReleaseSound, &d.ReleaseLoopSound, &d.FinishSound, &d.CancelSound,
        &d.NumSecsBetweenDamageHit, &d.DamagePerHit, &d.DurationOfSpellSecs, &d.RadiusOfEffect, &d.EnvChangeSecs, &d.EnvReturnSecs,
        &d.StaminaCostPerSec, &d.DamageTakenMultiplier };
    for (int i = 0; i < 14; ++i) { v[i]->id = i + 1; v[i]->value = 0; }
}
int main()
{
    CDef a, b; Ids(a); Ids(b);
    b.base = 5; b.StartPowerupSound.value = 11; b.DamageTakenMultiplier.value = 22; b.RandomFactorForNumSecsBetweenDamageHit = 0.5f;
    b.DoLifting = true; b.VelocityOfLift = 2.0f; b.DistanceToLift = 3.0f; b.SmallRingRadius = 4.0f; b.LargeRingRadius = 5.0f;
    CDef* r = &(a = b);
    if (r != &a || a.base != 5 || a.StartPowerupSound.value != 11 || a.DamageTakenMultiplier.value != 22) { std::printf("004608c6_TEST FAIL\n"); return 1; }
    if (a.RandomFactorForNumSecsBetweenDamageHit != 0.5f || !a.DoLifting || a.VelocityOfLift != 2.0f || a.DistanceToLift != 3.0f || a.SmallRingRadius != 4.0f || a.LargeRingRadius != 5.0f) { std::printf("004608c6_TEST FAIL\n"); return 1; }
    if (g_n != 15) { std::printf("004608c6_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 15; ++i) if (g_order[i] != i) { std::printf("004608c6_TEST FAIL\n"); return 1; }
    std::printf("004608c6_TEST PASS\n");
    return 0;
}