// Native virtual destructor, identified by deleting-dtor slot0 RTTI.
// Actual quarantined donor retained. Native packed parent39 vs donor3d;
// native vectors12 vs donor16. novtable views emit no production vtable.
#pragma optimize("s",on)
#define CSpecialAbilitiesLightningSpellDef CSpecialAbilitiesLightningSpellDefDonor
#include "engine/_quarantine/CSpecialAbilitiesLightningSpellDef.h"
#undef CSpecialAbilitiesLightningSpellDef
#define CParentDefClassBase CParentDefClassBaseDonor
#include "engine/CParentDefClassBase.h"
#undef CParentDefClassBase
#include <stdlib.h>
#ifdef _MSC_VER
typedef long S32;
#else
typedef int S32;
#define __declspec(x)
#endif
#pragma pack(push,1)
struct __declspec(novtable) CParentDefClassBase {
 virtual ~CParentDefClassBase(); //430300, genuine native slot0
 unsigned char DefClassBase[0x21];S32 InstantiationName_TablePos;
 unsigned char SubDefInfoMap[0x10];
};
#pragma pack(pop)
template<class T> struct NativeVector {T* Start;T* Finish;T* End;~NativeVector(){if(Start)free(Start);}};
struct __declspec(novtable) CSpecialAbilitiesLightningSpellDef:CParentDefClassBase {
 NativeVector<float> m_faAreaEffect;
 NativeVector<float> m_faDamagePerHit;
 NativeVector<float> m_faSubBeamDamagePerHit;
 NativeVector<float> m_faActivationTime;
 NativeVector<float> m_faDeActivationTime;
 NativeVector<S32> m_naMinMainBeams;
 NativeVector<S32> m_naMaxMainBeams;
 NativeVector<S32> m_naMinSubBeams;
 NativeVector<S32> m_naMaxSubBeams;
 NativeVector<float> m_faTemporaryBeamLife;
 NativeVector<float> m_faTargettingFOV;
 NativeVector<float> m_faStaminaPerSecond;
    float         m_fRandomBeamMinAngleX;
    float         m_fRandomBeamMaxAngleX;
    float         m_fRandomBeamMinAngleZ;
    float         m_fRandomBeamMaxAngleZ;
 NativeVector<float> m_faMainBeamDecapitationTime;
 NativeVector<float> m_faSubBeamDecapitationTime;
 NativeVector<float> m_faSubBeamDelay;
 NativeVector<float> OnhitEveryNSecs;
    float         CombatMultiplierIncreaseTime;
    S32          InitSound_TablePos;
    S32          LoopingSound_TablePos;
    S32          EndHeroSound_TablePos;
    S32          EndAttackeeSound_TablePos;
    float         LightningThemeTransitionInSecs;
    float         LightningThemeTransitionOutSecs;
 virtual ~CSpecialAbilitiesLightningSpellDef();
};
CSpecialAbilitiesLightningSpellDef::~CSpecialAbilitiesLightningSpellDef() {}
