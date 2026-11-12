#ifndef FABLE_ENGINE_CWEAPONDEF_H
#define FABLE_ENGINE_CWEAPONDEF_H
// CWeaponDef (new retail-model header, agentSCR 2026-11-10; the generated flattened Ego_r copy stays in engine/_quarantine/).
// RTTI .?AVCWeaponDef@@ : CSubDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase
// (ghidra_out/rtti_bases.tsv). Members: Ego_r.pdb CWeaponDef (struct_layouts_egor.tsv, 0xF4 there) in order and spelling at the
// retail offsets: Ego_r's two CArray<CDefString> in CObjectAugmentationParticleSet and its two maps are 4 bytes larger each
// (Dinkumware), so Shot (Ego_r +0xAC) is retail +0xA0 and sizeof is 0xE4 (as engine/_quarantine/CWeaponDef.h computes).
// Retail evidence: Class +0x2C is the argument of CTCWeapon::IsMeleeWeaponClass in 0x005C8459 / 0x005C9A95 (`mov ecx,[eax+2Ch]`),
// Shot +0xA0 the def index the screens and 0x005C3180 GetActiveProjectileWeaponDamage pass to GetPDefFromGlobalIndex (a
// CThingShotDef, whose HitThingDamage +0x54 they read). StatGetTypeName: the inline class-name literal the landed
// GetSubDef<CWeaponDef> 0x0042B1A6 builds ('CWeaponDef', Ego_r ?StatGetTypeName@CWeaponDef@@SI?AVCCharString@@XZ).
// Composite members are opaque byte arrays (their real types in the comments). Requires FABLE_RETAIL_DEF_CLASS_BASE.
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CWeaponDef.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CSubDefClassBase.h"
#include "engine/EWeaponClass.h"
class CPersistContext;
class CWeaponDef : public CSubDefClassBase {
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CWeaponDef", -1); }   // inline (Ego_r name set)
#ifdef FABLE_RETAIL_CWEAPONDEF_TOOHEAVY
    // agentFAM09 opt-in (row 0x0121F610, its dynamic initializer): Ego_r.pdb / FableWin.pdb `dynamic initializer for
    // 'CWeaponDef::TooHeavyAnimationGroupName'' ("TOO_HEAVY" at 0x013BAF5C), public static const per Anniv360 Test PDB (third
    // source) ?TooHeavyAnimationGroupName@CWeaponDef@@2VCCharString@@B. Static member: no layout change.
    static const CCharString TooHeavyAnimationGroupName;                 // 0x013BAF5C
#endif
    virtual void Transfer(CPersistContext&);                         // ?Transfer@CWeaponDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase*);                         // ?Copy@CWeaponDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                          // ?GetSizeofClass@CWeaponDef@@UAEKXZ
    unsigned char _pad_0x25[0x3];                                    // +0x25
    long          Type;                                              // +0x28 EWeaponType
    EWeaponClass  Class;                                             // +0x2c
    unsigned long Property;                                          // +0x30
    CCharString   AnimationGroupName;                                // +0x34
    long          SheatheObject;                                     // +0x38
    bool          SheatheObjectAlwaysVisible;                        // +0x3c
    float         AnimationSpeedVal;                                 // +0x40
    float         Damage;                                            // +0x44
    long          DamageType;                                        // +0x48 EDamageAttribute
    long          Explosion;                                         // +0x4c
    unsigned char FlourishParticles[0x2c];                           // +0x50 CObjectAugmentationParticleSet
    long          FlourishSpecialWeaponTrail[2];                     // +0x7c CWeaponTrailGraphicSet {Attack, Knockdown}
    unsigned char WeaponTrails[0xc];                                 // +0x84 map<EObjectAugmentationType,CWeaponTrailGraphicSet>
    bool          UseAnalogueLoading;                                // +0x90
    long          MissileLoadCarrySlot;                              // +0x94
    CCharString   ProjectileWeaponHelperNameTip;                     // +0x98
    CCharString   ProjectileWeaponHelperNameBase;                    // +0x9c
    long          Shot;                                              // +0xa0 (def index of the CThingShotDef the weapon fires)
    long          Ammo;                                              // +0xa4
    long          TargetingRange;                                    // +0xa8
    float         TargetingFOV;                                      // +0xac
    unsigned char ArrowTrails[0xc];                                  // +0xb0 map<EObjectAugmentationType,long>
    float         ReloadSpeedRange[2];                               // +0xbc CFloatRange {HighValue, LowValue}
    bool          ProjectileWeaponAutoReload;                        // +0xc4
    bool          ProjectileWeaponStartLoaded;                       // +0xc5
    bool          UseExtendedProjectileTrails;                       // +0xc6
    long          NextWeaponForBoss;                                 // +0xc8
    float         MeleeTargetingArc;                                 // +0xcc
    bool          NeedsLoading;                                      // +0xd0
    CCharString   UnsheatheToCombatAnim;                             // +0xd4
    CCharString   UnsheatheToNormalAnim;                             // +0xd8
    CCharString   SheatheFromCombatAnim;                             // +0xdc
    CCharString   SheatheFromNormalAnim;                             // +0xe0
};
FABLE_STATIC_ASSERT(offsetof(CWeaponDef, Class) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CWeaponDef, Damage) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CWeaponDef, Shot) == 0xa0);
FABLE_STATIC_ASSERT(sizeof(CWeaponDef) == 0xe4);
#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_CWEAPONDEF_H
