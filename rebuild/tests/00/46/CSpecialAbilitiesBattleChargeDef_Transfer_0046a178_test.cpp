// behaviour test for 0x0046A178 CSpecialAbilitiesBattleChargeDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x3C, +0x40, +0x4C, +0x58, +0x64, +0x68, +0x6C, +0x70, +0x74, +0x78, +0x7C, +0x80, +0x84, +0x88, +0x8C, +0x90, +0x9C, +0xA0),
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
    float RunSpeed;
    Blob12 DashDistance;
    Blob12 RadiusOfEffect;
    Blob12 BattleChargeDamage;
    float BattleChargeCombatMultiplierFactor;
    float BattleChargeRadialStartSeconds;
    float BattleChargeRadialEndSeconds;
    float BattleChargeRadialThreshold;
    float BattleChargeRadialLength;
    float BattleChargeRadialIntensity;
    float BattleChargeRadialFade;
    float BattleChargeRadialRange;
    float BattleChargeRadialTwist;
    float PoseDelaySecs;
    float ChargeDelay;
    Blob12 NumPeopleHit;
    float DragonExtraGumphRadiusMultiplier;
    float DragonExtraGumphRadius;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.RunSpeed);
    c.Transfer(d.DashDistance);
    c.Transfer(d.RadiusOfEffect);
    c.Transfer(d.BattleChargeDamage);
    c.Transfer(d.BattleChargeCombatMultiplierFactor);
    c.Transfer(d.BattleChargeRadialStartSeconds);
    c.Transfer(d.BattleChargeRadialEndSeconds);
    c.Transfer(d.BattleChargeRadialThreshold);
    c.Transfer(d.BattleChargeRadialLength);
    c.Transfer(d.BattleChargeRadialIntensity);
    c.Transfer(d.BattleChargeRadialFade);
    c.Transfer(d.BattleChargeRadialRange);
    c.Transfer(d.BattleChargeRadialTwist);
    c.Transfer(d.PoseDelaySecs);
    c.Transfer(d.ChargeDelay);
    c.Transfer(d.NumPeopleHit);
    c.Transfer(d.DragonExtraGumphRadiusMultiplier);
    c.Transfer(d.DragonExtraGumphRadius);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[18] = { &d.RunSpeed, &d.DashDistance, &d.RadiusOfEffect, &d.BattleChargeDamage, &d.BattleChargeCombatMultiplierFactor, &d.BattleChargeRadialStartSeconds, &d.BattleChargeRadialEndSeconds, &d.BattleChargeRadialThreshold, &d.BattleChargeRadialLength, &d.BattleChargeRadialIntensity, &d.BattleChargeRadialFade, &d.BattleChargeRadialRange, &d.BattleChargeRadialTwist, &d.PoseDelaySecs, &d.ChargeDelay, &d.NumPeopleHit, &d.DragonExtraGumphRadiusMultiplier, &d.DragonExtraGumphRadius };
    if (save.count != 18u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 18u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.RunSpeed, 0, sizeof(e.RunSpeed));
    std::memset(&e.DashDistance, 0, sizeof(e.DashDistance));
    std::memset(&e.RadiusOfEffect, 0, sizeof(e.RadiusOfEffect));
    std::memset(&e.BattleChargeDamage, 0, sizeof(e.BattleChargeDamage));
    std::memset(&e.BattleChargeCombatMultiplierFactor, 0, sizeof(e.BattleChargeCombatMultiplierFactor));
    std::memset(&e.BattleChargeRadialStartSeconds, 0, sizeof(e.BattleChargeRadialStartSeconds));
    std::memset(&e.BattleChargeRadialEndSeconds, 0, sizeof(e.BattleChargeRadialEndSeconds));
    std::memset(&e.BattleChargeRadialThreshold, 0, sizeof(e.BattleChargeRadialThreshold));
    std::memset(&e.BattleChargeRadialLength, 0, sizeof(e.BattleChargeRadialLength));
    std::memset(&e.BattleChargeRadialIntensity, 0, sizeof(e.BattleChargeRadialIntensity));
    std::memset(&e.BattleChargeRadialFade, 0, sizeof(e.BattleChargeRadialFade));
    std::memset(&e.BattleChargeRadialRange, 0, sizeof(e.BattleChargeRadialRange));
    std::memset(&e.BattleChargeRadialTwist, 0, sizeof(e.BattleChargeRadialTwist));
    std::memset(&e.PoseDelaySecs, 0, sizeof(e.PoseDelaySecs));
    std::memset(&e.ChargeDelay, 0, sizeof(e.ChargeDelay));
    std::memset(&e.NumPeopleHit, 0, sizeof(e.NumPeopleHit));
    std::memset(&e.DragonExtraGumphRadiusMultiplier, 0, sizeof(e.DragonExtraGumphRadiusMultiplier));
    std::memset(&e.DragonExtraGumphRadius, 0, sizeof(e.DragonExtraGumphRadius));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("0046a178_TEST FAIL %d\n", fail); return 1; }
    std::printf("0046a178_TEST PASS\n");
    return 0;
}