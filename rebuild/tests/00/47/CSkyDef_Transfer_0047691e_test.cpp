// behaviour test for 0x0047691E CSkyDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x3C, +0x40, +0x4C, +0x50, +0x44, +0x48, +0x54, +0x58, +0x5C, +0x60, +0x64, +0x68, +0x6C, +0x70, +0x74),
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
    long SunRadius;
    long SunTexture;
    long MoonRadius;
    long MoonTexture;
    long SunFlareRadius;
    long SunFlareTexture;
    long StarTexture;
    long StarChartTextureSize;
    float StarSize;
    long StarChartFilter;
    long TwinkleInterval;
    float TwinkleSpeed;
    float TwinkleMin;
    float TwinkleMax;
    Blob12 FlareElements;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.SunRadius);
    c.Transfer(d.SunTexture);
    c.Transfer(d.SunFlareRadius);
    c.Transfer(d.SunFlareTexture);
    c.Transfer(d.MoonRadius);
    c.Transfer(d.MoonTexture);
    c.Transfer(d.StarTexture);
    c.Transfer(d.StarChartTextureSize);
    c.Transfer(d.StarSize);
    c.Transfer(d.StarChartFilter);
    c.Transfer(d.TwinkleInterval);
    c.Transfer(d.TwinkleSpeed);
    c.Transfer(d.TwinkleMin);
    c.Transfer(d.TwinkleMax);
    c.Transfer(d.FlareElements);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[15] = { &d.SunRadius, &d.SunTexture, &d.SunFlareRadius, &d.SunFlareTexture, &d.MoonRadius, &d.MoonTexture, &d.StarTexture, &d.StarChartTextureSize, &d.StarSize, &d.StarChartFilter, &d.TwinkleInterval, &d.TwinkleSpeed, &d.TwinkleMin, &d.TwinkleMax, &d.FlareElements };
    if (save.count != 15u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 15u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.SunRadius, 0, sizeof(e.SunRadius));
    std::memset(&e.SunTexture, 0, sizeof(e.SunTexture));
    std::memset(&e.SunFlareRadius, 0, sizeof(e.SunFlareRadius));
    std::memset(&e.SunFlareTexture, 0, sizeof(e.SunFlareTexture));
    std::memset(&e.MoonRadius, 0, sizeof(e.MoonRadius));
    std::memset(&e.MoonTexture, 0, sizeof(e.MoonTexture));
    std::memset(&e.StarTexture, 0, sizeof(e.StarTexture));
    std::memset(&e.StarChartTextureSize, 0, sizeof(e.StarChartTextureSize));
    std::memset(&e.StarSize, 0, sizeof(e.StarSize));
    std::memset(&e.StarChartFilter, 0, sizeof(e.StarChartFilter));
    std::memset(&e.TwinkleInterval, 0, sizeof(e.TwinkleInterval));
    std::memset(&e.TwinkleSpeed, 0, sizeof(e.TwinkleSpeed));
    std::memset(&e.TwinkleMin, 0, sizeof(e.TwinkleMin));
    std::memset(&e.TwinkleMax, 0, sizeof(e.TwinkleMax));
    std::memset(&e.FlareElements, 0, sizeof(e.FlareElements));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("0047691e_TEST FAIL %d\n", fail); return 1; }
    std::printf("0047691e_TEST PASS\n");
    return 0;
}