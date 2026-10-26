// behaviour test for 0x00465634 CSpecialAbilitiesHealLifeDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x3C, +0x48, +0x54),
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
    Blob12 HealthIncreasePerFrame;
    Blob12 TotalHealthIncrease;
    Blob12 RadiusOfEffect;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.HealthIncreasePerFrame);
    c.Transfer(d.TotalHealthIncrease);
    c.Transfer(d.RadiusOfEffect);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[3] = { &d.HealthIncreasePerFrame, &d.TotalHealthIncrease, &d.RadiusOfEffect };
    if (save.count != 3u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 3u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.HealthIncreasePerFrame, 0, sizeof(e.HealthIncreasePerFrame));
    std::memset(&e.TotalHealthIncrease, 0, sizeof(e.TotalHealthIncrease));
    std::memset(&e.RadiusOfEffect, 0, sizeof(e.RadiusOfEffect));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("00465634_TEST FAIL %d\n", fail); return 1; }
    std::printf("00465634_TEST PASS\n");
    return 0;
}