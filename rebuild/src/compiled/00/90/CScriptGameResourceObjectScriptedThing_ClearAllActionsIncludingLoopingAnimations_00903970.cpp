// CScriptGameResourceObjectScriptedThing::ClearAllActionsIncludingLoopingAnimations() -- as ClearAllActions, then finish the current action a second time and clear it (CThingCreatureBase::ClearCurrentAction)
// Leaf: ?ClearAllActionsIncludingLoopingAnimations@CScriptGameResourceObjectScriptedThing@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00903970, length 84 bytes (vtable 0x0128D86C slot 22).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 four times (on PThing at +0x14), CThingCreatureBase::ClearQueuedActions @ 0x00663600,
//   FinishCurrentAction @ 0x00662650 twice, ClearCurrentAction @ 0x006633A0 (tail jump). Shape: same guard chain as ClearAllActions
//   (0x00903920); the creature local in esi for four calls.
// Identity (two sources): Ego_r.pdb 0x0092A280 + FableWin.pdb 0x02B4C100 ?ClearAllActionsIncludingLoopingAnimations@CScriptGameResourceObjectScriptedThing@@UAEXXZ,
//   the override in vtable 0x0128D86C slot 22 (base 0x007E7410); no landed row references 0x00903970 (exe scan: the vtable only).
// NF171 (2026-11-20, q11935 / q11936): ported onto the shared headers (as 0x00903920; CLEARCURRENTACTION as the landed 0x008916A0 uses it):
//   0x006633A0 binds Ego_r.pdb 0x0053E2D0 + FableWin.pdb 0x01D9E6D0 ?ClearCurrentAction@CThingCreatureBase@@QAEXXZ (was the stand-in
//   ?ReleaseLoopingAnimation@CThing_Retail@@QAEXXZ), 0x00662650 / 0x00663600 their CThingCreatureBase PDB spellings; own leaf was
//   ?ClearAllActionsIncludingLoopingAnimations@CScriptGameResourceObjectScriptedThing_Methods@@QAEXXZ.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTHINGCREATUREBASE_CLEARQUEUED
#define FABLE_RETAIL_CTHINGCREATUREBASE_CLEARCURRENTACTION
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHING
#define FABLE_RETAIL_SCRIPTEDTHING_CLEARALLACTIONS
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CScriptGameResourceObjectScriptedThing.h"

void CScriptGameResourceObjectScriptedThing::ClearAllActionsIncludingLoopingAnimations()
{
    if (PThing.Get()) {
        if (!PThing.Get()->IsDead()) {
            if (PThing.Get()->GetTypeFlags() & 8) {
                CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(PThing.Get());
                creature->ClearQueuedActions();
                creature->FinishCurrentAction();
                creature->FinishCurrentAction();
                creature->ClearCurrentAction();
            }
        }
    }
}
