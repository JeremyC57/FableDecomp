// behaviour test for 0x004529AE CTattooDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x2C, +0x30, +0x34, +0x38, +0x3C, +0x3D, +0x40, +0x58, +0x54, +0x5C, +0x44, +0x48, +0x4C, +0x50),
// so a binary save / load round trip restores the def
#include <cstdio>
#include <cstring>
struct Blob4 { unsigned char b[4]; };
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
    Blob4 TattooName;
    long TextureLayer;
    long ReplacingTextureIndex;
    long BankIndex;
    long Blend;
    bool RemovesAllTattoos;
    bool Permanent;
    long CoversBodyAreaFlags;
    float VisibilityMultiplier;
    long SpecificCoversBodyAreaFlags;
    Blob4 CustomFileName;
    Blob4 CustomFileNameBase;
    float Attractiveness;
    float Scariness;
    float Goodstrength;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.TattooName);
    c.Transfer(d.TextureLayer);
    c.Transfer(d.ReplacingTextureIndex);
    c.Transfer(d.BankIndex);
    c.Transfer(d.Blend);
    c.Transfer(d.RemovesAllTattoos);
    c.Transfer(d.Permanent);
    c.Transfer(d.CoversBodyAreaFlags);
    c.Transfer(d.Scariness);
    c.Transfer(d.Attractiveness);
    c.Transfer(d.Goodstrength);
    c.Transfer(d.VisibilityMultiplier);
    c.Transfer(d.SpecificCoversBodyAreaFlags);
    c.Transfer(d.CustomFileName);
    c.Transfer(d.CustomFileNameBase);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    d.RemovesAllTattoos = true;
    d.Permanent = true;
    Context save(0); Persist(d, save);
    const void* want[15] = { &d.TattooName, &d.TextureLayer, &d.ReplacingTextureIndex, &d.BankIndex, &d.Blend, &d.RemovesAllTattoos, &d.Permanent, &d.CoversBodyAreaFlags, &d.Scariness, &d.Attractiveness, &d.Goodstrength, &d.VisibilityMultiplier, &d.SpecificCoversBodyAreaFlags, &d.CustomFileName, &d.CustomFileNameBase };
    if (save.count != 15u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 15u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.TattooName, 0, sizeof(e.TattooName));
    std::memset(&e.TextureLayer, 0, sizeof(e.TextureLayer));
    std::memset(&e.ReplacingTextureIndex, 0, sizeof(e.ReplacingTextureIndex));
    std::memset(&e.BankIndex, 0, sizeof(e.BankIndex));
    std::memset(&e.Blend, 0, sizeof(e.Blend));
    std::memset(&e.RemovesAllTattoos, 0, sizeof(e.RemovesAllTattoos));
    std::memset(&e.Permanent, 0, sizeof(e.Permanent));
    std::memset(&e.CoversBodyAreaFlags, 0, sizeof(e.CoversBodyAreaFlags));
    std::memset(&e.Scariness, 0, sizeof(e.Scariness));
    std::memset(&e.Attractiveness, 0, sizeof(e.Attractiveness));
    std::memset(&e.Goodstrength, 0, sizeof(e.Goodstrength));
    std::memset(&e.VisibilityMultiplier, 0, sizeof(e.VisibilityMultiplier));
    std::memset(&e.SpecificCoversBodyAreaFlags, 0, sizeof(e.SpecificCoversBodyAreaFlags));
    std::memset(&e.CustomFileName, 0, sizeof(e.CustomFileName));
    std::memset(&e.CustomFileNameBase, 0, sizeof(e.CustomFileNameBase));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("004529ae_TEST FAIL %d\n", fail); return 1; }
    std::printf("004529ae_TEST PASS\n");
    return 0;
}