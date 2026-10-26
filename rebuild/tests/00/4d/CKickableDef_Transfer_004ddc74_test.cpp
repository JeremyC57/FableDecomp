// behaviour test for 0x004DDC74 CKickableDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x2C, +0x30, +0x34, +0x38, +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x50),
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
    float BaseDistanceToTravel;
    float StrengthScaleRange;
    float SpeedScaleRange;
    float BaseSpeedScaleRange;
    float RandomBase;
    float RandomRange;
    long ShotToAttach;
    long OpinionDeedType;
    long OpinionDeedFaction;
    long OpinionDeedTypeToEnemies;
    bool IgnoreNavigationTest;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.BaseDistanceToTravel);
    c.Transfer(d.StrengthScaleRange);
    c.Transfer(d.SpeedScaleRange);
    c.Transfer(d.BaseSpeedScaleRange);
    c.Transfer(d.RandomBase);
    c.Transfer(d.RandomRange);
    c.Transfer(d.ShotToAttach);
    c.Transfer(d.OpinionDeedType);
    c.Transfer(d.OpinionDeedFaction);
    c.Transfer(d.OpinionDeedTypeToEnemies);
    c.Transfer(d.IgnoreNavigationTest);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    d.IgnoreNavigationTest = true;
    Context save(0); Persist(d, save);
    const void* want[11] = { &d.BaseDistanceToTravel, &d.StrengthScaleRange, &d.SpeedScaleRange, &d.BaseSpeedScaleRange, &d.RandomBase, &d.RandomRange, &d.ShotToAttach, &d.OpinionDeedType, &d.OpinionDeedFaction, &d.OpinionDeedTypeToEnemies, &d.IgnoreNavigationTest };
    if (save.count != 11u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 11u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.BaseDistanceToTravel, 0, sizeof(e.BaseDistanceToTravel));
    std::memset(&e.StrengthScaleRange, 0, sizeof(e.StrengthScaleRange));
    std::memset(&e.SpeedScaleRange, 0, sizeof(e.SpeedScaleRange));
    std::memset(&e.BaseSpeedScaleRange, 0, sizeof(e.BaseSpeedScaleRange));
    std::memset(&e.RandomBase, 0, sizeof(e.RandomBase));
    std::memset(&e.RandomRange, 0, sizeof(e.RandomRange));
    std::memset(&e.ShotToAttach, 0, sizeof(e.ShotToAttach));
    std::memset(&e.OpinionDeedType, 0, sizeof(e.OpinionDeedType));
    std::memset(&e.OpinionDeedFaction, 0, sizeof(e.OpinionDeedFaction));
    std::memset(&e.OpinionDeedTypeToEnemies, 0, sizeof(e.OpinionDeedTypeToEnemies));
    std::memset(&e.IgnoreNavigationTest, 0, sizeof(e.IgnoreNavigationTest));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("004ddc74_TEST FAIL %d\n", fail); return 1; }
    std::printf("004ddc74_TEST PASS\n");
    return 0;
}