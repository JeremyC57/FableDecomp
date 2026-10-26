// behaviour test for 0x0046FC51 CAppearanceModifierDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x34, +0x40, +0x44, +0x48, +0x4C, +0x50, +0x64, +0x68, +0x6C, +0x54, +0x58, +0x5C, +0x60),
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
    Blob12 HideMaterials;
    Blob12 MorphGraphics;
    long CoversBodyAreaFlags;
    bool IsRemoveable;
    long HeroSuit;
    int SuitPart;
    int AppearanceType;
    float SoundRadiusMultiplier;
    float VisibilityMultiplier;
    long ArmourDefIndex;
    float OveralProtectionWeighting;
    float Scariness;
    float Attractiveness;
    float Goodstrength;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.HideMaterials);
    c.Transfer(d.MorphGraphics);
    c.Transfer(d.CoversBodyAreaFlags);
    c.Transfer(d.IsRemoveable);
    c.Transfer(d.HeroSuit);
    c.Transfer(d.SuitPart);
    c.Transfer(d.AppearanceType);
    c.Transfer(d.Scariness);
    c.Transfer(d.Attractiveness);
    c.Transfer(d.Goodstrength);
    c.Transfer(d.SoundRadiusMultiplier);
    c.Transfer(d.VisibilityMultiplier);
    c.Transfer(d.ArmourDefIndex);
    c.Transfer(d.OveralProtectionWeighting);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    d.IsRemoveable = true;
    Context save(0); Persist(d, save);
    const void* want[14] = { &d.HideMaterials, &d.MorphGraphics, &d.CoversBodyAreaFlags, &d.IsRemoveable, &d.HeroSuit, &d.SuitPart, &d.AppearanceType, &d.Scariness, &d.Attractiveness, &d.Goodstrength, &d.SoundRadiusMultiplier, &d.VisibilityMultiplier, &d.ArmourDefIndex, &d.OveralProtectionWeighting };
    if (save.count != 14u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 14u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.HideMaterials, 0, sizeof(e.HideMaterials));
    std::memset(&e.MorphGraphics, 0, sizeof(e.MorphGraphics));
    std::memset(&e.CoversBodyAreaFlags, 0, sizeof(e.CoversBodyAreaFlags));
    std::memset(&e.IsRemoveable, 0, sizeof(e.IsRemoveable));
    std::memset(&e.HeroSuit, 0, sizeof(e.HeroSuit));
    std::memset(&e.SuitPart, 0, sizeof(e.SuitPart));
    std::memset(&e.AppearanceType, 0, sizeof(e.AppearanceType));
    std::memset(&e.Scariness, 0, sizeof(e.Scariness));
    std::memset(&e.Attractiveness, 0, sizeof(e.Attractiveness));
    std::memset(&e.Goodstrength, 0, sizeof(e.Goodstrength));
    std::memset(&e.SoundRadiusMultiplier, 0, sizeof(e.SoundRadiusMultiplier));
    std::memset(&e.VisibilityMultiplier, 0, sizeof(e.VisibilityMultiplier));
    std::memset(&e.ArmourDefIndex, 0, sizeof(e.ArmourDefIndex));
    std::memset(&e.OveralProtectionWeighting, 0, sizeof(e.OveralProtectionWeighting));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("0046fc51_TEST FAIL %d\n", fail); return 1; }
    std::printf("0046fc51_TEST PASS\n");
    return 0;
}