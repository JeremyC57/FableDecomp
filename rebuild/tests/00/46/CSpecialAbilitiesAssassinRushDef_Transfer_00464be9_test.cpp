// behaviour test for 0x00464BE9 CSpecialAbilitiesAssassinRushDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x50, +0x54, +0x58, +0x5C, +0x60, +0x64, +0x68, +0x6C, +0x70, +0x74, +0x78, +0x7C, +0x80),
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
    float DistanceAwayFromtargetToAssassinRushTo;
    float SafeCollideRadiusPercentage;
    float DistFromFloor;
    float AssassinStartSeconds;
    float AssassinEndSeconds;
    float AssassinThreshold;
    float AssassinLength;
    float AssassinIntensity;
    float AssassinFade;
    float AssassinRange;
    float AssassinTwist;
    float RunSpeed;
    float TrailDelaySecs;
    float SlowTimeLength;
    long AssassinAlpha;
    long SlowTimeMultiplier;
    float RushDelay;
    Blob12 DashDistance;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.DistanceAwayFromtargetToAssassinRushTo);
    c.Transfer(d.SafeCollideRadiusPercentage);
    c.Transfer(d.DistFromFloor);
    c.Transfer(d.AssassinStartSeconds);
    c.Transfer(d.AssassinEndSeconds);
    c.Transfer(d.AssassinThreshold);
    c.Transfer(d.AssassinLength);
    c.Transfer(d.AssassinIntensity);
    c.Transfer(d.AssassinFade);
    c.Transfer(d.AssassinRange);
    c.Transfer(d.AssassinTwist);
    c.Transfer(d.RunSpeed);
    c.Transfer(d.TrailDelaySecs);
    c.Transfer(d.SlowTimeLength);
    c.Transfer(d.AssassinAlpha);
    c.Transfer(d.SlowTimeMultiplier);
    c.Transfer(d.RushDelay);
    c.Transfer(d.DashDistance);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[18] = { &d.DistanceAwayFromtargetToAssassinRushTo, &d.SafeCollideRadiusPercentage, &d.DistFromFloor, &d.AssassinStartSeconds, &d.AssassinEndSeconds, &d.AssassinThreshold, &d.AssassinLength, &d.AssassinIntensity, &d.AssassinFade, &d.AssassinRange, &d.AssassinTwist, &d.RunSpeed, &d.TrailDelaySecs, &d.SlowTimeLength, &d.AssassinAlpha, &d.SlowTimeMultiplier, &d.RushDelay, &d.DashDistance };
    if (save.count != 18u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 18u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.DistanceAwayFromtargetToAssassinRushTo, 0, sizeof(e.DistanceAwayFromtargetToAssassinRushTo));
    std::memset(&e.SafeCollideRadiusPercentage, 0, sizeof(e.SafeCollideRadiusPercentage));
    std::memset(&e.DistFromFloor, 0, sizeof(e.DistFromFloor));
    std::memset(&e.AssassinStartSeconds, 0, sizeof(e.AssassinStartSeconds));
    std::memset(&e.AssassinEndSeconds, 0, sizeof(e.AssassinEndSeconds));
    std::memset(&e.AssassinThreshold, 0, sizeof(e.AssassinThreshold));
    std::memset(&e.AssassinLength, 0, sizeof(e.AssassinLength));
    std::memset(&e.AssassinIntensity, 0, sizeof(e.AssassinIntensity));
    std::memset(&e.AssassinFade, 0, sizeof(e.AssassinFade));
    std::memset(&e.AssassinRange, 0, sizeof(e.AssassinRange));
    std::memset(&e.AssassinTwist, 0, sizeof(e.AssassinTwist));
    std::memset(&e.RunSpeed, 0, sizeof(e.RunSpeed));
    std::memset(&e.TrailDelaySecs, 0, sizeof(e.TrailDelaySecs));
    std::memset(&e.SlowTimeLength, 0, sizeof(e.SlowTimeLength));
    std::memset(&e.AssassinAlpha, 0, sizeof(e.AssassinAlpha));
    std::memset(&e.SlowTimeMultiplier, 0, sizeof(e.SlowTimeMultiplier));
    std::memset(&e.RushDelay, 0, sizeof(e.RushDelay));
    std::memset(&e.DashDistance, 0, sizeof(e.DashDistance));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("00464be9_TEST FAIL %d\n", fail); return 1; }
    std::printf("00464be9_TEST PASS\n");
    return 0;
}