// CHeroCombatDef::CHeroCombatDef -- initialize ranges, stat maps, ability vectors and heavy-weapon map.
// Retail00458AA7,212 bytes. Corrected identity from RTTI/vtable01234CF4 and adjacent dtor00458B9D.
// Native base60 vs donor61, maps/vectors12 vs donor16. Shared donor retained for layout audit;
// native polymorphic base is separate because the shared generated header is a flattened nonpolymorphic record.
// Callees CParentDefClassBase ctor00430370, CHeroAbilityStatWeight ctor0045326D (7),
// CHeroAbilityStatPackage ctor0045324F. No globals or pool constants; fldz is literal0.0f.
// Leaf: ??0CHeroCombatDef
#pragma optimize("s",on)
#ifdef _MSC_VER
#define CParentDefClassBase CParentDefClassBaseDonor
#include "engine/CParentDefClassBase.h"
#undef CParentDefClassBase
#else
#define __forceinline inline
#endif
struct CDefString { long TablePos; };
struct CCharString;
struct CDefClassBase { struct CSubDefInfo; };
struct CPersistContext;
struct CParentDefClassBase {
 virtual ~CParentDefClassBase(); // slot0 virtual deleting destructor (native derived00458B81)
 virtual void Delete() const; // slot1 0042D930
 virtual bool UnloadResource(); // slot2 009ACE90
 virtual unsigned long GetUnloadableMemory() const; // slot3 009FBEF0
 virtual bool IsResourceLoaded() const; // slot4 0042D950
 virtual void OnResourceLoaded(); // slot5 0042D960
 virtual bool CanAbortResourceLoad(long) const; // slot6 0042D970
 virtual void OnResourceLoadAborted(); // slot7 0042D980
 virtual unsigned long GetDebugMemoryInUse() const; // slot8 009FBF00
 virtual void ResetDebugMemoryInUse(); // slot9 009FBF10
 virtual CDefString GetInstantiationName() const; // slot10 004303B0
 virtual void SetInstantiationName(CDefString); // slot11 004303C0
 virtual void AddSubDef(const CCharString&,long,long); // slot12 009B00D0
 virtual void RemoveSubDef(const CCharString&); // slot13 009B0170
 virtual CDefClassBase::CSubDefInfo* GetPSubDefInfo(const CCharString&); // VC reverses overload order: native15 009AD510
 virtual const CDefClassBase::CSubDefInfo* GetPSubDefInfo(const CCharString&) const; // native14 009AD4A0
 virtual bool IsParentDef() const; // slot16 004303D0
 virtual void PostTransfer(); // slot17 0042DA80
 virtual void Transfer(CPersistContext&); // slot18 inherited override
 virtual void Copy(const CDefClassBase*); // slot19
 virtual unsigned long GetSizeofClass(); // slot20
 unsigned char NativeBaseData[0x38];
 CParentDefClassBase(); // 0x00430370
};

template <class T> struct CPODVector {             // retail std::vector<T> (12 bytes)
    T* m_first;
    T* m_last;
    T* m_end;
    __forceinline CPODVector() : m_first(0),m_last(0),m_end(0) {}
};
struct CFloatRange { float Min, Max; CFloatRange() : Min(0),Max(0) {} };
struct CHeroAbilityStatWeight {                    // map<EHeroTrainableStatType, float> (STLport, 12 bytes)
    unsigned char StatWeights[12];
    CHeroAbilityStatWeight();                     // 0x0045326D
};
struct CHeroAbilityStatPackage {                   // map<EHeroTrainableStatType, long> (STLport, 12 bytes)
    unsigned char StatThresholds[12];
    CHeroAbilityStatPackage();                    // 0x0045324F
};

struct CHeroCombatDef : CParentDefClassBase {      // vtable 0x01234CF4, retail 0x148 bytes
    CFloatRange RangedAccuracySecondsForMaximumWithSkill;      // +0x3c
    CFloatRange RangedAccuracyPercentFractionWithBuildup;      // +0x44
    CFloatRange RangedAccuracyPercentWithStats;                // +0x4c
    CHeroAbilityStatWeight RangedAccuracyWeighting;            // +0x54
    CFloatRange RangedAccuracyShakeyCursor;                    // +0x60
    CHeroAbilityStatWeight RangedAccuracyShakeyCursorWeighting;    // +0x68
    float RangedAccuracyShakeFractionOfRadius;                 // +0x74
    CHeroAbilityStatWeight RangedDamageMultiplierWeighting;    // +0x78
    float RangedDamageMultiplierSpeedWithStatParameter;        // +0x84
    float RangedDamageMultiplierSpeedWithDistance;             // +0x88
    float RangedDamageMultiplierSpeedNominalBuildup;           // +0x8c
    float RangedDamageMultiplierScaleNominalBuildup;           // +0x90
    float RangedDamageMultiplierSpeedSuperBuildup;             // +0x94
    float RangedDamageMultiplierScaleSuperBuildup;             // +0x98
    CHeroAbilityStatWeight RangedWeaponReloadSpeedWeighting;   // +0x9c
    CFloatRange MeleeDamageMultiplierWithStats;                // +0xa8
    CHeroAbilityStatWeight MeleeDamageWeighting;               // +0xb0
    CFloatRange HitDamageMultiplierWithStats;                  // +0xbc
    CHeroAbilityStatWeight HitDamageWeighting;                 // +0xc4
    bool MeleeFlourishValidOnMultiplierLevelIncrease;          // +0xd0
    long MeleeFlourishNumMultiplierLevelsPerFlourish;          // +0xd4
    bool MeleeFlourishAllowBreakingOfRotation;                 // +0xd8
    float MeleeFlourishSecondsForAvailibility;                 // +0xdc
    bool MeleeFlourishOneShot;                                 // +0xe0
    bool MeleeFlourishUsable;                                  // +0xe1
    bool MeleeCombatPlayStrikeSounds;                          // +0xe2
    float TargetingPressureFractionThresholdForToggle;         // +0xe4
    float MaxAngleToTurnToShootAtTargetInThirdPersonBowMode;   // +0xe8
    long MeleeStrikeNumFramesPauseOnHeroStrike;                // +0xec
    CFloatRange MeleeStrikeAnimationSpeedFractionWithStats;    // +0xf0
    CHeroAbilityStatWeight MeleeStrikeAnimationSpeedWeighting; // +0xf8
    float AutoBlockSkillThreshold;                             // +0x104
    float AutoBlockStrengthThreshold;                          // +0x108
    float MeleeRecoilMaintainHandednessSkillThreshold;         // +0x10c
    float MeleeRecoilMaintainHandednessStrengthThreshold;      // +0x110
    float MeleeRecoilMaintainComboChainSkillThreshold;         // +0x114
    float MeleeRecoilMaintainComboChainStrengthThreshold;      // +0x118
    CPODVector<long> DefaultCombatAbilities;                   // +0x11c
    CPODVector<float> ProjectileWeaponFirstPersonModeMaxFOVValues; // +0x128
    bool ProjectileTargetingAcquireTargetTag;                  // +0x134
    bool ProjectileTargetingAcquireTargetsAuto;                // +0x135
    bool ProjectileTargetingUseAnalogueZoom;                   // +0x136
    float ProjectileWeaponAnalogueZoomControlWeight;           // +0x138
    CHeroAbilityStatPackage HeavyWeapons;                      // +0x13c
    CHeroCombatDef();
    virtual ~CHeroCombatDef(); // 0x00458B9D; deleting thunk00458B81
    virtual void Transfer(CPersistContext&); // 0x004665B6 slot18
    virtual void Copy(const CDefClassBase*); // 0x00461F77 slot19
    virtual unsigned long GetSizeofClass(); // 0x00458B7B slot20
};


CHeroCombatDef::CHeroCombatDef() {}