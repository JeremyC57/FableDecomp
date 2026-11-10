#ifndef FABLE_ENGINE_CHEROABILITYSTATPACKAGE_H
#define FABLE_ENGINE_CHEROABILITYSTATPACKAGE_H
// CHeroAbilityStatPackage (new shared header, chunk 1004): Ego_r.pdb class (V key), one member StatThresholds =
// map<EHeroTrainableStatType, long> (Ego_r 16 bytes; retail STLport 4.6 _Rb_tree, 12 bytes: the header node pointer at +0, read as
// [map+0] and its leftmost [header+8] by 0x005C9769; node key +0x10, value +0x14). Ego_r.pdb / FableWin.pdb methods:
// ?IsValid@CHeroAbilityStatPackage@@QBE_NABVCTCHeroExperience@@@Z and ?AddStatThreshold@CHeroAbilityStatPackage@@QAEXW4EHeroTrainableStatType@@J@Z
// (declared only; not used by the rows that include this header).
#include "rebuild_abi.h"
#include "engine/EHeroTrainableStatType.h"
#include "stlport/fable_map.h"
class CTCHeroExperience;
class CHeroAbilityStatPackage {
public:
    bool IsValid(const CTCHeroExperience& experience) const;
    void AddStatThreshold(EHeroTrainableStatType stat, long threshold);
    std::map<EHeroTrainableStatType, long> StatThresholds;              // +0x00
};
FABLE_STATIC_ASSERT(sizeof(CHeroAbilityStatPackage) == 0xc);
#endif // FABLE_ENGINE_CHEROABILITYSTATPACKAGE_H
