// behaviour test for 0x00457F91 CSpecialAbilitiesThunderLightningStormDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x50, +0x54, +0x58, +0x5C, +0x60, +0x64),
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
    long NumBeams;
    float StartRadius;
    float EndRadius;
    float SkyBeamStartTime;
    float MainBeamsStartTime;
    float BeamRotationStartSpeed;
    float BeamRotationEndSpeed;
    float TotalLifetime;
    float Damage;
    Blob4 InitSound;
    Blob4 LoopingSound;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.NumBeams);
    c.Transfer(d.StartRadius);
    c.Transfer(d.EndRadius);
    c.Transfer(d.SkyBeamStartTime);
    c.Transfer(d.MainBeamsStartTime);
    c.Transfer(d.BeamRotationStartSpeed);
    c.Transfer(d.BeamRotationEndSpeed);
    c.Transfer(d.TotalLifetime);
    c.Transfer(d.Damage);
    c.Transfer(d.InitSound);
    c.Transfer(d.LoopingSound);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[11] = { &d.NumBeams, &d.StartRadius, &d.EndRadius, &d.SkyBeamStartTime, &d.MainBeamsStartTime, &d.BeamRotationStartSpeed, &d.BeamRotationEndSpeed, &d.TotalLifetime, &d.Damage, &d.InitSound, &d.LoopingSound };
    if (save.count != 11u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 11u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.NumBeams, 0, sizeof(e.NumBeams));
    std::memset(&e.StartRadius, 0, sizeof(e.StartRadius));
    std::memset(&e.EndRadius, 0, sizeof(e.EndRadius));
    std::memset(&e.SkyBeamStartTime, 0, sizeof(e.SkyBeamStartTime));
    std::memset(&e.MainBeamsStartTime, 0, sizeof(e.MainBeamsStartTime));
    std::memset(&e.BeamRotationStartSpeed, 0, sizeof(e.BeamRotationStartSpeed));
    std::memset(&e.BeamRotationEndSpeed, 0, sizeof(e.BeamRotationEndSpeed));
    std::memset(&e.TotalLifetime, 0, sizeof(e.TotalLifetime));
    std::memset(&e.Damage, 0, sizeof(e.Damage));
    std::memset(&e.InitSound, 0, sizeof(e.InitSound));
    std::memset(&e.LoopingSound, 0, sizeof(e.LoopingSound));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("00457f91_TEST FAIL %d\n", fail); return 1; }
    std::printf("00457f91_TEST PASS\n");
    return 0;
}