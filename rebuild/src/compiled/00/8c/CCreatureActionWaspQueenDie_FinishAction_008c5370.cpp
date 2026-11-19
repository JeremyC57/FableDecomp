// CCreatureAction_WaspQueenDie::FinishAction() -- base FinishAction; count the death loop: on the fifth loop kill the creature, otherwise restart the action with a fresh CCreatureAction_WaspQueenDie(creature, LoopNum)
// Leaf: ?FinishAction@CCreatureAction_WaspQueenDie@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C5370, length 158 bytes.
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (PCreature at +8), CThing::Kill(bool) @ 0x004C9B80, CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long,
// const CSoundPair&) @ 0x00858030 (action code 250), CThingCreatureBase::SetCurrentAction @ 0x006644F0, the action
// destructor @ 0x0062E900 (manifest "~CTCLightningOrb" is propagated).
// Shape: the restart builds this very class as a 0x118-byte temporary (inline constructor: base, vtable 0x01280D84, then
// LoopNum at +0x114 from the argument) above the zeroed 8-byte CSoundPair; the early `return` duplicates the epilogue.
// The creature is a named local fetched before the temporary, so LoopNum is re-read after that call (retail
// `mov esi,[esi+114h]`) and the incremented value is not kept in a callee-saved register (`mov eax,ecx ; cmp eax,5`).
// LoopNum (Ego_r.pdb +0x12c) is +0x114 in retail (CCombatActionBase is 0x114 bytes); modelled locally.
// Port (NF93, q11261 / q11262): the local CSoundPair / CBaseIntelligentPointer / CIntelligentPointer / CCombatActionBase / CThingCreatureBase
//   stand-ins replaced by the shared headers (the 0x008C2980 recipe: retail CCombatActionBase with the SOUNDCTOR (creature, priority, sound) ctor
//   0x00858030, engine/CSoundPair.h, CThing::Kill(bool) 0x004C9B80): SetCurrentAction 0x006644F0 and ~CCombatActionBase 0x0062E900 now bind the
//   PDB spellings; own leaf QAE -> UAE (slot-4 override). CCreatureAction_WaspQueenDie has no shared header: it stays a row-local class on the
//   shared CCombatActionBase with its FableWin.pdb inline ctor ??0CCreatureAction_WaspQueenDie@@QAE@AAVCThingCreatureBase@@J@Z (not in Ego_r).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatActionBase.h"

class CCreatureAction_WaspQueenDie : public CCombatActionBase {        // retail sizeof 0x118, vtable 0x01280D84 (no shared header)
public:
    CCreatureAction_WaspQueenDie(CThingCreatureBase& creature, long loop_num)   // FableWin.pdb inline ctor
        : CCombatActionBase(creature, 250, CSoundPair())
        , LoopNum(loop_num)
    {
    }
    virtual void FinishAction();                                         // slot 4, 0x008C5370
protected:
    long LoopNum;                                                        // +0x114 (Ego_r.pdb +0x12c)
};

void CCreatureAction_WaspQueenDie::FinishAction()
{
    CCombatActionBase::FinishAction();
    if (++LoopNum == 5) {
        PCreature.Get()->Kill(true);
        return;
    }
    CThingCreatureBase* pcreature = PCreature.Get();   // fetched first: LoopNum is re-read after this call
    PCreature.Get()->SetCurrentAction(CCreatureAction_WaspQueenDie(*pcreature, LoopNum));
}
