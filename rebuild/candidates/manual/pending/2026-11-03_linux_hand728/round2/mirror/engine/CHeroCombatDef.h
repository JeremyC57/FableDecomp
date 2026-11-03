#ifndef FABLE_ENGINE_CHEROCOMBATDEF_H
#define FABLE_ENGINE_CHEROCOMBATDEF_H
#include "engine/CParentDefClassBase.h"
// Raw TPI0x14e2c CParentDefClassBase; original MeleeStrikeNumFramesPauseOnHeroStrike
// is donor108, nativeec after retail weight/container reductions; native00667feb/00668055.
// All untouched original members through HeavyWeapons remain at their complete extent.
class CHeroCombatDef : public CParentDefClassBase {
public:
    unsigned char _pad_0x39[0xb3];
    long MeleeStrikeNumFramesPauseOnHeroStrike;
    unsigned char _pad_0xf0[0x58];
};
FABLE_STATIC_ASSERT(sizeof(CHeroCombatDef)==0x148);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef,MeleeStrikeNumFramesPauseOnHeroStrike)==0xec);

#endif

