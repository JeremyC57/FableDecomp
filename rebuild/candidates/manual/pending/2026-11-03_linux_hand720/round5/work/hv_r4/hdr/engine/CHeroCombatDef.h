#ifndef FABLE_ENGINE_CHEROCOMBATDEF_H
#define FABLE_ENGINE_CHEROCOMBATDEF_H
#include "engine/CParentDefClassBase.h"
// Raw TPI0x14e2c CParentDefClassBase; original MeleeStrikeNumFramesPauseOnHeroStrike
// is donor108, nativeec after retail weight/container reductions; native00667feb/00668055.
// All untouched original members through HeavyWeapons remain at their complete extent.
#ifdef FABLE_RETAIL_CHEROCOMBATDEF
// Retail opt-in (w22 agent720c, row 0x005C90FC CTCInventoryWeapons::ConstructWeaponDescriptionContainer): HeavyWeapons, the
// CHeroAbilityStatPackage (Ego_r.pdb: one member, map<EHeroTrainableStatType,long> StatThresholds) at Ego_r +0x164, retail
// +0x13C (the eight CHeroAbilityStatWeight maps and the two vectors before it are 4 bytes narrower each in STLport). The
// container row walks StatThresholds (`mov eax,[esi+13Ch]` header, begin = header->_M_left) against
// CTCHeroExperience::GetTrainableStatLevel. The bytes in between keep their pad (sizeof unchanged, 0x148).
#include "stlport/fable_map.h"
enum EHeroTrainableStatType;
class CHeroAbilityStatPackage {
public:
    std::map<EHeroTrainableStatType, long> StatThresholds;              // +0x00
};
#endif
class CHeroCombatDef : public CParentDefClassBase {
public:
    unsigned char _pad_0x39[0xb3];
    long MeleeStrikeNumFramesPauseOnHeroStrike;
#ifdef FABLE_RETAIL_CHEROCOMBATDEF
    unsigned char _pad_0xf0[0x4c];
    CHeroAbilityStatPackage HeavyWeapons;                               // +0x13c (Ego_r +0x164)
#else
    unsigned char _pad_0xf0[0x58];
#endif
};
FABLE_STATIC_ASSERT(sizeof(CHeroCombatDef)==0x148);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef,MeleeStrikeNumFramesPauseOnHeroStrike)==0xec);

#endif

