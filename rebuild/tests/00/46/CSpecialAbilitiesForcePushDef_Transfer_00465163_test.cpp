// behaviour test for 0x00465163 CSpecialAbilitiesForcePushDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x50, +0x54, +0x58, +0x5C, +0x60, +0x64, +0x68, +0x6C, +0x70, +0x74, +0x78, +0x84, +0x88, +0x8C, +0x98),
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
    float MaxDistanceToPush;
    float MinDistanceToPush;
    float DustBoxWidth;
    long MaxNumOfExtraEffects;
    long MinNumOfExtraEffects;
    float LevelUpTimings[3];
    float ReleaseLevel1Delay;
    float ReleaseLevel2Delay;
    float ReleaseLevel3DelayA;
    float ReleaseLevel3DelayB;
    float ReleaseLevel4Delay;
    float HeightOffGround;
    float Level4OffsetMagnitude;
    Blob12 RadiusOfEffect;
    float ExtraPush;
    float PushSpeed;
    Blob12 ObjectDamage;
    Blob12 TimeDelayBetweenCastsSec;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.MaxDistanceToPush);
    c.Transfer(d.MinDistanceToPush);
    c.Transfer(d.DustBoxWidth);
    c.Transfer(d.MaxNumOfExtraEffects);
    c.Transfer(d.MinNumOfExtraEffects);
    c.Transfer(d.LevelUpTimings[0]);
    c.Transfer(d.LevelUpTimings[1]);
    c.Transfer(d.LevelUpTimings[2]);
    c.Transfer(d.ReleaseLevel1Delay);
    c.Transfer(d.ReleaseLevel2Delay);
    c.Transfer(d.ReleaseLevel3DelayA);
    c.Transfer(d.ReleaseLevel3DelayB);
    c.Transfer(d.ReleaseLevel4Delay);
    c.Transfer(d.HeightOffGround);
    c.Transfer(d.Level4OffsetMagnitude);
    c.Transfer(d.RadiusOfEffect);
    c.Transfer(d.ExtraPush);
    c.Transfer(d.PushSpeed);
    c.Transfer(d.ObjectDamage);
    c.Transfer(d.TimeDelayBetweenCastsSec);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[20] = { &d.MaxDistanceToPush, &d.MinDistanceToPush, &d.DustBoxWidth, &d.MaxNumOfExtraEffects, &d.MinNumOfExtraEffects, &d.LevelUpTimings[0], &d.LevelUpTimings[1], &d.LevelUpTimings[2], &d.ReleaseLevel1Delay, &d.ReleaseLevel2Delay, &d.ReleaseLevel3DelayA, &d.ReleaseLevel3DelayB, &d.ReleaseLevel4Delay, &d.HeightOffGround, &d.Level4OffsetMagnitude, &d.RadiusOfEffect, &d.ExtraPush, &d.PushSpeed, &d.ObjectDamage, &d.TimeDelayBetweenCastsSec };
    if (save.count != 20u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 20u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.MaxDistanceToPush, 0, sizeof(e.MaxDistanceToPush));
    std::memset(&e.MinDistanceToPush, 0, sizeof(e.MinDistanceToPush));
    std::memset(&e.DustBoxWidth, 0, sizeof(e.DustBoxWidth));
    std::memset(&e.MaxNumOfExtraEffects, 0, sizeof(e.MaxNumOfExtraEffects));
    std::memset(&e.MinNumOfExtraEffects, 0, sizeof(e.MinNumOfExtraEffects));
    std::memset(&e.LevelUpTimings[0], 0, sizeof(e.LevelUpTimings[0]));
    std::memset(&e.LevelUpTimings[1], 0, sizeof(e.LevelUpTimings[1]));
    std::memset(&e.LevelUpTimings[2], 0, sizeof(e.LevelUpTimings[2]));
    std::memset(&e.ReleaseLevel1Delay, 0, sizeof(e.ReleaseLevel1Delay));
    std::memset(&e.ReleaseLevel2Delay, 0, sizeof(e.ReleaseLevel2Delay));
    std::memset(&e.ReleaseLevel3DelayA, 0, sizeof(e.ReleaseLevel3DelayA));
    std::memset(&e.ReleaseLevel3DelayB, 0, sizeof(e.ReleaseLevel3DelayB));
    std::memset(&e.ReleaseLevel4Delay, 0, sizeof(e.ReleaseLevel4Delay));
    std::memset(&e.HeightOffGround, 0, sizeof(e.HeightOffGround));
    std::memset(&e.Level4OffsetMagnitude, 0, sizeof(e.Level4OffsetMagnitude));
    std::memset(&e.RadiusOfEffect, 0, sizeof(e.RadiusOfEffect));
    std::memset(&e.ExtraPush, 0, sizeof(e.ExtraPush));
    std::memset(&e.PushSpeed, 0, sizeof(e.PushSpeed));
    std::memset(&e.ObjectDamage, 0, sizeof(e.ObjectDamage));
    std::memset(&e.TimeDelayBetweenCastsSec, 0, sizeof(e.TimeDelayBetweenCastsSec));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("00465163_TEST FAIL %d\n", fail); return 1; }
    std::printf("00465163_TEST PASS\n");
    return 0;
}