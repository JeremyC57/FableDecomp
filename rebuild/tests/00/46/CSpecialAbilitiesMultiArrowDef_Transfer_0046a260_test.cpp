// behaviour test for 0x0046A260 CSpecialAbilitiesMultiArrowDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x50, +0x54, +0x58, +0x5C, +0x64, +0x68, +0x74, +0x60),
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
    long NumberOfShots[4];
    long NumberInVolley[4];
    float AmmoEffectCreationDelaySecs;
    Blob4 CastSound;
    float BowHighlightWidth;
    Blob12 BowHighlightColor;
    Blob12 MultiArrowPerArrowDamageMultiplier;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.NumberOfShots[0]);
    c.Transfer(d.NumberOfShots[1]);
    c.Transfer(d.NumberOfShots[2]);
    c.Transfer(d.NumberOfShots[3]);
    c.Transfer(d.NumberInVolley[0]);
    c.Transfer(d.NumberInVolley[1]);
    c.Transfer(d.NumberInVolley[2]);
    c.Transfer(d.NumberInVolley[3]);
    c.Transfer(d.AmmoEffectCreationDelaySecs);
    c.Transfer(d.BowHighlightWidth);
    c.Transfer(d.BowHighlightColor);
    c.Transfer(d.MultiArrowPerArrowDamageMultiplier);
    c.Transfer(d.CastSound);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[13] = { &d.NumberOfShots[0], &d.NumberOfShots[1], &d.NumberOfShots[2], &d.NumberOfShots[3], &d.NumberInVolley[0], &d.NumberInVolley[1], &d.NumberInVolley[2], &d.NumberInVolley[3], &d.AmmoEffectCreationDelaySecs, &d.BowHighlightWidth, &d.BowHighlightColor, &d.MultiArrowPerArrowDamageMultiplier, &d.CastSound };
    if (save.count != 13u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 13u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.NumberOfShots[0], 0, sizeof(e.NumberOfShots[0]));
    std::memset(&e.NumberOfShots[1], 0, sizeof(e.NumberOfShots[1]));
    std::memset(&e.NumberOfShots[2], 0, sizeof(e.NumberOfShots[2]));
    std::memset(&e.NumberOfShots[3], 0, sizeof(e.NumberOfShots[3]));
    std::memset(&e.NumberInVolley[0], 0, sizeof(e.NumberInVolley[0]));
    std::memset(&e.NumberInVolley[1], 0, sizeof(e.NumberInVolley[1]));
    std::memset(&e.NumberInVolley[2], 0, sizeof(e.NumberInVolley[2]));
    std::memset(&e.NumberInVolley[3], 0, sizeof(e.NumberInVolley[3]));
    std::memset(&e.AmmoEffectCreationDelaySecs, 0, sizeof(e.AmmoEffectCreationDelaySecs));
    std::memset(&e.BowHighlightWidth, 0, sizeof(e.BowHighlightWidth));
    std::memset(&e.BowHighlightColor, 0, sizeof(e.BowHighlightColor));
    std::memset(&e.MultiArrowPerArrowDamageMultiplier, 0, sizeof(e.MultiArrowPerArrowDamageMultiplier));
    std::memset(&e.CastSound, 0, sizeof(e.CastSound));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("0046a260_TEST FAIL %d\n", fail); return 1; }
    std::printf("0046a260_TEST PASS\n");
    return 0;
}