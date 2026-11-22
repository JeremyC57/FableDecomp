#ifndef FABLE_ENGINE_NCREATUREMODE_CMODEDRAGONHOVER_H
#define FABLE_ENGINE_NCREATUREMODE_CMODEDRAGONHOVER_H
#ifdef FABLE_RETAIL_JACKDRAGON_HOVER_PASS27
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CIntelligentPointer.h"
namespace NCreatureMode {
// Original RTTI derives from CModeStandBase. FableWin 02864F80 builds Filter and PTarget in sequence.
class CModeDragonHover : public CModeStandBase {
public:
    CModeDragonHover(CThing& parent_thing,CThing& target);
    virtual unsigned long GetType() const;
#ifdef FABLE_RETAIL_NCREATUREMODE_CMODEDRAGONHOVER_VIRTUALS
    // NF247 opt-in (q12682, row 0x008E31A0 ~CModeDragonHover, emitter of the derived CModeDragonFireBeam vtable 0x01285900): the (CThing&)
    // constructor ??0CModeDragonHover@NCreatureMode@@QAE@AAVCThing@@@Z and the class's own overrides of vtable 0x0126845C, slot 2
    // ?AnimUpdate@CModeDragonHover@NCreatureMode@@UAEXAAVCAnimateThing@@@Z (0x008D6FF0), 3 ?FrameUpdate@..@@UAEXXZ (0x008DB460), 4
    // ?GetAnimFilter@..@@UBEABVCAnimFilterBase@@XZ (0x00741C00), 5 ?OnActivate@..@@UAEXXZ (0x008D6A30); each in Ego_r.pdb / FableWin.pdb /
    // Ego_d.pdb. Slot 6 is CModeStandBase::OnDeactivate 0x00861D00 (no DragonHover override). Declarations only: no layout change.
    CModeDragonHover(CThing& parent_thing);
    virtual void AnimUpdate(CAnimateThing& animation);                   // slot 2, 0x008D6FF0
    virtual void FrameUpdate();                                          // slot 3, 0x008DB460
    virtual const CAnimFilterBase& GetAnimFilter() const;                // slot 4, 0x00741C00
    virtual void OnActivate();                                           // slot 5, 0x008D6A30
#endif
private:
    CAnimFilterOfGroup Filter;
    CIntelligentPointer<CThing> PTarget;
    bool UseHeadTurning;
};
FABLE_STATIC_ASSERT_AT(19, sizeof(CModeDragonHover)==0x80);
}
#endif
#endif
