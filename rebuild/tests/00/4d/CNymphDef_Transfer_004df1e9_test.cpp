// behaviour test for 0x004DF1E9 CNymphDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x30, +0x2C, +0x34, +0x38, +0x3C, +0x40, +0x44, +0x48, +0x4C),
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
    float MinAttackRange;
    float MaxHeight;
    float MinHeight;
    float CircleRange;
    long FlyAroundTime;
    long ShotTrapType;
    long ShotsBeforeIdle;
    float IdleTimeBetweenShots;
    float FadeInTime;
    float FadeOutTime;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.MinAttackRange);
    c.Transfer(d.MinHeight);
    c.Transfer(d.MaxHeight);
    c.Transfer(d.CircleRange);
    c.Transfer(d.FlyAroundTime);
    c.Transfer(d.ShotTrapType);
    c.Transfer(d.ShotsBeforeIdle);
    c.Transfer(d.IdleTimeBetweenShots);
    c.Transfer(d.FadeInTime);
    c.Transfer(d.FadeOutTime);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[10] = { &d.MinAttackRange, &d.MinHeight, &d.MaxHeight, &d.CircleRange, &d.FlyAroundTime, &d.ShotTrapType, &d.ShotsBeforeIdle, &d.IdleTimeBetweenShots, &d.FadeInTime, &d.FadeOutTime };
    if (save.count != 10u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 10u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.MinAttackRange, 0, sizeof(e.MinAttackRange));
    std::memset(&e.MinHeight, 0, sizeof(e.MinHeight));
    std::memset(&e.MaxHeight, 0, sizeof(e.MaxHeight));
    std::memset(&e.CircleRange, 0, sizeof(e.CircleRange));
    std::memset(&e.FlyAroundTime, 0, sizeof(e.FlyAroundTime));
    std::memset(&e.ShotTrapType, 0, sizeof(e.ShotTrapType));
    std::memset(&e.ShotsBeforeIdle, 0, sizeof(e.ShotsBeforeIdle));
    std::memset(&e.IdleTimeBetweenShots, 0, sizeof(e.IdleTimeBetweenShots));
    std::memset(&e.FadeInTime, 0, sizeof(e.FadeInTime));
    std::memset(&e.FadeOutTime, 0, sizeof(e.FadeOutTime));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("004df1e9_TEST FAIL %d\n", fail); return 1; }
    std::printf("004df1e9_TEST PASS\n");
    return 0;
}