// behaviour test for 0x0045C112 CMaterialDef::operator=
#include <cstdio>
static int g_copies = 0;
struct CParentDefClassBase {
    int baseTag;
    CParentDefClassBase& operator=(const CParentDefClassBase& o) { baseTag = o.baseTag; return *this; }
};
struct CLongVector { long v; CLongVector& operator=(const CLongVector& o) { v = o.v; ++g_copies; return *this; } };
struct CBlendedEffectMap { long v; CBlendedEffectMap& operator=(const CBlendedEffectMap& o) { v = o.v; g_copies += 10; return *this; } };
struct CMaterialDef : CParentDefClassBase {
    long FirstHitSound, LastHitSound; float OnHitRumbleLevel, OnHitRumbleFalloff; bool DeflectAllHits, IsDiggable;
    CLongVector SlideFootParticleEffect, SwordHitParticleEffect, KnockdownHitFloorParticleEffect, WindBlowDustEffect, BerserkedSwordHitParticleEffect;
    CBlendedEffectMap BlendedSwordHitEffects;
    CMaterialDef& operator=(const CMaterialDef& other)
    {
        CParentDefClassBase::operator=(other);
        FirstHitSound = other.FirstHitSound;
        LastHitSound = other.LastHitSound;
        OnHitRumbleLevel = other.OnHitRumbleLevel;
        OnHitRumbleFalloff = other.OnHitRumbleFalloff;
        DeflectAllHits = other.DeflectAllHits;
        IsDiggable = other.IsDiggable;
        SlideFootParticleEffect = other.SlideFootParticleEffect;
        SwordHitParticleEffect = other.SwordHitParticleEffect;
        KnockdownHitFloorParticleEffect = other.KnockdownHitFloorParticleEffect;
        WindBlowDustEffect = other.WindBlowDustEffect;
        BerserkedSwordHitParticleEffect = other.BerserkedSwordHitParticleEffect;
        BlendedSwordHitEffects = other.BlendedSwordHitEffects;
        return *this;
    }
};
int main()
{
    CMaterialDef a, b;
    b.baseTag = 9; b.FirstHitSound = 10; b.LastHitSound = 20; b.OnHitRumbleLevel = 0.75f; b.IsDiggable = true;
    b.SwordHitParticleEffect.v = 3; b.BlendedSwordHitEffects.v = 4;
    a.baseTag = 0; a.FirstHitSound = 0; a.LastHitSound = 0; a.OnHitRumbleLevel = 0; a.IsDiggable = false;
    a.SwordHitParticleEffect.v = 0; a.BlendedSwordHitEffects.v = 0;
    CMaterialDef* r = &(a = b);
    if (r != &a || a.baseTag != 9 || a.FirstHitSound != 10 || a.LastHitSound != 20 || a.OnHitRumbleLevel != 0.75f
        || !a.IsDiggable || a.SwordHitParticleEffect.v != 3 || a.BlendedSwordHitEffects.v != 4 || g_copies != 15)
    { std::printf("0045c112_TEST FAIL\n"); return 1; }
    std::printf("0045c112_TEST PASS\n");
    return 0;
}
