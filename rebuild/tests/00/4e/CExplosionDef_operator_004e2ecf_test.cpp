// behaviour test for CExplosionDef::operator= (0x004E2ECF): every member copied, base assignment called, *this returned
#include <cstdio>
static int g_base = 0, g_ops = 0;
class CDefClassBase { public: unsigned char _body[0x25]; CDefClassBase& operator=(const CDefClassBase& o) { ++g_base; for (int i = 0; i < 0x25; ++i) _body[i] = o._body[i]; return *this; } };
struct CExplosionRingVector { unsigned long m_words[3]; CExplosionRingVector& operator=(const CExplosionRingVector& o) { ++g_ops; for (int i = 0; i < 3; ++i) m_words[i] = o.m_words[i]; return *this; } };
class CExplosionDef : public CDefClassBase { public:
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
    CExplosionRingVector ExplosionRings;
    bool ExplosionShouldKnockdown;
    bool CauseFireEffectOnHitCreatures;
    bool CausesRumble;
    long RumbleStrength;
    long RumbleLength;
    CExplosionDef& operator=(const CExplosionDef& other);
};
CExplosionDef& CExplosionDef::operator=(const CExplosionDef& other)
{
    CDefClassBase::operator=(other);
    Damage = other.Damage;
    ExplosionRadius = other.ExplosionRadius;
    FireDamage = other.FireDamage;
    ExplosionLifeTime = other.ExplosionLifeTime;
    DamageFalloffWithRadius = other.DamageFalloffWithRadius;
    ExplosionBlendSmall = other.ExplosionBlendSmall;
    ExplosionBlendLarge = other.ExplosionBlendLarge;
    ExplosionReplacementBlendSmall = other.ExplosionReplacementBlendSmall;
    ExplosionReplacementBlendLarge = other.ExplosionReplacementBlendLarge;
    SecondsBeforeEffectReplacement = other.SecondsBeforeEffectReplacement;
    ExplosionRadiusSmall = other.ExplosionRadiusSmall;
    ExplosionRadiusLarge = other.ExplosionRadiusLarge;
    ExplosionRings = other.ExplosionRings;
    ExplosionShouldKnockdown = other.ExplosionShouldKnockdown;
    CauseFireEffectOnHitCreatures = other.CauseFireEffectOnHitCreatures;
    CausesRumble = other.CausesRumble;
    RumbleStrength = other.RumbleStrength;
    RumbleLength = other.RumbleLength;
    return *this;
}
int main()
{
    CExplosionDef a, b;
    unsigned char* pa = (unsigned char*)&a; unsigned char* pb = (unsigned char*)&b;
    for (unsigned int i = 0; i < sizeof(a); ++i) { pa[i] = (unsigned char)(i * 7 + 3); pb[i] = 0; }
    a.DamageFalloffWithRadius = true;
    a.ExplosionShouldKnockdown = true;
    a.CauseFireEffectOnHitCreatures = true;
    a.CausesRumble = true;
    CExplosionDef* r = &(b = a);
    int bad = (r != &b) || g_base != 1;
    { const unsigned char* x = (const unsigned char*)&a.Damage; const unsigned char* y = (const unsigned char*)&b.Damage; for (unsigned int i = 0; i < sizeof(a.Damage); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.ExplosionRadius; const unsigned char* y = (const unsigned char*)&b.ExplosionRadius; for (unsigned int i = 0; i < sizeof(a.ExplosionRadius); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.FireDamage; const unsigned char* y = (const unsigned char*)&b.FireDamage; for (unsigned int i = 0; i < sizeof(a.FireDamage); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.ExplosionLifeTime; const unsigned char* y = (const unsigned char*)&b.ExplosionLifeTime; for (unsigned int i = 0; i < sizeof(a.ExplosionLifeTime); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.DamageFalloffWithRadius; const unsigned char* y = (const unsigned char*)&b.DamageFalloffWithRadius; for (unsigned int i = 0; i < sizeof(a.DamageFalloffWithRadius); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.ExplosionBlendSmall; const unsigned char* y = (const unsigned char*)&b.ExplosionBlendSmall; for (unsigned int i = 0; i < sizeof(a.ExplosionBlendSmall); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.ExplosionBlendLarge; const unsigned char* y = (const unsigned char*)&b.ExplosionBlendLarge; for (unsigned int i = 0; i < sizeof(a.ExplosionBlendLarge); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.ExplosionReplacementBlendSmall; const unsigned char* y = (const unsigned char*)&b.ExplosionReplacementBlendSmall; for (unsigned int i = 0; i < sizeof(a.ExplosionReplacementBlendSmall); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.ExplosionReplacementBlendLarge; const unsigned char* y = (const unsigned char*)&b.ExplosionReplacementBlendLarge; for (unsigned int i = 0; i < sizeof(a.ExplosionReplacementBlendLarge); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SecondsBeforeEffectReplacement; const unsigned char* y = (const unsigned char*)&b.SecondsBeforeEffectReplacement; for (unsigned int i = 0; i < sizeof(a.SecondsBeforeEffectReplacement); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.ExplosionRadiusSmall; const unsigned char* y = (const unsigned char*)&b.ExplosionRadiusSmall; for (unsigned int i = 0; i < sizeof(a.ExplosionRadiusSmall); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.ExplosionRadiusLarge; const unsigned char* y = (const unsigned char*)&b.ExplosionRadiusLarge; for (unsigned int i = 0; i < sizeof(a.ExplosionRadiusLarge); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.ExplosionRings; const unsigned char* y = (const unsigned char*)&b.ExplosionRings; for (unsigned int i = 0; i < sizeof(a.ExplosionRings); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.ExplosionShouldKnockdown; const unsigned char* y = (const unsigned char*)&b.ExplosionShouldKnockdown; for (unsigned int i = 0; i < sizeof(a.ExplosionShouldKnockdown); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.CauseFireEffectOnHitCreatures; const unsigned char* y = (const unsigned char*)&b.CauseFireEffectOnHitCreatures; for (unsigned int i = 0; i < sizeof(a.CauseFireEffectOnHitCreatures); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.CausesRumble; const unsigned char* y = (const unsigned char*)&b.CausesRumble; for (unsigned int i = 0; i < sizeof(a.CausesRumble); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.RumbleStrength; const unsigned char* y = (const unsigned char*)&b.RumbleStrength; for (unsigned int i = 0; i < sizeof(a.RumbleStrength); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.RumbleLength; const unsigned char* y = (const unsigned char*)&b.RumbleLength; for (unsigned int i = 0; i < sizeof(a.RumbleLength); ++i) if (x[i] != y[i]) bad = 1; }
    for (int i = 0; i < 0x25; ++i) if (a._body[i] != b._body[i]) bad = 1;
    if (bad) { std::printf("004e2ecf_TEST FAIL\n"); return 1; }
    std::printf("004e2ecf_TEST PASS\n");
    return 0;
}