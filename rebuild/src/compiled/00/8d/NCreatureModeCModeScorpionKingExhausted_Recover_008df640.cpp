// NCreatureMode::CModeScorpionKingExhausted::Recover() -- the creature (+0x78) sent underground (virtual slot 28 with true) and given a CCreatureAction_ScorpionRecover (0x008E4770, a 0x114-byte temporary on the frame) as its current action (SetCurrentAction @ 0x006644F0)
// Leaf: ?Recover@CModeScorpionKingExhausted@NCreatureMode@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008DF640, length 58 bytes.
// Callees: the creature's virtual slot 28 (+0x70), CCreatureAction_ScorpionRecover::CCreatureAction_ScorpionRecover(CThingCreatureBase&) @ 0x008E4770, CThingCreatureBase::SetCurrentAction @ 0x006644F0 (the action by reference: the explicit temporary reuses the constructor's returned `this` in eax), CCombatActionBase::~CCombatActionBase @ 0x0062E900 (labelled ~CTCLightningOrb, propagated). Shape: a 0x114 frame, `this` in esi, the creature re-read from +0x78 for every use, `ret`.
// Port (NF93, q11261 / q11262): the local NCreatureMode::CThingCreatureBase / CCreatureActionBase / CCreatureAction_GoUnderground / CModeBase
//   stand-ins replaced by the shared headers (the landed sibling 0x008DF3A0 Vulnerable::Recover recipe) with the NF93 opt-in
//   CCREATUREACTION_SCORPIONRECOVER_CLASS. The temporary is CCreatureAction_ScorpionRecover, not GoUnderground: 0x008E4770 stores vtable
//   0x01286724 (RTTI .?AVCCreatureAction_ScorpionRecover@@) and is Ego_r ??0CCreatureAction_ScorpionRecover@@QAE@AAVCThingCreatureBase@@@Z.
//   Slot 28 is CThing::SetAsDrawable (CThing.h FABLE_RETAIL_CTHING_DRAWABLE; the old local name SetUnderground was made up). Recover is
//   private (Ego_r.pdb ?Recover@CModeScorpionKingExhausted@NCreatureMode@@AAEXXZ): own leaf QAE -> AAE. CModeScorpionKingExhausted has no
//   shared header: it stays a row-local class (Ego_r.pdb layout, as its landed siblings 0x008D82B0 / 0x008DA2E0) on the shared CModeStandBase.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_ISPLACED
#define FABLE_RETAIL_CTHING_DRAWABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONRECOVER_CLASS
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CCreatureAction_ScorpionRecover.h"

namespace NCreatureMode {
class CModeScorpionKingExhausted : public CModeStandBase {             // Ego_r.pdb sizeof 0x80, vtable 0x012858A0 (no shared header)
private:
    void Recover();                                                      // 0x008DF640
    long RecoveryCount;                                                  // +0x68
    CAnimFilterOfGroup Filter;                                           // +0x6c
    CThingCreatureBase* PCreature;                                       // +0x78
    bool bRecovering;                                                    // +0x7c
};
}

void NCreatureMode::CModeScorpionKingExhausted::Recover()
{
    PCreature->SetAsDrawable(true);
    PCreature->SetCurrentAction(CCreatureAction_ScorpionRecover(*PCreature));
}
