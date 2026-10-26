// behaviour test for 0x004EB02F CExplosionDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x2C, +0x30, +0x34, +0x38, +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x50, +0x54, +0x58, +0x64, +0x65, +0x66, +0x68, +0x6C),
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
    float Damage;
    float ExplosionRadius;
    long FireDamage;
    float ExplosionLifeTime;
    bool DamageFalloffWithRadius;
    long ExplosionBlendSmall;
    long ExplosionBlendLarge;
    long ExplosionReplacementBlendSmall;
    long ExplosionReplacementBlendLarge;
    float SecondsBeforeEffectReplacement;
    float ExplosionRadiusSmall;
    float ExplosionRadiusLarge;
    Blob12 ExplosionRings;
    bool ExplosionShouldKnockdown;
    bool CauseFireEffectOnHitCreatures;
    bool CausesRumble;
    int RumbleStrength;
    int RumbleLength;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.Damage);
    c.Transfer(d.ExplosionRadius);
    c.Transfer(d.FireDamage);
    c.Transfer(d.ExplosionLifeTime);
    c.Transfer(d.DamageFalloffWithRadius);
    c.Transfer(d.ExplosionBlendSmall);
    c.Transfer(d.ExplosionBlendLarge);
    c.Transfer(d.ExplosionReplacementBlendSmall);
    c.Transfer(d.ExplosionReplacementBlendLarge);
    c.Transfer(d.SecondsBeforeEffectReplacement);
    c.Transfer(d.ExplosionRadiusSmall);
    c.Transfer(d.ExplosionRadiusLarge);
    c.Transfer(d.ExplosionRings);
    c.Transfer(d.ExplosionShouldKnockdown);
    c.Transfer(d.CauseFireEffectOnHitCreatures);
    c.Transfer(d.CausesRumble);
    c.Transfer(d.RumbleStrength);
    c.Transfer(d.RumbleLength);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    d.DamageFalloffWithRadius = true;
    d.ExplosionShouldKnockdown = true;
    d.CauseFireEffectOnHitCreatures = true;
    d.CausesRumble = true;
    Context save(0); Persist(d, save);
    const void* want[18] = { &d.Damage, &d.ExplosionRadius, &d.FireDamage, &d.ExplosionLifeTime, &d.DamageFalloffWithRadius, &d.ExplosionBlendSmall, &d.ExplosionBlendLarge, &d.ExplosionReplacementBlendSmall, &d.ExplosionReplacementBlendLarge, &d.SecondsBeforeEffectReplacement, &d.ExplosionRadiusSmall, &d.ExplosionRadiusLarge, &d.ExplosionRings, &d.ExplosionShouldKnockdown, &d.CauseFireEffectOnHitCreatures, &d.CausesRumble, &d.RumbleStrength, &d.RumbleLength };
    if (save.count != 18u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 18u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.Damage, 0, sizeof(e.Damage));
    std::memset(&e.ExplosionRadius, 0, sizeof(e.ExplosionRadius));
    std::memset(&e.FireDamage, 0, sizeof(e.FireDamage));
    std::memset(&e.ExplosionLifeTime, 0, sizeof(e.ExplosionLifeTime));
    std::memset(&e.DamageFalloffWithRadius, 0, sizeof(e.DamageFalloffWithRadius));
    std::memset(&e.ExplosionBlendSmall, 0, sizeof(e.ExplosionBlendSmall));
    std::memset(&e.ExplosionBlendLarge, 0, sizeof(e.ExplosionBlendLarge));
    std::memset(&e.ExplosionReplacementBlendSmall, 0, sizeof(e.ExplosionReplacementBlendSmall));
    std::memset(&e.ExplosionReplacementBlendLarge, 0, sizeof(e.ExplosionReplacementBlendLarge));
    std::memset(&e.SecondsBeforeEffectReplacement, 0, sizeof(e.SecondsBeforeEffectReplacement));
    std::memset(&e.ExplosionRadiusSmall, 0, sizeof(e.ExplosionRadiusSmall));
    std::memset(&e.ExplosionRadiusLarge, 0, sizeof(e.ExplosionRadiusLarge));
    std::memset(&e.ExplosionRings, 0, sizeof(e.ExplosionRings));
    std::memset(&e.ExplosionShouldKnockdown, 0, sizeof(e.ExplosionShouldKnockdown));
    std::memset(&e.CauseFireEffectOnHitCreatures, 0, sizeof(e.CauseFireEffectOnHitCreatures));
    std::memset(&e.CausesRumble, 0, sizeof(e.CausesRumble));
    std::memset(&e.RumbleStrength, 0, sizeof(e.RumbleStrength));
    std::memset(&e.RumbleLength, 0, sizeof(e.RumbleLength));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("004eb02f_TEST FAIL %d\n", fail); return 1; }
    std::printf("004eb02f_TEST PASS\n");
    return 0;
}