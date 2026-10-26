// behaviour test for 0x00451CA2 CThingMultiArrowShotDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x30, +0x2C, +0x34, +0x38, +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x50),
// so a binary save / load round trip restores the def
#include <cstdio>
#include <cstring>
struct Context {
    unsigned char buf[1024]; unsigned int pos; int load; const void* order[64]; unsigned int count;
    Context(int l) : pos(0), load(l), count(0) { }
    template <class T> void Transfer(T& v)
    {
        order[count++] = &v;
        if (load) std::memcpy(&v, buf + pos, sizeof(T)); else std::memcpy(buf + pos, &v, sizeof(T));
        pos += (unsigned int)sizeof(T);
    }
};
struct Def {
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
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.AccelerationScalar);
    c.Transfer(d.MaxAngle);
    c.Transfer(d.MaxDistance);
    c.Transfer(d.TimeToReachTarget);
    c.Transfer(d.MaxAngleToTurnDegs);
    c.Transfer(d.MultiArrowWeaponTrail);
    c.Transfer(d.MultiArrowHitEffect);
    c.Transfer(d.MultiArrowTrailEffect);
    c.Transfer(d.MultiArrowSpeed);
    c.Transfer(d.MultiArrowDeflectionByMelee);
    c.Transfer(d.MultiArrowHitEffectPercentage);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    d.MultiArrowDeflectionByMelee = true;
    Context save(0); Persist(d, save);
    const void* want[11] = { &d.AccelerationScalar, &d.MaxAngle, &d.MaxDistance, &d.TimeToReachTarget, &d.MaxAngleToTurnDegs, &d.MultiArrowWeaponTrail, &d.MultiArrowHitEffect, &d.MultiArrowTrailEffect, &d.MultiArrowSpeed, &d.MultiArrowDeflectionByMelee, &d.MultiArrowHitEffectPercentage };
    if (save.count != 11u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 11u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.AccelerationScalar, 0, sizeof(e.AccelerationScalar));
    std::memset(&e.MaxAngle, 0, sizeof(e.MaxAngle));
    std::memset(&e.MaxDistance, 0, sizeof(e.MaxDistance));
    std::memset(&e.TimeToReachTarget, 0, sizeof(e.TimeToReachTarget));
    std::memset(&e.MaxAngleToTurnDegs, 0, sizeof(e.MaxAngleToTurnDegs));
    std::memset(&e.MultiArrowWeaponTrail, 0, sizeof(e.MultiArrowWeaponTrail));
    std::memset(&e.MultiArrowHitEffect, 0, sizeof(e.MultiArrowHitEffect));
    std::memset(&e.MultiArrowTrailEffect, 0, sizeof(e.MultiArrowTrailEffect));
    std::memset(&e.MultiArrowSpeed, 0, sizeof(e.MultiArrowSpeed));
    std::memset(&e.MultiArrowDeflectionByMelee, 0, sizeof(e.MultiArrowDeflectionByMelee));
    std::memset(&e.MultiArrowHitEffectPercentage, 0, sizeof(e.MultiArrowHitEffectPercentage));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("00451ca2_TEST FAIL %d\n", fail); return 1; }
    std::printf("00451ca2_TEST PASS\n");
    return 0;
}