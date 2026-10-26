// behaviour test for 0x004530DC CSummonerDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x2C, +0x30, +0x34, +0x38, +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x50, +0x54, +0x58),
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
    float LightningRangeForAutomaticUse;
    float LightningPercentChanceForRandomUse;
    float LightningPercentChanceForBoastUse;
    float LightningRandomChanceTestSeconds;
    float LightningSecondsToWaitBeforeReuse;
    float ChargeRangeNear;
    float ChargeRangeFar;
    float ChargeRangeAttackInterrupt;
    float ChargeSecondsToWaitBeforeReuse;
    long SummonerDeathExplosion;
    long LightningOrb;
    long LightningOrbAttackFireheart;
    float LightningOrbLifeSeconds;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.LightningRangeForAutomaticUse);
    c.Transfer(d.LightningPercentChanceForRandomUse);
    c.Transfer(d.LightningPercentChanceForBoastUse);
    c.Transfer(d.LightningRandomChanceTestSeconds);
    c.Transfer(d.LightningSecondsToWaitBeforeReuse);
    c.Transfer(d.ChargeRangeNear);
    c.Transfer(d.ChargeRangeFar);
    c.Transfer(d.ChargeRangeAttackInterrupt);
    c.Transfer(d.ChargeSecondsToWaitBeforeReuse);
    c.Transfer(d.SummonerDeathExplosion);
    c.Transfer(d.LightningOrb);
    c.Transfer(d.LightningOrbAttackFireheart);
    c.Transfer(d.LightningOrbLifeSeconds);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[13] = { &d.LightningRangeForAutomaticUse, &d.LightningPercentChanceForRandomUse, &d.LightningPercentChanceForBoastUse, &d.LightningRandomChanceTestSeconds, &d.LightningSecondsToWaitBeforeReuse, &d.ChargeRangeNear, &d.ChargeRangeFar, &d.ChargeRangeAttackInterrupt, &d.ChargeSecondsToWaitBeforeReuse, &d.SummonerDeathExplosion, &d.LightningOrb, &d.LightningOrbAttackFireheart, &d.LightningOrbLifeSeconds };
    if (save.count != 13u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 13u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.LightningRangeForAutomaticUse, 0, sizeof(e.LightningRangeForAutomaticUse));
    std::memset(&e.LightningPercentChanceForRandomUse, 0, sizeof(e.LightningPercentChanceForRandomUse));
    std::memset(&e.LightningPercentChanceForBoastUse, 0, sizeof(e.LightningPercentChanceForBoastUse));
    std::memset(&e.LightningRandomChanceTestSeconds, 0, sizeof(e.LightningRandomChanceTestSeconds));
    std::memset(&e.LightningSecondsToWaitBeforeReuse, 0, sizeof(e.LightningSecondsToWaitBeforeReuse));
    std::memset(&e.ChargeRangeNear, 0, sizeof(e.ChargeRangeNear));
    std::memset(&e.ChargeRangeFar, 0, sizeof(e.ChargeRangeFar));
    std::memset(&e.ChargeRangeAttackInterrupt, 0, sizeof(e.ChargeRangeAttackInterrupt));
    std::memset(&e.ChargeSecondsToWaitBeforeReuse, 0, sizeof(e.ChargeSecondsToWaitBeforeReuse));
    std::memset(&e.SummonerDeathExplosion, 0, sizeof(e.SummonerDeathExplosion));
    std::memset(&e.LightningOrb, 0, sizeof(e.LightningOrb));
    std::memset(&e.LightningOrbAttackFireheart, 0, sizeof(e.LightningOrbAttackFireheart));
    std::memset(&e.LightningOrbLifeSeconds, 0, sizeof(e.LightningOrbLifeSeconds));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("004530dc_TEST FAIL %d\n", fail); return 1; }
    std::printf("004530dc_TEST PASS\n");
    return 0;
}