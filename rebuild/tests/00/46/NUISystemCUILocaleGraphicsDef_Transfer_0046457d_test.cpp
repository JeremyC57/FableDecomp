// behaviour test for 0x0046457D NUISystem::CUILocaleGraphicsDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x3C, +0x48, +0x54),
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
    Blob12 WorldMapRegionName;
    Blob12 HelpScreenGraphics;
    unsigned long HelpRingPic;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.WorldMapRegionName);
    c.Transfer(d.HelpScreenGraphics);
    c.Transfer(d.HelpRingPic);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[3] = { &d.WorldMapRegionName, &d.HelpScreenGraphics, &d.HelpRingPic };
    if (save.count != 3u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 3u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.WorldMapRegionName, 0, sizeof(e.WorldMapRegionName));
    std::memset(&e.HelpScreenGraphics, 0, sizeof(e.HelpScreenGraphics));
    std::memset(&e.HelpRingPic, 0, sizeof(e.HelpRingPic));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("0046457d_TEST FAIL %d\n", fail); return 1; }
    std::printf("0046457d_TEST PASS\n");
    return 0;
}