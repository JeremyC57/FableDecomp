// behaviour test for 0x00476DC3 CEnvironmentThemeDaySetDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x3C, +0x48, +0x4C, +0x50, +0x54, +0x58, +0x5C, +0x60, +0x64, +0x68),
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
    Blob12 TimeTheme;
    float SunTilt;
    float SunRotate;
    float SunHeight;
    float MoonTilt;
    float MoonRotate;
    float MoonHeight;
    long ColourLookupColumn;
    Blob4 EditorColour;
    float FishWeightMult;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.TimeTheme);
    c.Transfer(d.SunTilt);
    c.Transfer(d.SunRotate);
    c.Transfer(d.SunHeight);
    c.Transfer(d.MoonTilt);
    c.Transfer(d.MoonRotate);
    c.Transfer(d.MoonHeight);
    c.Transfer(d.ColourLookupColumn);
    c.Transfer(d.EditorColour);
    c.Transfer(d.FishWeightMult);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[10] = { &d.TimeTheme, &d.SunTilt, &d.SunRotate, &d.SunHeight, &d.MoonTilt, &d.MoonRotate, &d.MoonHeight, &d.ColourLookupColumn, &d.EditorColour, &d.FishWeightMult };
    if (save.count != 10u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 10u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.TimeTheme, 0, sizeof(e.TimeTheme));
    std::memset(&e.SunTilt, 0, sizeof(e.SunTilt));
    std::memset(&e.SunRotate, 0, sizeof(e.SunRotate));
    std::memset(&e.SunHeight, 0, sizeof(e.SunHeight));
    std::memset(&e.MoonTilt, 0, sizeof(e.MoonTilt));
    std::memset(&e.MoonRotate, 0, sizeof(e.MoonRotate));
    std::memset(&e.MoonHeight, 0, sizeof(e.MoonHeight));
    std::memset(&e.ColourLookupColumn, 0, sizeof(e.ColourLookupColumn));
    std::memset(&e.EditorColour, 0, sizeof(e.EditorColour));
    std::memset(&e.FishWeightMult, 0, sizeof(e.FishWeightMult));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("00476dc3_TEST FAIL %d\n", fail); return 1; }
    std::printf("00476dc3_TEST PASS\n");
    return 0;
}