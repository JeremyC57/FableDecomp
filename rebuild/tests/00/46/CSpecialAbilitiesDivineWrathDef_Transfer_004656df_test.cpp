// behaviour test for 0x004656DF CSpecialAbilitiesDivineWrathDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x3C, +0x48, +0x54, +0x60, +0x6C, +0x78, +0x84, +0x88, +0x94, +0xA0, +0xAC, +0xB8, +0xC4, +0xD0, +0xD4, +0xD8, +0xDC, +0xE8, +0xF4, +0xF8),
// so a binary save / load round trip restores the def
#include <cstdio>
#include <cstring>
struct Blob12 { unsigned char b[12]; };
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
    Blob12 StartPowerupSound;
    Blob12 LoopingPowerupSound;
    Blob12 ReleaseSound;
    Blob12 ReleaseLoopSound;
    Blob12 FinishSound;
    Blob12 CancelSound;
    float RandomFactorForNumSecsBetweenDamageHit;
    Blob12 NumSecsBetweenDamageHit;
    Blob12 DamagePerHit;
    Blob12 DurationOfSpellSecs;
    Blob12 RadiusOfEffect;
    Blob12 EnvChangeSecs;
    Blob12 EnvReturnSecs;
    bool DoLifting;
    float VelocityOfLift;
    float DistanceToLift;
    Blob12 StaminaCostPerSec;
    Blob12 DamageTakenMultiplier;
    float SmallRingRadius;
    float LargeRingRadius;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.StartPowerupSound);
    c.Transfer(d.LoopingPowerupSound);
    c.Transfer(d.ReleaseSound);
    c.Transfer(d.ReleaseLoopSound);
    c.Transfer(d.FinishSound);
    c.Transfer(d.CancelSound);
    c.Transfer(d.RandomFactorForNumSecsBetweenDamageHit);
    c.Transfer(d.NumSecsBetweenDamageHit);
    c.Transfer(d.DamagePerHit);
    c.Transfer(d.DurationOfSpellSecs);
    c.Transfer(d.RadiusOfEffect);
    c.Transfer(d.EnvChangeSecs);
    c.Transfer(d.EnvReturnSecs);
    c.Transfer(d.DoLifting);
    c.Transfer(d.VelocityOfLift);
    c.Transfer(d.DistanceToLift);
    c.Transfer(d.StaminaCostPerSec);
    c.Transfer(d.DamageTakenMultiplier);
    c.Transfer(d.SmallRingRadius);
    c.Transfer(d.LargeRingRadius);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    d.DoLifting = true;
    Context save(0); Persist(d, save);
    const void* want[20] = { &d.StartPowerupSound, &d.LoopingPowerupSound, &d.ReleaseSound, &d.ReleaseLoopSound, &d.FinishSound, &d.CancelSound, &d.RandomFactorForNumSecsBetweenDamageHit, &d.NumSecsBetweenDamageHit, &d.DamagePerHit, &d.DurationOfSpellSecs, &d.RadiusOfEffect, &d.EnvChangeSecs, &d.EnvReturnSecs, &d.DoLifting, &d.VelocityOfLift, &d.DistanceToLift, &d.StaminaCostPerSec, &d.DamageTakenMultiplier, &d.SmallRingRadius, &d.LargeRingRadius };
    if (save.count != 20u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 20u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.StartPowerupSound, 0, sizeof(e.StartPowerupSound));
    std::memset(&e.LoopingPowerupSound, 0, sizeof(e.LoopingPowerupSound));
    std::memset(&e.ReleaseSound, 0, sizeof(e.ReleaseSound));
    std::memset(&e.ReleaseLoopSound, 0, sizeof(e.ReleaseLoopSound));
    std::memset(&e.FinishSound, 0, sizeof(e.FinishSound));
    std::memset(&e.CancelSound, 0, sizeof(e.CancelSound));
    std::memset(&e.RandomFactorForNumSecsBetweenDamageHit, 0, sizeof(e.RandomFactorForNumSecsBetweenDamageHit));
    std::memset(&e.NumSecsBetweenDamageHit, 0, sizeof(e.NumSecsBetweenDamageHit));
    std::memset(&e.DamagePerHit, 0, sizeof(e.DamagePerHit));
    std::memset(&e.DurationOfSpellSecs, 0, sizeof(e.DurationOfSpellSecs));
    std::memset(&e.RadiusOfEffect, 0, sizeof(e.RadiusOfEffect));
    std::memset(&e.EnvChangeSecs, 0, sizeof(e.EnvChangeSecs));
    std::memset(&e.EnvReturnSecs, 0, sizeof(e.EnvReturnSecs));
    std::memset(&e.DoLifting, 0, sizeof(e.DoLifting));
    std::memset(&e.VelocityOfLift, 0, sizeof(e.VelocityOfLift));
    std::memset(&e.DistanceToLift, 0, sizeof(e.DistanceToLift));
    std::memset(&e.StaminaCostPerSec, 0, sizeof(e.StaminaCostPerSec));
    std::memset(&e.DamageTakenMultiplier, 0, sizeof(e.DamageTakenMultiplier));
    std::memset(&e.SmallRingRadius, 0, sizeof(e.SmallRingRadius));
    std::memset(&e.LargeRingRadius, 0, sizeof(e.LargeRingRadius));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("004656df_TEST FAIL %d\n", fail); return 1; }
    std::printf("004656df_TEST PASS\n");
    return 0;
}