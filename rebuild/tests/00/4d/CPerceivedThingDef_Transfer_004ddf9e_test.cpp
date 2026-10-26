// behaviour test for 0x004DDF9E CPerceivedThingDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x34, +0x30, +0x38, +0x3C, +0x40, +0x44, +0x2C, +0x48, +0x4C),
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
    float FieldOfView;
    float GiveUpChaseRadius;
    float SightRadius;
    float ExtendedSightRadius;
    float SoundRadius;
    float SmellRadius;
    long Type;
    float AwarenessTimer;
    float DayTimeVisibilityMultiplier;
    float NightTimeVisibilityMultiplier;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.FieldOfView);
    c.Transfer(d.ExtendedSightRadius);
    c.Transfer(d.SightRadius);
    c.Transfer(d.SoundRadius);
    c.Transfer(d.SmellRadius);
    c.Transfer(d.Type);
    c.Transfer(d.AwarenessTimer);
    c.Transfer(d.GiveUpChaseRadius);
    c.Transfer(d.DayTimeVisibilityMultiplier);
    c.Transfer(d.NightTimeVisibilityMultiplier);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[10] = { &d.FieldOfView, &d.ExtendedSightRadius, &d.SightRadius, &d.SoundRadius, &d.SmellRadius, &d.Type, &d.AwarenessTimer, &d.GiveUpChaseRadius, &d.DayTimeVisibilityMultiplier, &d.NightTimeVisibilityMultiplier };
    if (save.count != 10u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 10u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.FieldOfView, 0, sizeof(e.FieldOfView));
    std::memset(&e.ExtendedSightRadius, 0, sizeof(e.ExtendedSightRadius));
    std::memset(&e.SightRadius, 0, sizeof(e.SightRadius));
    std::memset(&e.SoundRadius, 0, sizeof(e.SoundRadius));
    std::memset(&e.SmellRadius, 0, sizeof(e.SmellRadius));
    std::memset(&e.Type, 0, sizeof(e.Type));
    std::memset(&e.AwarenessTimer, 0, sizeof(e.AwarenessTimer));
    std::memset(&e.GiveUpChaseRadius, 0, sizeof(e.GiveUpChaseRadius));
    std::memset(&e.DayTimeVisibilityMultiplier, 0, sizeof(e.DayTimeVisibilityMultiplier));
    std::memset(&e.NightTimeVisibilityMultiplier, 0, sizeof(e.NightTimeVisibilityMultiplier));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("004ddf9e_TEST FAIL %d\n", fail); return 1; }
    std::printf("004ddf9e_TEST PASS\n");
    return 0;
}