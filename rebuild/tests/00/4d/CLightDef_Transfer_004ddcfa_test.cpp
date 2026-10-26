// behaviour test for 0x004DDCFA CLightDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x2C, +0x30, +0x34, +0x38, +0x3C, +0x40, +0x41, +0x42, +0x54, +0x58, +0x44, +0x48, +0x4C, +0x50),
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
    Blob4 Colour;
    long HaloGraphic;
    float InnerRadius;
    float OuterRadius;
    float Flicker;
    float FlickerSpeed;
    bool StartActive;
    bool SunlightAttenuator;
    bool DaylightAttenuator;
    float DaylightAttenuatorFadeInStart;
    float DaylightAttenuatorFadeInEnd;
    float DaylightAttenuatorFadeOutStart;
    float DaylightAttenuatorFadeOutEnd;
    long EffectChannel;
    bool Inverted;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.Colour);
    c.Transfer(d.HaloGraphic);
    c.Transfer(d.InnerRadius);
    c.Transfer(d.OuterRadius);
    c.Transfer(d.Flicker);
    c.Transfer(d.FlickerSpeed);
    c.Transfer(d.StartActive);
    c.Transfer(d.SunlightAttenuator);
    c.Transfer(d.DaylightAttenuator);
    c.Transfer(d.EffectChannel);
    c.Transfer(d.Inverted);
    c.Transfer(d.DaylightAttenuatorFadeInStart);
    c.Transfer(d.DaylightAttenuatorFadeInEnd);
    c.Transfer(d.DaylightAttenuatorFadeOutStart);
    c.Transfer(d.DaylightAttenuatorFadeOutEnd);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    d.StartActive = true;
    d.SunlightAttenuator = true;
    d.DaylightAttenuator = true;
    d.Inverted = true;
    Context save(0); Persist(d, save);
    const void* want[15] = { &d.Colour, &d.HaloGraphic, &d.InnerRadius, &d.OuterRadius, &d.Flicker, &d.FlickerSpeed, &d.StartActive, &d.SunlightAttenuator, &d.DaylightAttenuator, &d.EffectChannel, &d.Inverted, &d.DaylightAttenuatorFadeInStart, &d.DaylightAttenuatorFadeInEnd, &d.DaylightAttenuatorFadeOutStart, &d.DaylightAttenuatorFadeOutEnd };
    if (save.count != 15u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 15u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.Colour, 0, sizeof(e.Colour));
    std::memset(&e.HaloGraphic, 0, sizeof(e.HaloGraphic));
    std::memset(&e.InnerRadius, 0, sizeof(e.InnerRadius));
    std::memset(&e.OuterRadius, 0, sizeof(e.OuterRadius));
    std::memset(&e.Flicker, 0, sizeof(e.Flicker));
    std::memset(&e.FlickerSpeed, 0, sizeof(e.FlickerSpeed));
    std::memset(&e.StartActive, 0, sizeof(e.StartActive));
    std::memset(&e.SunlightAttenuator, 0, sizeof(e.SunlightAttenuator));
    std::memset(&e.DaylightAttenuator, 0, sizeof(e.DaylightAttenuator));
    std::memset(&e.EffectChannel, 0, sizeof(e.EffectChannel));
    std::memset(&e.Inverted, 0, sizeof(e.Inverted));
    std::memset(&e.DaylightAttenuatorFadeInStart, 0, sizeof(e.DaylightAttenuatorFadeInStart));
    std::memset(&e.DaylightAttenuatorFadeInEnd, 0, sizeof(e.DaylightAttenuatorFadeInEnd));
    std::memset(&e.DaylightAttenuatorFadeOutStart, 0, sizeof(e.DaylightAttenuatorFadeOutStart));
    std::memset(&e.DaylightAttenuatorFadeOutEnd, 0, sizeof(e.DaylightAttenuatorFadeOutEnd));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("004ddcfa_TEST FAIL %d\n", fail); return 1; }
    std::printf("004ddcfa_TEST PASS\n");
    return 0;
}