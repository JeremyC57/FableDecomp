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
#include <stdio.h>
#include <string.h>
#include <new>
static void* calls[9]; static int nc=0;
CParentDefClassBase::CParentDefClassBase() { calls[nc++]=this; memset(NativeBaseData,0x39,sizeof NativeBaseData); }
CParentDefClassBase::~CParentDefClassBase() {}
void CParentDefClassBase::Delete() const {}
bool CParentDefClassBase::UnloadResource() {return false;}
unsigned long CParentDefClassBase::GetUnloadableMemory() const {return 3;}
bool CParentDefClassBase::IsResourceLoaded() const {return true;}
void CParentDefClassBase::OnResourceLoaded() {}
bool CParentDefClassBase::CanAbortResourceLoad(long) const {return false;}
void CParentDefClassBase::OnResourceLoadAborted() {}
unsigned long CParentDefClassBase::GetDebugMemoryInUse() const {return 8;}
void CParentDefClassBase::ResetDebugMemoryInUse() {}
CDefString CParentDefClassBase::GetInstantiationName() const { CDefString s;s.TablePos=10;return s;}
void CParentDefClassBase::SetInstantiationName(CDefString) {}
void CParentDefClassBase::AddSubDef(const CCharString&,long,long) {}
void CParentDefClassBase::RemoveSubDef(const CCharString&) {}
CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) {return 0;}
const CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) const {return 0;}
bool CParentDefClassBase::IsParentDef() const {return true;}
void CParentDefClassBase::PostTransfer() {}
void CParentDefClassBase::Transfer(CPersistContext&) {}
void CParentDefClassBase::Copy(const CDefClassBase*) {}
unsigned long CParentDefClassBase::GetSizeofClass() {return 0;}
CHeroCombatDef::~CHeroCombatDef() {}
void CHeroCombatDef::Transfer(CPersistContext&) {calls[nc++]=this;}
void CHeroCombatDef::Copy(const CDefClassBase*) {calls[nc++]=this;}
unsigned long CHeroCombatDef::GetSizeofClass() {return 0x148;}
CHeroAbilityStatWeight::CHeroAbilityStatWeight() {calls[nc++]=this;memset(StatWeights,0x51,sizeof StatWeights);}
CHeroAbilityStatPackage::CHeroAbilityStatPackage() {calls[nc++]=this;memset(StatThresholds,0x71,sizeof StatThresholds);}
static bool zeros(const CFloatRange& f) {return f.Min==0 && f.Max==0;}
int main() {
 void* storage=::operator new(sizeof(CHeroCombatDef));memset(storage,0xa5,sizeof(CHeroCombatDef));
 CHeroCombatDef* p=new(storage) CHeroCombatDef;
 void* expected[9]={static_cast<CParentDefClassBase*>(p),&p->RangedAccuracyWeighting,&p->RangedAccuracyShakeyCursorWeighting,&p->RangedDamageMultiplierWeighting,&p->RangedWeaponReloadSpeedWeighting,&p->MeleeDamageWeighting,&p->HitDamageWeighting,&p->MeleeStrikeAnimationSpeedWeighting,&p->HeavyWeapons};
 bool ok=nc==9;
#ifdef _MSC_VER
 ok=ok && sizeof(CHeroCombatDef)==0x148 && (unsigned char*)&p->RangedAccuracySecondsForMaximumWithSkill-(unsigned char*)p==0x3c && (unsigned char*)&p->RangedAccuracyWeighting-(unsigned char*)p==0x54 && (unsigned char*)&p->DefaultCombatAbilities-(unsigned char*)p==0x11c && (unsigned char*)&p->ProjectileWeaponFirstPersonModeMaxFOVValues-(unsigned char*)p==0x128 && (unsigned char*)&p->HeavyWeapons-(unsigned char*)p==0x13c;
#endif
for(int i=0;i<9;++i)ok=ok&&calls[i]==expected[i];
 ok=ok&&zeros(p->RangedAccuracySecondsForMaximumWithSkill)&&zeros(p->RangedAccuracyPercentFractionWithBuildup)&&zeros(p->RangedAccuracyPercentWithStats)&&zeros(p->RangedAccuracyShakeyCursor)&&zeros(p->MeleeDamageMultiplierWithStats)&&zeros(p->HitDamageMultiplierWithStats)&&zeros(p->MeleeStrikeAnimationSpeedFractionWithStats);
 ok=ok&&!p->DefaultCombatAbilities.m_first&&!p->DefaultCombatAbilities.m_last&&!p->DefaultCombatAbilities.m_end&&!p->ProjectileWeaponFirstPersonModeMaxFOVValues.m_first&&!p->ProjectileWeaponFirstPersonModeMaxFOVValues.m_last&&!p->ProjectileWeaponFirstPersonModeMaxFOVValues.m_end;
 const unsigned char* untouched=(const unsigned char*)&p->RangedDamageMultiplierSpeedNominalBuildup;for(unsigned j=0;j<sizeof(float);++j)ok=ok&&untouched[j]==0xa5;
 for(unsigned k=0;k<sizeof p->NativeBaseData;++k)ok=ok&&p->NativeBaseData[k]==0x39;

 // All bytes outside the native initialized members must keep their original pattern.
 unsigned char touched[sizeof(CHeroCombatDef)]; memset(touched,0,sizeof touched);
 memset(touched,1,sizeof(CParentDefClassBase));
 const CFloatRange* ranges[7]={&p->RangedAccuracySecondsForMaximumWithSkill,&p->RangedAccuracyPercentFractionWithBuildup,&p->RangedAccuracyPercentWithStats,&p->RangedAccuracyShakeyCursor,&p->MeleeDamageMultiplierWithStats,&p->HitDamageMultiplierWithStats,&p->MeleeStrikeAnimationSpeedFractionWithStats};
 for(int ri=0;ri<7;++ri)memset(touched+((const unsigned char*)ranges[ri]-(const unsigned char*)p),1,sizeof(CFloatRange));
 for(int mi=1;mi<9;++mi)memset(touched+((const unsigned char*)expected[mi]-(const unsigned char*)p),1,12);
 memset(touched+((const unsigned char*)&p->DefaultCombatAbilities-(const unsigned char*)p),1,sizeof p->DefaultCombatAbilities);
 memset(touched+((const unsigned char*)&p->ProjectileWeaponFirstPersonModeMaxFOVValues-(const unsigned char*)p),1,sizeof p->ProjectileWeaponFirstPersonModeMaxFOVValues);
 for(unsigned bi=0;bi<sizeof(CHeroCombatDef);++bi)if(!touched[bi])ok=ok&&((const unsigned char*)p)[bi]==0xa5;
 CParentDefClassBase* base=p;ok=ok&&base->GetSizeofClass()==0x148&&base->GetInstantiationName().TablePos==10;
 p->~CHeroCombatDef();::operator delete(storage);
 printf("00458aa7_TEST %s\n",ok?"PASS":"FAIL");return ok?0:1;
}