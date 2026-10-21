#include <cstdio>
// behaviour test: CThingShotDef::Transfer visits every field once, in retail order, through the overload of its type

struct CDefString { long m_index; };
struct CFloatRange { float Min; float Max; };
enum EDamageAttribute { EDamageAttribute_Force32 = 0x7fffffff };

struct CPersistContext {
    const void* order[64];
    int kind[64];
    int count;
    void Transfer(CDefString& v) { kind[count] = 1; order[count++] = &v; }
    void Transfer(CFloatRange& v) { kind[count] = 2; order[count++] = &v; }
    void Transfer(EDamageAttribute& v) { kind[count] = 3; order[count++] = &v; }
    void Transfer(bool& v) { kind[count] = 4; order[count++] = &v; }
    void Transfer(float& v) { kind[count] = 5; order[count++] = &v; }
    void Transfer(long& v) { kind[count] = 6; order[count++] = &v; }
};

struct CThingShotDef {
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x38];                    // CDefClassBase fields (+0x04)
    float Speed;                                  // +0x3c
    long PrimaryEffect;                           // +0x40
    long PrimaryEffectSound;                      // +0x44
    long AttachedContinuousEffect;                // +0x48
    long AttachedContinuousSound;                 // +0x4c
    bool UseEndTimeLineOnContinousEffect;         // +0x50
    float HitThingDamage;                         // +0x54
    float HitThingShieldDamage;                   // +0x58
    bool CauseDamage;                             // +0x5c
    CDefString ShotDefinitionClassName;           // +0x60
    CFloatRange ShotOffsetAngleRange;             // +0x64
    CFloatRange ShotOffsetRadiusRange;            // +0x6c
    long ExplosionDef;                            // +0x74
    long GroundExplosionDef;                      // +0x78
    long ObjectDef;                               // +0x7c
    bool LeavesObjectBehind;                      // +0x80
    bool IsHittableByMelee;                       // +0x81
    bool IsDeflectedOnHitByMelee;                 // +0x82
    float MeleeHitDeflectionDamageMultiplier;     // +0x84
    bool FaceMovementDirection;                   // +0x88
    bool AllowDeflections;                        // +0x89
    bool BounceOnGround;                          // +0x8a
    float Bounciness;                             // +0x8c
    bool RotateOnDeflection;                      // +0x90
    bool IsBlockable;                             // +0x91
    EDamageAttribute DamageType;                  // +0x94
    long NumSubShots;                             // +0x98
    long SubShotType;                             // +0x9c
    float SubShotCreationTime;                    // +0xa0
    float SubShotMaxAngle;                        // +0xa4
    bool RemoveMainShotOnSubShotCreation;         // +0xa8
    bool DoesExplosionDamageCaster;               // +0xa9
    bool CanHitRollingTargets;                    // +0xaa
    bool PreventSlidingOnCollision;               // +0xab
};

void CThingShotDef::Transfer(CPersistContext& context)
{
    context.Transfer(Speed);
    context.Transfer(PrimaryEffect);
    context.Transfer(PrimaryEffectSound);
    context.Transfer(AttachedContinuousEffect);
    context.Transfer(AttachedContinuousSound);
    context.Transfer(UseEndTimeLineOnContinousEffect);
    context.Transfer(HitThingDamage);
    context.Transfer(HitThingShieldDamage);
    context.Transfer(CauseDamage);
    context.Transfer(ShotDefinitionClassName);
    context.Transfer(ShotOffsetAngleRange);
    context.Transfer(ShotOffsetRadiusRange);
    context.Transfer(ExplosionDef);
    context.Transfer(GroundExplosionDef);
    context.Transfer(ObjectDef);
    context.Transfer(LeavesObjectBehind);
    context.Transfer(IsHittableByMelee);
    context.Transfer(IsDeflectedOnHitByMelee);
    context.Transfer(MeleeHitDeflectionDamageMultiplier);
    context.Transfer(FaceMovementDirection);
    context.Transfer(AllowDeflections);
    context.Transfer(BounceOnGround);
    context.Transfer(Bounciness);
    context.Transfer(RotateOnDeflection);
    context.Transfer(IsBlockable);
    context.Transfer(DamageType);
    context.Transfer(NumSubShots);
    context.Transfer(SubShotType);
    context.Transfer(SubShotCreationTime);
    context.Transfer(SubShotMaxAngle);
    context.Transfer(RemoveMainShotOnSubShotCreation);
    context.Transfer(DoesExplosionDamageCaster);
    context.Transfer(CanHitRollingTargets);
    context.Transfer(PreventSlidingOnCollision);
}

int main()
{
    static CThingShotDef def;
    CPersistContext ctx; ctx.count = 0;
    def.Transfer(ctx);
    const void* expect[34] = { &def.Speed, &def.PrimaryEffect, &def.PrimaryEffectSound, &def.AttachedContinuousEffect, &def.AttachedContinuousSound, &def.UseEndTimeLineOnContinousEffect, &def.HitThingDamage, &def.HitThingShieldDamage, &def.CauseDamage, &def.ShotDefinitionClassName, &def.ShotOffsetAngleRange, &def.ShotOffsetRadiusRange, &def.ExplosionDef, &def.GroundExplosionDef, &def.ObjectDef, &def.LeavesObjectBehind, &def.IsHittableByMelee, &def.IsDeflectedOnHitByMelee, &def.MeleeHitDeflectionDamageMultiplier, &def.FaceMovementDirection, &def.AllowDeflections, &def.BounceOnGround, &def.Bounciness, &def.RotateOnDeflection, &def.IsBlockable, &def.DamageType, &def.NumSubShots, &def.SubShotType, &def.SubShotCreationTime, &def.SubShotMaxAngle, &def.RemoveMainShotOnSubShotCreation, &def.DoesExplosionDamageCaster, &def.CanHitRollingTargets, &def.PreventSlidingOnCollision };
    const int kinds[34] = { 5, 6, 6, 6, 6, 4, 5, 5, 4, 1, 2, 2, 6, 6, 6, 4, 4, 4, 5, 4, 4, 4, 5, 4, 4, 3, 6, 6, 5, 5, 4, 4, 4, 4 };
    bool ok = ctx.count == 34;
    for (int i = 0; ok && i < 34; ++i)
        ok = ctx.order[i] == expect[i] && ctx.kind[i] == kinds[i];
    if (!ok) { std::printf("00458d6f_TEST FAIL\n"); return 1; }
    std::printf("00458d6f_TEST PASS\n");
    return 0;
}
