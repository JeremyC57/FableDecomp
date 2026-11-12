#ifndef FABLE_ENGINE_NTHINGSHOT_CTCSHOTDEFINITIONBASE_H
#define FABLE_ENGINE_NTHINGSHOT_CTCSHOTDEFINITIONBASE_H
// NThingShot::CTCShotDefinitionBase (new, chunk 770): RTTI .?AVCTCShotDefinitionBase@NThingShot@@ (class key V, pdb_class_keys.tsv),
// vtable 0x0126E1A4; base CTCBase (0xC bytes, engine/CTCBase.h).
// Layout (Ego_r.pdb names; retail offsets: the STLport vector is 12 bytes, so from PTargetThing on every member sits 4 below the PDB;
// the landed GetInitialVelocity 0x007BEFB0 reads PShotDef at +0x38, the row 0x007BEFF0 PWeapon at +0x30):
//   PEffect +0x0C, HitThingsList +0x14, PTargetThing +0x20, PShooter +0x28, PWeapon +0x30, PShotDef +0x38. Only the members up to
//   PWeapon are modelled (no sizeof assertion); virtual overrides are not declared.
// Name set (Ego_r.pdb, donor order CanBeHitByMeleeStrike / GetInitialVelocity / GetPWeapon = retail 0x007BEFA0 / 0x007BEFB0 /
//   0x007BEFF0): ?GetPWeapon@CTCShotDefinitionBase@NThingShot@@QBEPAVCThing@@XZ.
// Includers define FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CTHING and FABLE_RETAIL_CINTELLIGENTPOINTER first.
#include "engine/CTCBase.h"
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#include "engine/CThingShotDef.h"
#include "engine/C3DVector.h"
#include "engine/ECollideType.h"
#include "engine/CDefPointer.h"
#include "engine/EHeroExperienceStatCategory.h"
class CMeleeCombatAbilityDef;
class CHitLocation;
#endif
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CThing;
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE
class C3DVector;
enum ECollideType;                                                      // MS forward enum (chunk 794)
#endif
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT
#include "engine/CDefPointer.h"
#include "engine/CThingShotDef.h"
class CMeleeCombatAbilityDef;
#endif
namespace NThingShot {
class CTCShotDefinitionBase : public CTCBase {
public:
#ifdef FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_SHOT_DEFINITION;}
#endif
    CThing* GetPWeapon() const;                                         // 0x007BEFF0
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT
    // Retail opt-in (chunk 1025, row 0x007C0AD0 NThingShot::CreateShot): Ego_r.pdb / FableWin.pdb
    // ?Init@CTCShotDefinitionBase@NThingShot@@QAEXV?$CDefPointer@$$CBVCThingShotDef@@@@PAVCThing@@1PBV4@MMMABV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@@Z
    // (the target-thing overload; both CreateShot overloads call it at 0x007C0690). Declaration only; the three floats are named after
    // the members they fill (AccuracyPercentage, DamageMultiplier, ShotBuildupTime; descriptive).
    void Init(CDefPointer<const CThingShotDef> def, CThing* shooter, CThing* weapon, const CThing* target, float accuracy,
              float damageMultiplier, float buildupTime, const CDefPointer<const CMeleeCombatAbilityDef>& ability);   // 0x007C0690
#endif
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT_POSITION
    // Opt-in (chunk 2053, row 0x0087F3A0): Ego_r.pdb
    // ?Init@CTCShotDefinitionBase@NThingShot@@QAEXV?$CDefPointer@$$CBVCThingShotDef@@@@PAVCThing@@1ABVC3DVector@@MMMABV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@@Z
    // (the target-position overload, retail 0x007BF790; the landed 0x008F3630 calls it too). Needs _INIT. Declaration only.
    void Init(CDefPointer<const CThingShotDef> def, CThing* shooter, CThing* weapon, const C3DVector& target, float accuracy,
              float damageMultiplier, float buildupTime, const CDefPointer<const CMeleeCombatAbilityDef>& ability);   // 0x007BF790
#endif
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE
    // Retail opt-in (chunk 794, row 0x007039A0 CThingObject::OnHit): the first two virtuals CTCShotDefinitionBase adds after CTCBase's 28
    // slots (vtable 0x0126E1A4 family; CTCShotDefinitionThrow 0x0123CDC4 slot 28 = 0x007BF510, slot 29 = 0x007C22F0), Ego_r.pdb
    // ?OnCollision@CTCShotDefinitionBase@NThingShot@@UAEXW4ECollideType@@ABVC3DVector@@1PAVCThing@@@Z (slot 28) and
    // ?OnHitByMelee@CTCShotDefinitionBase@NThingShot@@UAEXAAVCThing@@PAV3@ABVC3DVector@@2_N3@Z (slot 29; OnHit calls `[eax+74h]`).
    // Declarations only: no layout change.
    virtual void OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* thing);   // slot 28
    virtual void OnHitByMelee(CThing& hitter, CThing* weapon, const C3DVector& position, const C3DVector& direction, bool knockDown, bool blockable);   // slot 29
#endif
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
public:
    virtual bool GetStatToModifyOnHit(EHeroExperienceStatCategory& category); // slot30
    virtual float GetShotDamage() const; // slot31
protected:
    virtual void UpdateFacingDirection(); // slot32
    virtual void InitMovement(); // slot33
    C3DVector GetShootAtPos() const; // 007bedd0, Ego_r protected const IBE
#endif
protected:
    CIntelligentPointer<CThing> PEffect;                                // +0x0c
    std::vector<CIntelligentPointer<const CThing> > HitThingsList;      // +0x14
    CIntelligentPointer<const CThing> PTargetThing;                     // +0x20
    CIntelligentPointer<CThing> PShooter;                               // +0x28
    CIntelligentPointer<CThing> PWeapon;                                // +0x30
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
    // Retail opt-in (Codex 2026-11-10): full donor tail shifted four bytes after HitThingsList.
    CDefPointer<const CThingShotDef> PShotDef; //38
    CDefPointer<const CMeleeCombatAbilityDef> PShotAbilityDef; //3c
    float AccuracyPercentage; //40
    float DamageMultiplier; //44
    float ShotBuildupTime; //48
    long StartFrame; //4c
    bool SubShotsTriggered; //50
    unsigned char TargetPos[12]; //51, C3DVector; opaque to preserve its odd native offset
    unsigned char _pad5d[3];
    long DieOnFrame; //60
    bool PassedTarget,MadeSound,KnockDown,Decapitate,CanBeDeflected; //64..68
    float InitialVelocity; //6c
    C3DVector BestCollisionPos; //70
    C3DVector BestCollisionNormal; //7c
    CIntelligentPointer<CThing> PBestCollisionThing; //88
    ECollideType BestCollisionType; //90
    CHitLocation* PBestCollisionHitLocation; //94
    bool MultiArrowShotType; //98
#endif
};
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
FABLE_STATIC_ASSERT(sizeof(CTCShotDefinitionBase)==0x9c);
#endif
}
#endif
