// behaviour test for 0x004E7F24 CTurncoatDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x25, +0x28, +0x2C, +0x30, +0x34, +0x38, +0x3C, +0x40, +0x4C, +0x50),
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
    bool Turncoatable;
    long TurncoatedEffect;
    long TurncoatActivateEffect;
    long TurncoatDeactivateEffect;
    float MinimumResistance;
    float TimeDelayBeforeResistanceRecoverySecs;
    float ResistanceRecoveryPerSec;
    Blob12 TurncoatStageEffects;
    float PercentageOfHealthAsResistance;
    float ExtraResistance;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.Turncoatable);
    c.Transfer(d.TurncoatedEffect);
    c.Transfer(d.TurncoatActivateEffect);
    c.Transfer(d.TurncoatDeactivateEffect);
    c.Transfer(d.MinimumResistance);
    c.Transfer(d.TimeDelayBeforeResistanceRecoverySecs);
    c.Transfer(d.ResistanceRecoveryPerSec);
    c.Transfer(d.TurncoatStageEffects);
    c.Transfer(d.PercentageOfHealthAsResistance);
    c.Transfer(d.ExtraResistance);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    d.Turncoatable = true;
    Context save(0); Persist(d, save);
    const void* want[10] = { &d.Turncoatable, &d.TurncoatedEffect, &d.TurncoatActivateEffect, &d.TurncoatDeactivateEffect, &d.MinimumResistance, &d.TimeDelayBeforeResistanceRecoverySecs, &d.ResistanceRecoveryPerSec, &d.TurncoatStageEffects, &d.PercentageOfHealthAsResistance, &d.ExtraResistance };
    if (save.count != 10u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 10u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.Turncoatable, 0, sizeof(e.Turncoatable));
    std::memset(&e.TurncoatedEffect, 0, sizeof(e.TurncoatedEffect));
    std::memset(&e.TurncoatActivateEffect, 0, sizeof(e.TurncoatActivateEffect));
    std::memset(&e.TurncoatDeactivateEffect, 0, sizeof(e.TurncoatDeactivateEffect));
    std::memset(&e.MinimumResistance, 0, sizeof(e.MinimumResistance));
    std::memset(&e.TimeDelayBeforeResistanceRecoverySecs, 0, sizeof(e.TimeDelayBeforeResistanceRecoverySecs));
    std::memset(&e.ResistanceRecoveryPerSec, 0, sizeof(e.ResistanceRecoveryPerSec));
    std::memset(&e.TurncoatStageEffects, 0, sizeof(e.TurncoatStageEffects));
    std::memset(&e.PercentageOfHealthAsResistance, 0, sizeof(e.PercentageOfHealthAsResistance));
    std::memset(&e.ExtraResistance, 0, sizeof(e.ExtraResistance));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("004e7f24_TEST FAIL %d\n", fail); return 1; }
    std::printf("004e7f24_TEST PASS\n");
    return 0;
}