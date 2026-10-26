// Behaviour test for CThingShotDef::operator= @ 0x0046267D: base part and every member are copied; returns *this.
#include <cstdio>
#include <cstring>

static int g_baseCopies = 0;
class CParentDefClassBase { public:
    int Instantiation;
    CParentDefClassBase& operator=(const CParentDefClassBase& o) { Instantiation = o.Instantiation; ++g_baseCopies; return *this; }
};
class CDefString { public: long m_index; };
class CFloatRange { public: float Min, Max; };
class CThingShotDef : public CParentDefClassBase { public:
    float Speed; long PrimaryEffect, PrimaryEffectSound, AttachedContinuousEffect, AttachedContinuousSound;
    bool UseEndTimeLineOnContinousEffect; float HitThingDamage, HitThingShieldDamage; bool CauseDamage;
    CDefString ShotDefinitionClassName; CFloatRange ShotOffsetAngleRange, ShotOffsetRadiusRange;
    long ExplosionDef, GroundExplosionDef, ObjectDef;
    bool LeavesObjectBehind, IsHittableByMelee, IsDeflectedOnHitByMelee; float MeleeHitDeflectionDamageMultiplier;
    bool FaceMovementDirection, AllowDeflections, BounceOnGround; float Bounciness;
    bool RotateOnDeflection, IsBlockable; long DamageType, NumSubShots, SubShotType;
    float SubShotCreationTime, SubShotMaxAngle;
    bool RemoveMainShotOnSubShotCreation, DoesExplosionDamageCaster, CanHitRollingTargets, PreventSlidingOnCollision;
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

int main()
{
    CThingShotDef a, b;
    std::memset(&a, 0, sizeof(a));
    std::memset(&b, 0, sizeof(b));
    b.Instantiation = 3; b.Speed = 10.0f; b.AttachedContinuousSound = 6; b.CauseDamage = true;
    b.ShotOffsetAngleRange.Max = 0.25f; b.ShotOffsetRadiusRange.Min = 2.0f; b.ObjectDef = 44;
    b.IsDeflectedOnHitByMelee = true; b.Bounciness = 0.75f; b.IsBlockable = true; b.SubShotMaxAngle = 30.0f;
    b.PreventSlidingOnCollision = true;
    CThingShotDef& r = (a = b);
    int ok = &r == &a && g_baseCopies == 1 && a.Instantiation == 3 && a.Speed == 10.0f && a.AttachedContinuousSound == 6
          && a.CauseDamage && a.ShotOffsetAngleRange.Max == 0.25f && a.ShotOffsetRadiusRange.Min == 2.0f && a.ObjectDef == 44
          && a.IsDeflectedOnHitByMelee && a.Bounciness == 0.75f && a.IsBlockable && a.SubShotMaxAngle == 30.0f
          && a.PreventSlidingOnCollision && !a.CanHitRollingTargets;
    if (!ok) { std::printf("0046267d_TEST FAIL\n"); return 1; }
    std::printf("0046267d_TEST PASS\n");
    return 0;
}