#ifndef FABLE_ENGINE_NENTITYEVENTS_CEVENTPERFORMEDSPECIALABILITY_H
#define FABLE_ENGINE_NENTITYEVENTS_CEVENTPERFORMEDSPECIALABILITY_H
// New header (agentC25, row 0x0072CF40): Ego_r.pdb NEntityEvents::CEventPerformedSpecialAbility, 16 bytes (CEventBase vptr + WorldFrame,
// Ability +0x08, Aggressive +0x0C; struct_layouts_egor.tsv), constructor ??0CEventPerformedSpecialAbility@NEntityEvents@@QAE@W4EHeroAbility@@@Z
// out of line at 0x008111F0 (`push <ability> ; mov ecx,<new 16 bytes> ; call`). CEventBase comes from the shared event header.
#include "engine/NEntityEvents_CEventKicked.h"
#include "engine/EHeroAbility.h"
namespace NEntityEvents {
class CEventPerformedSpecialAbility : public CEventBase {
public:
    CEventPerformedSpecialAbility(EHeroAbility ability);                 // 0x008111F0
    virtual EEventType GetType() const;
private:
    EHeroAbility Ability;                                                // +0x08
    bool Aggressive;                                                     // +0x0C
};
}
#endif
