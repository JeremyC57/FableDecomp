// behaviour test for 0x00463BFF CMeleeCombatAbilityDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x3C, +0x40, +0x44, +0x48, +0x49, +0x4C, +0x50, +0x5C, +0x60, +0x64, +0x65, +0x68, +0x74, +0x78),
// so a binary save / load round trip restores the def
#include <cstdio>
#include <cstring>
struct Blob4 { unsigned char b[4]; };
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
    Blob4 FlourishAnimName;
    Blob4 FlourishResponseAnimName;
    float DamageMultiplier;
    bool Flourish;
    bool BlockCounter;
    long AbilityButtonGraphicOverride;
    Blob12 ValidWeaponClasses;
    bool Decapitate;
    long KnockDownEffects;
    bool EvadeAllHits;
    bool CauseRecoil;
    Blob12 StatsNeeded;
    long ComboMultiplierLowerBound;
    long ComboMultiplierUpperBound;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.FlourishAnimName);
    c.Transfer(d.FlourishResponseAnimName);
    c.Transfer(d.DamageMultiplier);
    c.Transfer(d.Flourish);
    c.Transfer(d.BlockCounter);
    c.Transfer(d.AbilityButtonGraphicOverride);
    c.Transfer(d.ValidWeaponClasses);
    c.Transfer(d.Decapitate);
    c.Transfer(d.KnockDownEffects);
    c.Transfer(d.EvadeAllHits);
    c.Transfer(d.CauseRecoil);
    c.Transfer(d.StatsNeeded);
    c.Transfer(d.ComboMultiplierLowerBound);
    c.Transfer(d.ComboMultiplierUpperBound);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    d.Flourish = true;
    d.BlockCounter = true;
    d.Decapitate = true;
    d.EvadeAllHits = true;
    d.CauseRecoil = true;
    Context save(0); Persist(d, save);
    const void* want[14] = { &d.FlourishAnimName, &d.FlourishResponseAnimName, &d.DamageMultiplier, &d.Flourish, &d.BlockCounter, &d.AbilityButtonGraphicOverride, &d.ValidWeaponClasses, &d.Decapitate, &d.KnockDownEffects, &d.EvadeAllHits, &d.CauseRecoil, &d.StatsNeeded, &d.ComboMultiplierLowerBound, &d.ComboMultiplierUpperBound };
    if (save.count != 14u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 14u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.FlourishAnimName, 0, sizeof(e.FlourishAnimName));
    std::memset(&e.FlourishResponseAnimName, 0, sizeof(e.FlourishResponseAnimName));
    std::memset(&e.DamageMultiplier, 0, sizeof(e.DamageMultiplier));
    std::memset(&e.Flourish, 0, sizeof(e.Flourish));
    std::memset(&e.BlockCounter, 0, sizeof(e.BlockCounter));
    std::memset(&e.AbilityButtonGraphicOverride, 0, sizeof(e.AbilityButtonGraphicOverride));
    std::memset(&e.ValidWeaponClasses, 0, sizeof(e.ValidWeaponClasses));
    std::memset(&e.Decapitate, 0, sizeof(e.Decapitate));
    std::memset(&e.KnockDownEffects, 0, sizeof(e.KnockDownEffects));
    std::memset(&e.EvadeAllHits, 0, sizeof(e.EvadeAllHits));
    std::memset(&e.CauseRecoil, 0, sizeof(e.CauseRecoil));
    std::memset(&e.StatsNeeded, 0, sizeof(e.StatsNeeded));
    std::memset(&e.ComboMultiplierLowerBound, 0, sizeof(e.ComboMultiplierLowerBound));
    std::memset(&e.ComboMultiplierUpperBound, 0, sizeof(e.ComboMultiplierUpperBound));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("00463bff_TEST FAIL %d\n", fail); return 1; }
    std::printf("00463bff_TEST PASS\n");
    return 0;
}