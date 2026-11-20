#ifndef FABLE_ENGINE_NCREATUREMODE_CMODEDRAINED_H
#define FABLE_ENGINE_NCREATUREMODE_CMODEDRAINED_H
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CIntelligentPointer.h"
namespace NCreatureMode {
class CModeDrained:public CModeStandBase {
public:
#ifdef FABLE_RETAIL_NCREATUREMODE_CMODEDRAINED_CTOR
    // NF147 opt-in (row 0x008676A0 CCombatAction_GenericResponseToDrain::Initialise): Ego_r.pdb 0x0083E1B0 / FableWin.pdb 0x0281FB70
    // ??0CModeDrained@NCreatureMode@@QAE@AAVCThing@@0@Z, out of line in retail at 0x00868220. Declaration only; no layout change.
    CModeDrained(CThing& thing, CThing& drainer);                       // 0x00868220
#endif
#ifdef FABLE_RETAIL_NCREATUREMODE_ALLOC
    // Opt-in (row 0x0073B130 CTCCreatureModeManager::RegisterModes): FableWin.pdb ?Alloc@CModeDrained@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z.
    static CTCCreatureModeManager::CModeBase* __fastcall Alloc(CThing& owner);
#endif
 virtual ~CModeDrained();
 virtual unsigned long GetType()const;
 virtual const CAnimFilterBase& GetAnimFilter()const;
 virtual void OnActivate();
 virtual void FrameUpdate();
 virtual void OnDeactivate();
 virtual bool IsAllowingPlayerSpecialMovementMoves()const;
private:
 CAnimFilterOfGroup Filter;
 CIntelligentPointer<CThing> PDrainer;
 unsigned long DrainerMode;
};
}
FABLE_STATIC_ASSERT_AT(27, sizeof(NCreatureMode::CModeDrained)==128);
#endif
