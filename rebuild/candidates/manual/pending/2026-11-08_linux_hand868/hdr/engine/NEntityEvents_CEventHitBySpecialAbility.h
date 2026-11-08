#ifndef FABLE_ENGINE_NENTITYEVENTS_CEVENTHITBYSPECIALABILITY_H
#define FABLE_ENGINE_NENTITYEVENTS_CEVENTHITBYSPECIALABILITY_H
// NEntityEvents::CEventHitBySpecialAbility (chunk 868, row 0x007A1B20): Ego_r.pdb struct_layouts_egor.tsv sizeof 0x18 (CEventHitBase
// 0x10 + the ability at +0x10); labels_*.tsv 0x00811830 NEntityEvents::CEventHitBySpecialAbility::CEventHitBySpecialAbility
// (FableWin.pdb ??0CEventHitBySpecialAbility@NEntityEvents@@QAE@ABVCHitParameters@@W4EHeroAbility@@@Z), out of line in retail: the
// will-lightning beam allocates 0x18 bytes (`push 18h ; call operator new`) and constructs it with the hit parameters and
// HERO_ABILITY_LIGHTNING_SPELL (11) before CTCEntityEvents::AddEvent.
#include "engine/NEntityEvents_CEventHitBase.h"
#include "engine/EHeroAbility.h"
namespace NEntityEvents {
class CEventHitBySpecialAbility : public CEventHitBase {
public:
    CEventHitBySpecialAbility(const CHitParameters& hit, EHeroAbility ability);   // 0x00811830
    EHeroAbility Ability;                                // +0x10 (Ego_r.pdb member name unproven: the tsv rows are padding)
    unsigned char _pad_0x14[4];                          // +0x14
};
}
FABLE_STATIC_ASSERT(sizeof(NEntityEvents::CEventHitBySpecialAbility) == 0x18);
#endif
