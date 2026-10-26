// behaviour test for 0x004536AE CThingMultiArrowShotDef::operator=: the base part is assigned first, every member is copied, and the result is *this
#include <cstdio>
static int g_base = 0;
struct Base { long PDefManager; long GlobalIndex; Base& operator=(const Base& o) { ++g_base; PDefManager = o.PDefManager; GlobalIndex = o.GlobalIndex; return *this; } };
struct Def : Base {
    float AccelerationScalar;
    float MaxDistance;
    float MaxAngle;
    float TimeToReachTarget;
    float MaxAngleToTurnDegs;
    long MultiArrowWeaponTrail;
    long MultiArrowHitEffect;
    long MultiArrowTrailEffect;
    float MultiArrowSpeed;
    bool MultiArrowDeflectionByMelee;
    float MultiArrowHitEffectPercentage;
    Def& Assign(const Def& o)                              // the function under test
    {
        Base::operator=(o);
        AccelerationScalar = o.AccelerationScalar;
        MaxDistance = o.MaxDistance;
        MaxAngle = o.MaxAngle;
        TimeToReachTarget = o.TimeToReachTarget;
        MaxAngleToTurnDegs = o.MaxAngleToTurnDegs;
        MultiArrowWeaponTrail = o.MultiArrowWeaponTrail;
        MultiArrowHitEffect = o.MultiArrowHitEffect;
        MultiArrowTrailEffect = o.MultiArrowTrailEffect;
        MultiArrowSpeed = o.MultiArrowSpeed;
        MultiArrowDeflectionByMelee = o.MultiArrowDeflectionByMelee;
        MultiArrowHitEffectPercentage = o.MultiArrowHitEffectPercentage;
        return *this;
    }
};
int main()
{
    int fail = 0;
    Def s, d;
    s.PDefManager = 11; s.GlobalIndex = 12; d.PDefManager = 0; d.GlobalIndex = 0;
    s.AccelerationScalar = 1.25f; d.AccelerationScalar = 0.0f;
    s.MaxDistance = 2.25f; d.MaxDistance = 0.0f;
    s.MaxAngle = 3.25f; d.MaxAngle = 0.0f;
    s.TimeToReachTarget = 4.25f; d.TimeToReachTarget = 0.0f;
    s.MaxAngleToTurnDegs = 5.25f; d.MaxAngleToTurnDegs = 0.0f;
    s.MultiArrowWeaponTrail = 105L; d.MultiArrowWeaponTrail = 0L;
    s.MultiArrowHitEffect = 106L; d.MultiArrowHitEffect = 0L;
    s.MultiArrowTrailEffect = 107L; d.MultiArrowTrailEffect = 0L;
    s.MultiArrowSpeed = 9.25f; d.MultiArrowSpeed = 0.0f;
    s.MultiArrowDeflectionByMelee = true; d.MultiArrowDeflectionByMelee = false;
    s.MultiArrowHitEffectPercentage = 11.25f; d.MultiArrowHitEffectPercentage = 0.0f;
    g_base = 0;
    if (&d.Assign(s) != &d) fail |= 1;
    if (g_base != 1 || d.PDefManager != 11 || d.GlobalIndex != 12) fail |= 2;
    if (d.AccelerationScalar != s.AccelerationScalar) fail |= 4;
    if (d.MaxDistance != s.MaxDistance) fail |= 4;
    if (d.MaxAngle != s.MaxAngle) fail |= 4;
    if (d.TimeToReachTarget != s.TimeToReachTarget) fail |= 4;
    if (d.MaxAngleToTurnDegs != s.MaxAngleToTurnDegs) fail |= 4;
    if (d.MultiArrowWeaponTrail != s.MultiArrowWeaponTrail) fail |= 4;
    if (d.MultiArrowHitEffect != s.MultiArrowHitEffect) fail |= 4;
    if (d.MultiArrowTrailEffect != s.MultiArrowTrailEffect) fail |= 4;
    if (d.MultiArrowSpeed != s.MultiArrowSpeed) fail |= 4;
    if (d.MultiArrowDeflectionByMelee != s.MultiArrowDeflectionByMelee) fail |= 4;
    if (d.MultiArrowHitEffectPercentage != s.MultiArrowHitEffectPercentage) fail |= 4;
    if (fail) { std::printf("004536ae_TEST FAIL %d\n", fail); return 1; }
    std::printf("004536ae_TEST PASS\n");
    return 0;
}