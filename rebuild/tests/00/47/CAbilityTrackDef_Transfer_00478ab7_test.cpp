// behaviour test for 0x00478AB7 CAbilityTrackDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x3C, +0x40, +0x4C),
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
    long ExperienceStat;
    Blob12 Abilities;
    long MaxedOutDescription;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.ExperienceStat);
    c.Transfer(d.Abilities);
    c.Transfer(d.MaxedOutDescription);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[3] = { &d.ExperienceStat, &d.Abilities, &d.MaxedOutDescription };
    if (save.count != 3u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 3u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.ExperienceStat, 0, sizeof(e.ExperienceStat));
    std::memset(&e.Abilities, 0, sizeof(e.Abilities));
    std::memset(&e.MaxedOutDescription, 0, sizeof(e.MaxedOutDescription));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("00478ab7_TEST FAIL %d\n", fail); return 1; }
    std::printf("00478ab7_TEST PASS\n");
    return 0;
}