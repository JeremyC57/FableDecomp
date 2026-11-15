#ifndef FABLE_ENGINE_CHEROABILITYSTATWEIGHT_H
#define FABLE_ENGINE_CHEROABILITYSTATWEIGHT_H
// CHeroAbilityStatWeight (chunk 777, row 0x00803540): Ego_r.pdb, one member StatWeights =
// map<EHeroTrainableStatType, float> (Ego_r 16 bytes; retail STLport 12, so CHeroCombatDef's weightings sit at +0x54 / +0x68).
// Out-of-line const member ?GetParameter@CHeroAbilityStatWeight@@QBEMABVCTCHeroExperience@@@Z 0x00851BD0 (walks StatWeights
// against the hero's trainable stat levels 0x006D6520 / 0x006D6770; landed callers 0x006D6EA0 GetShotAccuracyPercentage).
// The map is opaque here (no caller reads it in place).
#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_CCREATUREDEF_TRANSFER
#include "engine/EHeroTrainableStatType.h"
#include "stlport/fable_map.h"
#endif
class CTCHeroExperience;
class CHeroAbilityStatWeight {
public:
    float GetParameter(const CTCHeroExperience& experience) const;   // 0x00851BD0
private:
#ifdef FABLE_RETAIL_CCREATUREDEF_TRANSFER
public:
    std::map<EHeroTrainableStatType,float> StatWeights;
#else
    unsigned char StatWeights[0xc];                             // +0x00 map<EHeroTrainableStatType,float> (STLport, 12 bytes)
#endif
};
FABLE_STATIC_ASSERT_AT(16, sizeof(CHeroAbilityStatWeight) == 0xc);
#endif // FABLE_ENGINE_CHEROABILITYSTATWEIGHT_H
