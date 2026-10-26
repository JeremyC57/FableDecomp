// behaviour test for 0x00452F8E CBriarRoseDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x2C, +0x30, +0x34, +0x38, +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x50),
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
    float SecondsToTeleport;
    float SecondsToBeIntangible;
    float SecondsToChargeDrainLife;
    long GuranteedImpostersHitBeforeAllowedToHitBriar;
    long NumberOfHitsToKillImposters;
    long EnvironmentThemeForIntangibleMode;
    float SecondsForIntangibleEnvironmentThemeTransitionIn;
    float SecondsForIntangibleEnvironmentThemeTransitionOut;
    float SecondsBetweenWillUse;
    float SecondsBetweenTeleportAway;
    float DistanceForcePushThreshold;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.SecondsToTeleport);
    c.Transfer(d.SecondsToBeIntangible);
    c.Transfer(d.SecondsToChargeDrainLife);
    c.Transfer(d.GuranteedImpostersHitBeforeAllowedToHitBriar);
    c.Transfer(d.NumberOfHitsToKillImposters);
    c.Transfer(d.EnvironmentThemeForIntangibleMode);
    c.Transfer(d.SecondsForIntangibleEnvironmentThemeTransitionIn);
    c.Transfer(d.SecondsForIntangibleEnvironmentThemeTransitionOut);
    c.Transfer(d.SecondsBetweenWillUse);
    c.Transfer(d.SecondsBetweenTeleportAway);
    c.Transfer(d.DistanceForcePushThreshold);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[11] = { &d.SecondsToTeleport, &d.SecondsToBeIntangible, &d.SecondsToChargeDrainLife, &d.GuranteedImpostersHitBeforeAllowedToHitBriar, &d.NumberOfHitsToKillImposters, &d.EnvironmentThemeForIntangibleMode, &d.SecondsForIntangibleEnvironmentThemeTransitionIn, &d.SecondsForIntangibleEnvironmentThemeTransitionOut, &d.SecondsBetweenWillUse, &d.SecondsBetweenTeleportAway, &d.DistanceForcePushThreshold };
    if (save.count != 11u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 11u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.SecondsToTeleport, 0, sizeof(e.SecondsToTeleport));
    std::memset(&e.SecondsToBeIntangible, 0, sizeof(e.SecondsToBeIntangible));
    std::memset(&e.SecondsToChargeDrainLife, 0, sizeof(e.SecondsToChargeDrainLife));
    std::memset(&e.GuranteedImpostersHitBeforeAllowedToHitBriar, 0, sizeof(e.GuranteedImpostersHitBeforeAllowedToHitBriar));
    std::memset(&e.NumberOfHitsToKillImposters, 0, sizeof(e.NumberOfHitsToKillImposters));
    std::memset(&e.EnvironmentThemeForIntangibleMode, 0, sizeof(e.EnvironmentThemeForIntangibleMode));
    std::memset(&e.SecondsForIntangibleEnvironmentThemeTransitionIn, 0, sizeof(e.SecondsForIntangibleEnvironmentThemeTransitionIn));
    std::memset(&e.SecondsForIntangibleEnvironmentThemeTransitionOut, 0, sizeof(e.SecondsForIntangibleEnvironmentThemeTransitionOut));
    std::memset(&e.SecondsBetweenWillUse, 0, sizeof(e.SecondsBetweenWillUse));
    std::memset(&e.SecondsBetweenTeleportAway, 0, sizeof(e.SecondsBetweenTeleportAway));
    std::memset(&e.DistanceForcePushThreshold, 0, sizeof(e.DistanceForcePushThreshold));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("00452f8e_TEST FAIL %d\n", fail); return 1; }
    std::printf("00452f8e_TEST PASS\n");
    return 0;
}