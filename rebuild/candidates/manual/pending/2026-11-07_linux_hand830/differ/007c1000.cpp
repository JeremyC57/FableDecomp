// NThingShot::CTCShotDefinitionBase::OnCollisionProper(bool&) -- resolve the best collision of a projectile: shot-struck event, hit parameters, deflection bounce, explosion, noise, stick-in-thing / experience orb, leave the object behind or kill the shot
// Grade: DIFFER(4655v4510) -- the call sequence matches retail exactly; +145 bytes from the CDefPointer releases (VC7.1 gives load/dec/store once the pointer escaped, retail `dec [ecx+4] ; jne`), the deflection-math scheduling (+36) and a 4-byte larger frame
// Next idea: a visible GetSubDef<CThingMultiArrowShotDef> body under #pragma auto_inline(off) as in the landed 0x007BDD77 sibling (needs CSubDefInfo / GetPSubDefInfo / GetPDefFromGlobalIndex / SetDynamicCast), so VC7.1 knows the callee does not capture the def pointer
// Leaf: ?OnCollisionProper@CTCShotDefinitionBase@NThingShot@@IAEXAA_N@Z
#include <math.h>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#include "fable_string.h"
#include "stlport/fable_containers.h"
class CTCBase;
#include "engine/CVectorMap.h"
enum ETCInterfaceType { };
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
class CRightHandedSet { public:
    C3DVector Up, Forward;
    CRightHandedSet() { }
    CRightHandedSet(const C3DVector& up, const C3DVector& forward);   // 0x00487050
};

class CBaseObject { public: virtual ~CBaseObject(); };
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "engine/CIntelligentPointer.h"

class CDefPointeeBase { public:
    virtual ~CDefPointeeBase();
    virtual void Delete();
    long RefCount;                                                    // +0x04
};
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;
    __forceinline CDefPointer() : Object(0) { }
    __forceinline ~CDefPointer() { if (Object) { CDefPointeeBase* object = Object; if (--object->RefCount == 0) object->Delete(); } }
    T* operator->() const { return static_cast<T*>(Object); }
};
class CDefClassBase : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;
};
class CWeaponDef : public CDefClassBase { public:
    unsigned char _pad08[0x98 - 0x08];
    CCharString ProjectileWeaponHelperNameTip;                        // +0x98
    CCharString ProjectileWeaponHelperNameBase;                       // +0x9c
};
class CThingMultiArrowShotDef : public CDefClassBase { public:
    unsigned char _pad08[0x40 - 0x08];
    long  MultiArrowHitEffect;                                        // +0x40
    unsigned char _pad44[0x50 - 0x44];
    float MultiArrowHitEffectChance;                                  // +0x50 (descriptive)
};
class CThingShotDef : public CDefClassBase { public:
    unsigned char _pad08[0x40 - 0x08];
    long  PrimaryEffect;                                              // +0x40
    unsigned char _pad44[0x50 - 0x44];
    bool  UseEndTimeLineOnContinousEffect;                            // +0x50
    unsigned char _pad51[0x74 - 0x51];
    long  ExplosionDef;                                               // +0x74
    long  GroundExplosionDef;                                         // +0x78
    long  ObjectDef;                                                  // +0x7c
    bool  LeavesObjectBehind;                                         // +0x80
    unsigned char _pad81[0x89 - 0x81];
    bool  AllowDeflections;                                           // +0x89
    bool  BounceOnGround;                                             // +0x8a
    float Bounciness;                                                 // +0x8c
    bool  RotateOnDeflection;                                         // +0x90
    unsigned char _pad91[0xa9 - 0x91];
    bool  DoesExplosionDamageCaster;                                  // +0xa9
    bool  CanHitRollingTargets;                                       // +0xaa
    bool  PreventSlidingOnCollision;                                  // +0xab
};
class CMeleeCombatAbilityDef;
class CThingBaseDef : public CDefClassBase { };

class CTCParamsBase;
class CThing;
class CWorld { public:
    unsigned long* GetWorldSeed() const;                              // 0x0049D810
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);     // 0x004978A0
class CWorldMap { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumberAt(const C3DVector& pos) const;          // slot 16
    float GetGroundSizeZAt(const C3DVector& pos) const;               // 0x0063A300
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;    // 0x009AD410
};
class CGameDefinitionManager { public:
    static CDefinitionManager* Get();                                 // 0x0044C6B0
};

class CTCBase { public:
    virtual ~CTCBase();
    CThing* ParentThing;                                              // +0x04
    bool ToBeRemoved;                                                 // +0x08
    bool IsInGlobalUpdateTCs;                                         // +0x09
    CWorld* GetWorld();                                               // 0x00686A70
    const CWorldMap& PeekWorldMap() const;                            // 0x00686D20
    CWorldMap& GetWorldMap();                                         // 0x00686D30
};
class CTCPhysicsBase : public CTCBase { public:
    virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05(); virtual void s06();
    virtual void s07(); virtual void s08(); virtual void s09(); virtual void s10(); virtual void s11(); virtual void s12();
    virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17(); virtual void s18();
    virtual void s19(); virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23(); virtual void s24();
    virtual void s25(); virtual void s26(); virtual void s27(); virtual void s28(); virtual void s29(); virtual void s30();
    virtual void ResetPosition(const C3DVector& pos);                 // slot 31 (+0x7c)
    virtual void ResetRHSet(const CRightHandedSet& set);              // slot 32 (+0x80)
    virtual void s33(); virtual void s34(); virtual void s35(); virtual void s36(); virtual void s37();
    virtual void EnableGravity(bool enable);                          // slot 38 (+0x98)
    virtual void s39(); virtual void s40(); virtual void s41(); virtual void s42();
    virtual void SetInteractionFlags(long flags);                     // slot 43 (+0xac)
    virtual void ReturnToDefaultInteractionFlags();                   // slot 44 (+0xb0)
    virtual void s45(); virtual void s46(); virtual void s47(); virtual void s48(); virtual void s49(); virtual void s50();
    virtual void s51(); virtual void s52(); virtual void s53(); virtual void s54(); virtual void s55(); virtual void s56();
    virtual void s57(); virtual void s58(); virtual void s59(); virtual void s60(); virtual void s61(); virtual void s62();
    virtual void s63(); virtual void s64(); virtual void s65(); virtual void s66();
    virtual void SetFacingAngleYZ(float angle);                       // slot 67 (+0x10c)
    virtual void SetFacingAngleXZ(float angle);                       // slot 68 (+0x110)
    virtual void s69();
    virtual C3DVector GetFacingVector() const;                        // slot 70 (+0x118)
    C3DVector Position;                                               // +0x0c
    C3DVector OldPosition;                                            // +0x18
    long OldPositionLastFrameSet;                                     // +0x24
    C3DVector Velocity;                                               // +0x28
    float Radius;                                                     // +0x34
    void* PhysicsDef;                                                 // +0x38
    bool Moving : 1;                                                  // +0x3c
    __forceinline void SetVelocity(const C3DVector& v) { Velocity = v; }
    void SetAsMoving(bool moving);                                    // 0x006B0130
};
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* pos, float time) const;   // 0x004C0560
};
}
class CThing : public CBaseObject { public:
    virtual void g01(); virtual void g02(); virtual void g03(); virtual void g04(); virtual void g05(); virtual void g06(); virtual void g07(); virtual void g08(); virtual void g09(); virtual void g10(); virtual void g11(); virtual void g12(); virtual void g13(); virtual void g14(); virtual void g15(); virtual void g16(); virtual void g17(); virtual void g18(); virtual void g19(); virtual void g20(); virtual void g21(); virtual void g22(); virtual void g23(); virtual void g24(); virtual void g25(); virtual void g26(); virtual void g27(); virtual void g28(); virtual void g29(); virtual void g30(); virtual void g31(); virtual void g32(); virtual void g33(); virtual void g34(); virtual void g35(); virtual void g36(); virtual void g37(); virtual void g38(); virtual void g39(); virtual void g40(); virtual void g41(); virtual void g42(); virtual void g43(); virtual void g44(); virtual void g45(); virtual void g46(); virtual void g47(); virtual void g48(); virtual void g49(); virtual void g50(); virtual void g51(); virtual void g52(); virtual void g53(); virtual void g54(); virtual void g55(); virtual void g56(); virtual void g57(); virtual void g58(); virtual void g59(); virtual void g60(); virtual void g61(); virtual void g62(); virtual void g63(); virtual void g64();
    unsigned char _pad04[0x20 - 0x04];
    std::bitset<274> UsingComponents;                                 // +0x20
    CInterfaceMap Components;                                         // +0x44
    unsigned char _pad54[0x60 - 0x54];
    CTCPhysicsBase* PhysicsTC;                                        // +0x60
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    long WorldFrameCreatedAt;                                         // +0x68
    unsigned long ThingTypeFlags;                                     // +0x6c
    CDefPointer<const CThingBaseDef> PBaseDef;                        // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Dead : 1;                                           // +0x91
    __forceinline bool HasTC(long id) const { return UsingComponents.test(id); }
    template <class T> __forceinline bool GetTC(long id, T** ptc) {
        if (HasTC(id)) {
            ETCInterfaceType type = (ETCInterfaceType)id;
            CInterfaceMap* map = &Components;
            CInterfaceMap::iterator it = map->LowerBound(type);
            *ptc = (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
    template <class T> bool GetTCDriver(T** out);
    const C3DVector& GetPos() const;                                  // 0x004C73D0
    void Kill(bool immediate);                                        // 0x004C9B80
    void RemoveTC(long type);                                         // 0x004C9840
    CTCBase* AddTC(const CCharString& name, bool replace, const CTCParamsBase* params);   // 0x004C9D60
    void SetCutscenePauseBehaviour(long behaviour);                   // 0x004C78A0
};
namespace NHitLocations { class CHitLocation; }
class CHitParameters { public:
    bool Localised, Directional;
    unsigned char Position[12], Direction[12];
    bool KnockDown, Decapitate, Blockable, Flourish, EpicSpell, BlockCounter;
    bool PlayHitResponse, PlayHitResponseOverrideSet, MoveBack;
    bool CreateParticleEffectOnHit, CreateDustParticleEffectOnHit, GuaranteeHit;
    bool Blocked, HitNegated, CauseRecoil;
    unsigned char _rest[0x5c - 0x29];
    CHitParameters();                                                 // 0x007A3BC0
    ~CHitParameters();                                                // 0x00789358
};
class CThingGameObject : public CThing { public:
    virtual void OnHit(CHitParameters& params);                       // slot 65 (+0x104)
    void GetShotHitParameters(NHitLocations::CHitLocation* location, CThing& shot, const C3DVector& pos, CHitParameters& params) const;   // 0x00838EC0
};
namespace NEntityEvents {
class CEventBase { public:
    long WorldFrame;
    CEventBase() : WorldFrame(0) { }
    virtual ~CEventBase() { }
};
class CEventShotStruck : public CEventBase { public:
    C3DVector StrikePos;
    CEventShotStruck(const C3DVector& pos) : StrikePos(pos) { }
};
}
class CTCEntityEvents : public CTCBase { public:
    void AddEvent(NEntityEvents::CEventBase* event);                  // 0x008113C0
};
class CTCWeaponTrail : public CTCBase { public:
    bool GetLastProjectileWeaponTrailPoints(std::vector<std::pair<C3DVector, C3DVector> >& points) const;   // 0x0076E5D0
    void SetLastProjectileWeaponTrailPoint(const C3DVector& pos);     // 0x0076E290
};
class CTCRotate : public CTCBase { public:
    void SetRotationInfo(const C3DVector& axis, float speed, float seconds);   // 0x0072A530
    unsigned char _pad0c[0x28 - 0x0c];
    bool ResetOnStop;                                                 // +0x28 (descriptive)
};
class CTCDParticleEmitter { public:
    static CThing* __fastcall Create(long type, const C3DVector& pos, bool flag);   // 0x006E0880
    void EnterTimelineEnd();                                          // 0x006E05F0
};
class CTCDExplosion { public:
    static CThing* __fastcall Create(long defIndex, const C3DVector& pos);   // 0x0080C460
    void SetThingAsImmune(CThing& thing);                             // 0x0080CBF0
};
class CThingNoise : public CThing { public:
    static CThingNoise* __fastcall Create(long defIndex, const C3DVector& pos, float radius);   // 0x008385B0
    void PassToNoiseComponent(CThing* thing);                         // 0x008386E0
};
class CTCHeroExperience : public CTCBase { public:
    void AddExperience(long amount);                                  // 0x006D77C0
};
class CTCDExperienceOrb { public:
    unsigned char _pad00[0x10];
    long Experience;                                                  // +0x10
    void OnCollected(CThing& collector);                              // 0x0077A180
};
class CTCDeadShotInThingTracker : public CTCBase { public:
    void SetThingStuckIn(const CThing& thing);                        // 0x007E3490
};
class CTCObjectAugmentations : public CTCBase { public:
    long GetOnHitExplodingEffect() const;                             // 0x00766480
    void StopTrailEffect();                                           // 0x007664E0
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x004CC340
class CMainGameComponent { public:
    static long ConstantFPS;                                          // 0x01375550
    static long GetConstantFPS() { return ConstantFPS; }
};

namespace NThingShot {
class CTCShotDefinitionBase : public CTCBase {
protected:
    virtual void t01(); virtual void t02(); virtual void t03(); virtual void t04(); virtual void t05(); virtual void t06();
    virtual void t07(); virtual void t08(); virtual void t09(); virtual void t10(); virtual void t11(); virtual void t12();
    virtual void t13(); virtual void t14(); virtual void t15(); virtual void t16(); virtual void t17(); virtual void t18();
    virtual void t19(); virtual void t20(); virtual void t21(); virtual void t22(); virtual void t23(); virtual void t24();
    virtual void t25(); virtual void t26(); virtual void t27(); virtual void t28(); virtual void t29(); virtual void t30();
    virtual void t31(); virtual void t32(); virtual void t33(); virtual void t34(); virtual void t35(); virtual void t36();
    virtual void t37(); virtual void t38(); virtual void t39(); virtual void t40(); virtual void t41();
    virtual bool IsNegatedHit() const;                                // slot 42 (+0xa8)
    virtual bool IsCreateParticleEffectOnHit() const;                 // slot 43 (+0xac)
    void OnCollisionProper(bool& killed);
    CIntelligentPointer<CThing> PEffect;                              // +0x0c
    std::vector<CIntelligentPointer<const CThing> > HitThingsList;    // +0x14
    CIntelligentPointer<const CThing> PTargetThing;                   // +0x20
    CIntelligentPointer<CThing> PShooter;                             // +0x28
    CIntelligentPointer<CThing> PWeapon;                              // +0x30
    CDefPointer<const CThingShotDef> PShotDef;                        // +0x38
    CDefPointer<const CMeleeCombatAbilityDef> PShotAbilityDef;        // +0x3c
    float AccuracyPercentage;                                         // +0x40
    float DamageMultiplier;                                           // +0x44
    float ShotBuildupTime;                                            // +0x48
    long StartFrame;                                                  // +0x4c
    bool SubShotsTriggered;                                           // +0x50
    C3DVector TargetPos;                                              // +0x54
    long DieOnFrame;                                                  // +0x60
    bool PassedTarget, MadeSound, KnockDown, Decapitate, CanBeDeflected;   // +0x64
    float InitialVelocity;                                            // +0x6c
    C3DVector BestCollisionPos;                                       // +0x70
    C3DVector BestCollisionNormal;                                    // +0x7c
    CIntelligentPointer<CThing> PBestCollisionThing;                  // +0x88
    long BestCollisionType;                                           // +0x90
    NHitLocations::CHitLocation* PBestCollisionHitLocation;           // +0x94
    bool MultiArrowShotType;                                          // +0x98
};
}

void NThingShot::CTCShotDefinitionBase::OnCollisionProper(bool& killed)
{
    if (BestCollisionType == 0)
        return;
    if (PShotDef->PreventSlidingOnCollision) {
        CTCPhysicsBase* physics = ParentThing->PhysicsTC;
        physics->SetVelocity(C3DVector(0.0f, 0.0f, 0.0f));
        physics->SetAsMoving(true);
    }
    CThingGameObject* hitThing = (CThingGameObject*)PBestCollisionThing.Get();
    long type = BestCollisionType;
    NHitLocations::CHitLocation* hitLocation = PBestCollisionHitLocation;
    bool stuck = false;
    CTCEntityEvents* events;
    if (ParentThing->GetTC(0x42, &events))
        events->AddEvent(new NEntityEvents::CEventShotStruck(BestCollisionPos));
    if (hitThing && !hitThing->Dead && (hitThing->ThingTypeFlags & 4) && hitLocation) {
        CDefPointer<const CWeaponDef> weaponDef;
        if (!ParentThing->PBaseDef->GetSubDef(&weaponDef)) {
            stuck = true;
        } else {
            CHitParameters params;
            hitThing->GetShotHitParameters(hitLocation, *ParentThing, BestCollisionPos, params);
            params.HitNegated = IsNegatedHit();
            params.CreateParticleEffectOnHit = IsCreateParticleEffectOnHit();
            hitThing->OnHit(params);
            if (params.Blocked || params.HitNegated)
                stuck = true;
        }
    }
    if (PBestCollisionThing.Get() && !PBestCollisionThing.Get()->Dead) {
        CIntelligentPointer<const CThing> hit;
        hit.Set(PBestCollisionThing.Get());
        HitThingsList.push_back(hit);
    }
    if (ParentThing->HasTC(0x2a))
        ParentThing->RemoveTC(0x2a);
    if (((type == 3 && PShotDef->BounceOnGround) || stuck) && PShotDef->AllowDeflections) {
        ParentThing->SetCutscenePauseBehaviour(2);
        CTCWeaponTrail* trail;
        ParentThing->GetTC(0x4c, &trail);
        std::vector<std::pair<C3DVector, C3DVector> > points;
        C3DVector direction;
        if (trail->GetLastProjectileWeaponTrailPoints(points) && points.size() >= 2) {
            direction = (points.end() - 1)->first - (points.end() - 2)->first;
            trail->SetLastProjectileWeaponTrailPoint(BestCollisionPos);
        } else {
            CTCPhysicsBase* physics = ParentThing->PhysicsTC;
            direction = physics->Position - physics->OldPosition;
        }
        direction *= PShotDef->Bounciness * 0.5f;
        C3DVector reflection = BestCollisionNormal * (float)fabs(direction.Dot(BestCollisionNormal));
        direction = direction + reflection + reflection;
        CThing* thing = ParentThing;
        CThing* shooter = PShooter.Get();
        thing->PhysicsTC->SetVelocity(C3DVector(0.0f, 0.0f, 0.0f));
        thing->PhysicsTC->SetAsMoving(true);
        C3DVector up(0.0f, 0.0f, 1.0f);
        C3DVector forward = direction;
        C3DVector side(direction.Y, -direction.X, 0.0f);
        if (side.GetSquaredMagnitude2D() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
            up = side ^ forward;
            up.AccurateNormalise();
        } else {
            forward.Z = 0.0f;
            if (forward.GetSquaredMagnitude2D() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
                forward.AccurateNormalise();
        }
        if (!(ParentThing->ThingTypeFlags & 8)) {
            CRightHandedSet set;
            set.Up = up;
            set.Forward = forward;
            thing->PhysicsTC->ResetRHSet(set);
        }
        CDefPointer<const CWeaponDef> weaponDef;
        C3DVector offset;
        if (thing->PBaseDef->GetSubDef(&weaponDef)) {
            C3DVector tip;
            if (!thing->PTCGraphicAppearance->GetHelperPoint(weaponDef->ProjectileWeaponHelperNameTip, 0, 0, &tip, 1.0f))
                tip = thing->GetPos();
            offset = tip - thing->GetPos();
        } else {
            offset = C3DVector(0.0f, 0.0f, 0.0f);
        }
        const C3DVector& position = BestCollisionPos;
        thing->PhysicsTC->ResetPosition(position - offset);
        thing->PhysicsTC->SetVelocity(forward);
        thing->PhysicsTC->SetAsMoving(true);
        thing->PhysicsTC->EnableGravity(true);
        if (PShotDef->RotateOnDeflection) {
            CTCRotate* rotate = (CTCRotate*)thing->AddTC(CCharString("CTCRotate"), false, 0);
            rotate->SetRotationInfo(BestCollisionNormal, 1.0f / CMainGameComponent::GetConstantFPS(), 100.0f);
            rotate->ResetOnStop = true;
        }
        thing->RemoveTC(0x77);
        if (!(ParentThing->ThingTypeFlags & 8))
            ParentThing->RemoveTC(0x7f);
        killed = true;
        if (PShotDef->PrimaryEffect)
            CTCDParticleEmitter::Create(PShotDef->PrimaryEffect, position, false);
        if (MultiArrowShotType) {
            CDefPointer<const CThingMultiArrowShotDef> arrowDef;
            PShotDef->GetSubDef(&arrowDef);
            long effect = arrowDef->MultiArrowHitEffect;
            if (effect && GFFloatRandom(1.0f, GetWorld()->GetWorldSeed()) < arrowDef->MultiArrowHitEffectChance)
                CTCDParticleEmitter::Create(effect, position, false);
        }
        CTCObjectAugmentations* augmentations;
        if (ParentThing->GetTC(0xbf, &augmentations)) {
            long effect = augmentations->GetOnHitExplodingEffect();
            if (effect)
                CTCDParticleEmitter::Create(effect, position, false);
        }
        return;
    }
    long explosion;
    if (PShotDef->GroundExplosionDef >= 0 && type == 3)
        explosion = PShotDef->GroundExplosionDef;
    else
        explosion = PShotDef->ExplosionDef;
    if (explosion >= 0) {
        CThing* blast = CTCDExplosion::Create(explosion, BestCollisionPos);
        if (!PShotDef->DoesExplosionDamageCaster && blast) {
            CTCDExplosion* driver;
            blast->GetTCDriver(&driver);
            driver->SetThingAsImmune(*PShooter.Get());
        }
    }
    if (!ParentThing->Dead) {
        if (PeekWorldMap().GetMapNumberAt(BestCollisionPos)) {
            long noiseDef = CGameDefinitionManager::Get()->GetDefGlobalIndexFromName(CCharString("SIMPLE_NOISE"));
            CThingNoise* noise = CThingNoise::Create(noiseDef, BestCollisionPos, -1.0f);
            if (IS_THING_ALIVE(PShooter))
                noise->PassToNoiseComponent(PShooter.Get());
        }
        const CThingShotDef* shotDef = PShotDef.operator->();
        bool leaveObject = shotDef->LeavesObjectBehind != 0;
        bool finishShot = true;
        bool keepShot = false;
        stuck = true;
        if (type == 1) {
            if (hitThing && !hitThing->Dead) {
                unsigned long flags = hitThing->ThingTypeFlags;
                CTCHeroExperience* experience;
                if (hitThing->HasTC(0x69) && IS_THING_ALIVE(PShooter) && PShooter.Get()->GetTC(0x68, &experience)) {
                    CTCDExperienceOrb* orb;
                    if (hitThing->GetTCDriver(&orb)) {
                        experience->AddExperience(orb->Experience);
                        orb->OnCollected(*PShooter.Get());
                    }
                }
                if ((flags & 8) || hitThing->HasTC(0x5a) || hitThing->PhysicsTC->Moving) {
                    stuck = false;
                    leaveObject = false;
                }
            } else {
                stuck = false;
                leaveObject = false;
            }
            if (shotDef->PrimaryEffect)
                CTCDParticleEmitter::Create(shotDef->PrimaryEffect, BestCollisionPos, false);
            if (MultiArrowShotType) {
                CDefPointer<const CThingMultiArrowShotDef> arrowDef;
                PShotDef->GetSubDef(&arrowDef);
                long effect = arrowDef->MultiArrowHitEffect;
                if (effect && GFFloatRandom(1.0f, GetWorld()->GetWorldSeed()) < arrowDef->MultiArrowHitEffectChance)
                    CTCDParticleEmitter::Create(effect, BestCollisionPos, false);
            }
            CTCObjectAugmentations* augmentations;
            if (ParentThing->GetTC(0xbf, &augmentations)) {
                long effect = augmentations->GetOnHitExplodingEffect();
                if (effect)
                    CTCDParticleEmitter::Create(effect, BestCollisionPos, false);
            }
            if (PShotAbilityDef.Object) {
                keepShot = true;
                finishShot = false;
            }
            if (stuck && hitThing && !hitThing->Dead) {
                ParentThing->AddTC(CCharString("CTCDeadShotInThingTracker"), false, 0);
                CTCDeadShotInThingTracker* tracker;
                ParentThing->GetTC(0x76, &tracker);
                tracker->SetThingStuckIn(*hitThing);
            }
        } else if (type == 3) {
            if (shotDef->PrimaryEffect)
                CTCDParticleEmitter::Create(shotDef->PrimaryEffect, BestCollisionPos, false);
            if (MultiArrowShotType) {
                CDefPointer<const CThingMultiArrowShotDef> arrowDef;
                PShotDef->GetSubDef(&arrowDef);
                long effect = arrowDef->MultiArrowHitEffect;
                if (effect && GFFloatRandom(1.0f, GetWorld()->GetWorldSeed()) < arrowDef->MultiArrowHitEffectChance)
                    CTCDParticleEmitter::Create(effect, BestCollisionPos, false);
            }
            CTCObjectAugmentations* augmentations;
            if (ParentThing->GetTC(0xbf, &augmentations)) {
                long effect = augmentations->GetOnHitExplodingEffect();
                if (effect)
                    CTCDParticleEmitter::Create(effect, BestCollisionPos, false);
            }
        } else if (type == 4) {
            leaveObject = false;
        }
        CTCPhysicsBase* physics = ParentThing->PhysicsTC;
        if (ParentThing->ThingTypeFlags & 8) {
            physics->SetFacingAngleXZ(0.0f);
            physics->SetFacingAngleYZ(0.0f);
        } else if (finishShot) {
            if (leaveObject) {
                if (IS_THING_ALIVE(PEffect)) {
                    CTCDParticleEmitter* emitter = 0;
                    PEffect.Get()->GetTCDriver(&emitter);
                    if (PShotDef->UseEndTimeLineOnContinousEffect)
                        emitter->EnterTimelineEnd();
                    else
                        PEffect.Get()->Kill(true);
                }
                CTCObjectAugmentations* augmentations;
                if (ParentThing->GetTC(0xbf, &augmentations))
                    augmentations->StopTrailEffect();
                CDefPointer<const CWeaponDef> weaponDef;
                if (stuck && ParentThing->PBaseDef->GetSubDef(&weaponDef)) {
                    NGraphicAppearance::CTCGraphicAppearance* appearance = ParentThing->PTCGraphicAppearance;
                    CCharString tipName(weaponDef->ProjectileWeaponHelperNameTip);
                    CCharString baseName(weaponDef->ProjectileWeaponHelperNameBase);
                    C3DVector tip;
                    C3DVector base;
                    if (tipName == baseName) {
                        appearance->GetHelperPoint(tipName, 0, 0, &tip, 1.0f);
                        appearance->GetHelperPoint(baseName, 1, 0, &base, 1.0f);
                    } else {
                        appearance->GetHelperPoint(tipName, 0, 0, &tip, 1.0f);
                        appearance->GetHelperPoint(baseName, 0, 0, &base, 1.0f);
                    }
                    C3DVector position = ParentThing->GetPos();
                    C3DVector middle = base + (tip - base) * 2.0f / 3.0f;
                    physics->ResetPosition(BestCollisionPos - (middle - position));
                    appearance->GetHelperPoint(tipName, 0, 0, &tip, 1.0f);
                    appearance->GetHelperPoint(baseName, 0, 0, &base, 1.0f);
                } else {
                    C3DVector ground = BestCollisionPos;
                    ground.Z = GetWorldMap().GetGroundSizeZAt(ground);
                    physics->ResetPosition(ground);
                    C3DVector facing = physics->GetFacingVector();
                    facing.Z = 0.0f;
                    if (facing.GetSquaredMagnitude2D() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
                        facing.AccurateNormalise();
                        physics->ResetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), facing));
                    }
                }
                physics->SetAsMoving(false);
                physics->SetInteractionFlags(0x20);
                ParentThing->RemoveTC(0x77);
                if (!(ParentThing->ThingTypeFlags & 8))
                    ParentThing->RemoveTC(0x7f);
                killed = true;
            } else {
                if (IS_THING_ALIVE(PEffect)) {
                    CTCDParticleEmitter* emitter = 0;
                    PEffect.Get()->GetTCDriver(&emitter);
                    if (PShotDef->UseEndTimeLineOnContinousEffect)
                        emitter->EnterTimelineEnd();
                    else
                        PEffect.Get()->Kill(true);
                }
                CTCObjectAugmentations* augmentations;
                if (ParentThing->GetTC(0xbf, &augmentations))
                    augmentations->StopTrailEffect();
                ParentThing->Kill(true);
                killed = true;
            }
            goto done;
        }
        if (!keepShot) {
            if (IS_THING_ALIVE(PEffect)) {
                CTCDParticleEmitter* emitter = 0;
                PEffect.Get()->GetTCDriver(&emitter);
                if (PShotDef->UseEndTimeLineOnContinousEffect)
                    emitter->EnterTimelineEnd();
                else
                    PEffect.Get()->Kill(true);
            }
            CTCObjectAugmentations* augmentations;
            if (ParentThing->GetTC(0xbf, &augmentations))
                augmentations->StopTrailEffect();
            physics->SetAsMoving(false);
            physics->EnableGravity(true);
            physics->ReturnToDefaultInteractionFlags();
            ParentThing->RemoveTC(0x77);
            if (!(ParentThing->ThingTypeFlags & 8))
                ParentThing->RemoveTC(0x7f);
            killed = true;
        }
    }
done:
    BestCollisionType = 0;
    PBestCollisionThing = CIntelligentPointer<CThing>(0);
    PBestCollisionHitLocation = 0;
}
