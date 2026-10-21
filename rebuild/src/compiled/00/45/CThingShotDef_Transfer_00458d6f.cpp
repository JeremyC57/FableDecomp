// CThingShotDef::Transfer(CPersistContext&) -- persist every shot-definition field in declaration order
// Leaf: ?Transfer@CThingShotDef@@UAEXAAVCPersistContext@@@Z
// Retail 0x00458D6F, 444 bytes, size-optimised def TU (#pragma optimize("s")): unaligned address, no padding.
// One CPersistContext::Transfer overload per member type:
//   0x00431061 Transfer(float&), 0x00431102 Transfer(long&), 0x0043314A Transfer(bool&),
//   0x0045228F Transfer(CDefString&), and two member-template instances emitted right after this body:
//   0x00458F2B Transfer<CFloatRange> and 0x00459042 Transfer<EDamageAttribute>.
//   (Manifest labels on all of these read "UpdateShadowScene": propagated, wrong.)
// Layout: Ego_r.pdb names and types; the TLC def base is 0x3c bytes (PDB 0x40), so retail offset = PDB offset - 4.
//   Local model: no engine/ header exists for CThingShotDef or CPersistContext.
// Shape: VC7.1 forms the last address with `add esi,0ABh ; push esi` because `this` is dead after it.
#pragma optimize("s", on)

struct CDefString { long m_index; };
struct CFloatRange { float Min; float Max; };
enum EDamageAttribute { EDamageAttribute_Force32 = 0x7fffffff };

struct CPersistContext {
    void Transfer(float& v);                 // 0x00431061
    void Transfer(long& v);                  // 0x00431102
    void Transfer(bool& v);                  // 0x0043314A
    void Transfer(CDefString& v);            // 0x0045228F
    template <class T> void Transfer(T& v);  // 0x00458F2B <CFloatRange>, 0x00459042 <EDamageAttribute>
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