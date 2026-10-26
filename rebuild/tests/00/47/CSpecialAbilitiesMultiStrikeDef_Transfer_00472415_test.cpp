// behaviour test for 0x00472415 CSpecialAbilitiesMultiStrikeDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x50, +0x54, +0x58, +0x5C, +0x60, +0x6C, +0x78, +0x84, +0x90),
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
    Blob4 Sounds[4];
    Blob4 Animations[4];
    float HighlightWidth;
    Blob12 HighlightColor;
    Blob12 DelayTimeSecs;
    Blob12 NumHits;
    Blob12 PerHitDamagerMultiplier;
    Blob12 AttachmentPoint;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.Sounds[0]);
    c.Transfer(d.Sounds[1]);
    c.Transfer(d.Sounds[2]);
    c.Transfer(d.Sounds[3]);
    c.Transfer(d.Animations[0]);
    c.Transfer(d.Animations[1]);
    c.Transfer(d.Animations[2]);
    c.Transfer(d.Animations[3]);
    c.Transfer(d.HighlightWidth);
    c.Transfer(d.HighlightColor);
    c.Transfer(d.DelayTimeSecs);
    c.Transfer(d.NumHits);
    c.Transfer(d.PerHitDamagerMultiplier);
    c.Transfer(d.AttachmentPoint);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[14] = { &d.Sounds[0], &d.Sounds[1], &d.Sounds[2], &d.Sounds[3], &d.Animations[0], &d.Animations[1], &d.Animations[2], &d.Animations[3], &d.HighlightWidth, &d.HighlightColor, &d.DelayTimeSecs, &d.NumHits, &d.PerHitDamagerMultiplier, &d.AttachmentPoint };
    if (save.count != 14u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 14u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.Sounds[0], 0, sizeof(e.Sounds[0]));
    std::memset(&e.Sounds[1], 0, sizeof(e.Sounds[1]));
    std::memset(&e.Sounds[2], 0, sizeof(e.Sounds[2]));
    std::memset(&e.Sounds[3], 0, sizeof(e.Sounds[3]));
    std::memset(&e.Animations[0], 0, sizeof(e.Animations[0]));
    std::memset(&e.Animations[1], 0, sizeof(e.Animations[1]));
    std::memset(&e.Animations[2], 0, sizeof(e.Animations[2]));
    std::memset(&e.Animations[3], 0, sizeof(e.Animations[3]));
    std::memset(&e.HighlightWidth, 0, sizeof(e.HighlightWidth));
    std::memset(&e.HighlightColor, 0, sizeof(e.HighlightColor));
    std::memset(&e.DelayTimeSecs, 0, sizeof(e.DelayTimeSecs));
    std::memset(&e.NumHits, 0, sizeof(e.NumHits));
    std::memset(&e.PerHitDamagerMultiplier, 0, sizeof(e.PerHitDamagerMultiplier));
    std::memset(&e.AttachmentPoint, 0, sizeof(e.AttachmentPoint));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("00472415_TEST FAIL %d\n", fail); return 1; }
    std::printf("00472415_TEST PASS\n");
    return 0;
}