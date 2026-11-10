#ifndef FABLE_ENGINE_CHEROCOMBATDEF_H
#define FABLE_ENGINE_CHEROCOMBATDEF_H
#include "engine/CParentDefClassBase.h"
// Raw TPI0x14e2c CParentDefClassBase; original MeleeStrikeNumFramesPauseOnHeroStrike
// is donor108, nativeec after retail weight/container reductions; native00667feb/00668055.
// All untouched original members through HeavyWeapons remain at their complete extent.
#ifdef FABLE_RETAIL_CHEROCOMBATDEF_RANGED
// Nested opt-in (chunk 777, row 0x00803540 CTCTargetingPlayer::UpdateTargetedPositionRandomWalk): the ranged-accuracy shake
// members that row reads, Ego_r.pdb names and types. Retail offsets: the first member (RangedAccuracySecondsForMaximumWithSkill,
// Ego_r +0x40) sits at +0x3C and each CHeroAbilityStatWeight (a map) is 12 bytes, so RangedAccuracyPercentWithStats +0x4C /
// RangedAccuracyWeighting +0x54 (landed 0x006D6EA0) and RangedAccuracyShakeyCursor +0x60 (Ego_r +0x68),
// RangedAccuracyShakeyCursorWeighting +0x68 (Ego_r +0x70), RangedAccuracyShakeFractionOfRadius +0x74 (Ego_r +0x80); retail
// 0x008036E9 / 0x008036F1 / 0x0080370A. The padding keeps MeleeStrikeNumFramesPauseOnHeroStrike at +0xEC and sizeof 0x148.
#include "engine/CFloatRange.h"
#include "engine/CHeroAbilityStatWeight.h"
#endif
class CHeroCombatDef : public CParentDefClassBase {
public:
#ifdef FABLE_RETAIL_CHEROCOMBATDEF_RANGED
    unsigned char _pad_0x39[0x60 - 0x39];
    CFloatRange RangedAccuracyShakeyCursor;                     // +0x60
    CHeroAbilityStatWeight RangedAccuracyShakeyCursorWeighting; // +0x68
    float RangedAccuracyShakeFractionOfRadius;                  // +0x74
    unsigned char _pad_0x78[0xec - 0x78];
#else
    unsigned char _pad_0x39[0xb3];
#endif
    long MeleeStrikeNumFramesPauseOnHeroStrike;
#ifdef FABLE_RETAIL_CHEROCOMBATDEF_BLOCK_THRESHOLDS
    unsigned char _pad_0xf0[0x14];
    float AutoBlockSkillThreshold,AutoBlockStrengthThreshold;
    float MeleeRecoilMaintainHandednessSkillThreshold,MeleeRecoilMaintainHandednessStrengthThreshold;
    float MeleeRecoilMaintainComboChainSkillThreshold,MeleeRecoilMaintainComboChainStrengthThreshold;
    unsigned char _pad_0x11c[0x2c];
#else
    unsigned char _pad_0xf0[0x58];
#endif
};
FABLE_STATIC_ASSERT(sizeof(CHeroCombatDef)==0x148);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef,MeleeStrikeNumFramesPauseOnHeroStrike)==0xec);
#ifdef FABLE_RETAIL_CHEROCOMBATDEF_RANGED
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef,RangedAccuracyShakeyCursor)==0x60);
FABLE_STATIC_ASSERT(offsetof(CHeroCombatDef,RangedAccuracyShakeFractionOfRadius)==0x74);
#endif

#endif

