// CThingShotDef::operator=(const CThingShotDef&) -- memberwise copy: CParentDefClassBase part, speed, effects/sounds, damage, offset ranges, explosion/object defs, deflection/bounce flags and sub-shot settings; returns *this
#pragma optimize("s", on)
// Leaf: operator=
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046267D, length 351 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax/al).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of CThingShotDef's
// vtable 0x01234E2C is the 5-byte `jmp 0x0046267D` right before this body.
// Layout: PDB CThingShotDef (0xB0) members +0x40.. sit at retail +0x3c.. (retail CParentDefClassBase 0x3c bytes).
// Shape: memberwise assignment in declaration order; CDefString is a 4-byte handle copied as a dword, bools as
// byte moves, each CFloatRange as two dwords.
#pragma pack(push, 1)
class CParentDefClassBase { public:
    virtual void v00();
    unsigned char _defClassBase[0x38];      // +0x04 CDefClassBase part, InstantiationName, SubDefInfoMap
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

class CDefString { public:                         // 4-byte def-string handle (trivially copyable)
    long m_index;
};

class CFloatRange { public:
    float Min;
    float Max;
};

class CThingShotDef : public CParentDefClassBase { public:    // vtable 0x01234E2C
    float       Speed;                              // +0x3c
    long        PrimaryEffect;                      // +0x40
    long        PrimaryEffectSound;                 // +0x44
    long        AttachedContinuousEffect;           // +0x48
    long        AttachedContinuousSound;            // +0x4c
    bool        UseEndTimeLineOnContinousEffect;    // +0x50
    float       HitThingDamage;                     // +0x54
    float       HitThingShieldDamage;               // +0x58
    bool        CauseDamage;                        // +0x5c
    CDefString  ShotDefinitionClassName;            // +0x60
    CFloatRange ShotOffsetAngleRange;               // +0x64
    CFloatRange ShotOffsetRadiusRange;              // +0x6c
    long        ExplosionDef;                       // +0x74
    long        GroundExplosionDef;                 // +0x78
    long        ObjectDef;                          // +0x7c
    bool        LeavesObjectBehind;                 // +0x80
    bool        IsHittableByMelee;                  // +0x81
    bool        IsDeflectedOnHitByMelee;            // +0x82
    float       MeleeHitDeflectionDamageMultiplier; // +0x84
    bool        FaceMovementDirection;              // +0x88
    bool        AllowDeflections;                   // +0x89
    bool        BounceOnGround;                     // +0x8a
    float       Bounciness;                         // +0x8c
    bool        RotateOnDeflection;                 // +0x90
    bool        IsBlockable;                        // +0x91
    long        DamageType;                         // +0x94 EDamageAttribute
    long        NumSubShots;                        // +0x98
    long        SubShotType;                        // +0x9c
    float       SubShotCreationTime;                // +0xa0
    float       SubShotMaxAngle;                    // +0xa4
    bool        RemoveMainShotOnSubShotCreation;    // +0xa8
    bool        DoesExplosionDamageCaster;          // +0xa9
    bool        CanHitRollingTargets;               // +0xaa
    bool        PreventSlidingOnCollision;          // +0xab
    CThingShotDef& operator=(const CThingShotDef& other);
};

CThingShotDef& CThingShotDef::operator=(const CThingShotDef& other)
{
    CParentDefClassBase::operator=(other);
    Speed = other.Speed;
    PrimaryEffect = other.PrimaryEffect;
    PrimaryEffectSound = other.PrimaryEffectSound;
    AttachedContinuousEffect = other.AttachedContinuousEffect;
    AttachedContinuousSound = other.AttachedContinuousSound;
    UseEndTimeLineOnContinousEffect = other.UseEndTimeLineOnContinousEffect;
    HitThingDamage = other.HitThingDamage;
    HitThingShieldDamage = other.HitThingShieldDamage;
    CauseDamage = other.CauseDamage;
    ShotDefinitionClassName = other.ShotDefinitionClassName;
    ShotOffsetAngleRange = other.ShotOffsetAngleRange;
    ShotOffsetRadiusRange = other.ShotOffsetRadiusRange;
    ExplosionDef = other.ExplosionDef;
    GroundExplosionDef = other.GroundExplosionDef;
    ObjectDef = other.ObjectDef;
    LeavesObjectBehind = other.LeavesObjectBehind;
    IsHittableByMelee = other.IsHittableByMelee;
    IsDeflectedOnHitByMelee = other.IsDeflectedOnHitByMelee;
    MeleeHitDeflectionDamageMultiplier = other.MeleeHitDeflectionDamageMultiplier;
    FaceMovementDirection = other.FaceMovementDirection;
    AllowDeflections = other.AllowDeflections;
    BounceOnGround = other.BounceOnGround;
    Bounciness = other.Bounciness;
    RotateOnDeflection = other.RotateOnDeflection;
    IsBlockable = other.IsBlockable;
    DamageType = other.DamageType;
    NumSubShots = other.NumSubShots;
    SubShotType = other.SubShotType;
    SubShotCreationTime = other.SubShotCreationTime;
    SubShotMaxAngle = other.SubShotMaxAngle;
    RemoveMainShotOnSubShotCreation = other.RemoveMainShotOnSubShotCreation;
    DoesExplosionDamageCaster = other.DoesExplosionDamageCaster;
    CanHitRollingTargets = other.CanHitRollingTargets;
    PreventSlidingOnCollision = other.PreventSlidingOnCollision;
    return *this;
}