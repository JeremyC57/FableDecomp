#ifndef FABLE_ENGINE_CHEROEXPERIENCEDEF_H
#define FABLE_ENGINE_CHEROEXPERIENCEDEF_H
// Retail opt-in (Codex pass 17): raw Ego_r class has eleven STLport containers, each four bytes smaller in retail.
#include "engine/CSubDefClassBase.h"
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
#include "engine/CHeroStatIncreaseData.h"
#include "engine/EDamageAttribute.h"
#endif
#ifdef FABLE_RETAIL_CHEROEXPERIENCEDEF_TRANSFER
#include "engine/CHeroStatIncreaseData.h"
#include "engine/EDamageAttribute.h"
#include "engine/CDefString.h"
#include "engine/CComboMultiplierData.h"
#include "engine/CAttackHistoryCombo.h"
#include "stlport/fable_containers.h"
class CPersistContext;
#endif
class CHeroExperienceDef:public CSubDefClassBase {public:
#if defined(FABLE_RETAIL_CHEROEXPERIENCEDEF_TRANSFER)
 // Retail opt-in (wf16 row 0x006D8F10 CHeroExperienceDef::Transfer): the real STLport member types (Ego_r.pdb) and the slot-18 override
 // ?Transfer@CHeroExperienceDef@@UAEXAAVCPersistContext@@@Z (Ego_r.pdb / FableWin.pdb, vtable 0x0124390C slot 18).
 std::vector<long> ExperienceLevelThresholds,HeroLevelTitles,FallbackMultipliers;
 std::map<long,CDefString> SoundIntensityThresholds;
 std::map<long,CComboMultiplierData> ComboMultiplierMap;
 float ExperienceMultiplierParamPerUnitComboMultiplier,ExperienceMultiplierRegionChangeMultiplier;
 std::map<long,CDefString> ComboMultiplierIncrease,ComboMultiplierLock,ComboMultiplierLost,ComboMultiplierRunningHitsIncrease;
 std::map<EDamageAttribute,CHeroStatIncreaseData> StatIncreasePerHitType;
 std::vector<CAttackHistoryCombo> AttackHistoryCombos;
 virtual void Transfer(CPersistContext&);
#elif defined(FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT)
 // Retail opt-in (Codex pass21): original map type proven by both PDBs and native OnHit accesses.
 unsigned char ExperienceLevelThresholds[12],HeroLevelTitles[12],FallbackMultipliers[12];
 unsigned char SoundIntensityThresholds[12],ComboMultiplierMap[12];
 float ExperienceMultiplierParamPerUnitComboMultiplier,ExperienceMultiplierRegionChangeMultiplier;
 unsigned char ComboMultiplierIncrease[12],ComboMultiplierLock[12],ComboMultiplierLost[12],ComboMultiplierRunningHitsIncrease[12];
 std::map<EDamageAttribute,CHeroStatIncreaseData> StatIncreasePerHitType;
 unsigned char AttackHistoryCombos[12];
#else
 unsigned char ExperienceLevelThresholds[12],HeroLevelTitles[12],FallbackMultipliers[12];
 unsigned char SoundIntensityThresholds[12],ComboMultiplierMap[12];
 float ExperienceMultiplierParamPerUnitComboMultiplier,ExperienceMultiplierRegionChangeMultiplier;
 unsigned char ComboMultiplierIncrease[12],ComboMultiplierLock[12],ComboMultiplierLost[12],ComboMultiplierRunningHitsIncrease[12],StatIncreasePerHitType[12],AttackHistoryCombos[12];
#endif
};
FABLE_STATIC_ASSERT(sizeof(CHeroExperienceDef)==0xb4);
#endif
