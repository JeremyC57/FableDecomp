// CScriptGameResourceObjectScriptedThing::ClearAllActions() -- when the scripted thing is alive and a creature, clear its queued actions and finish its current one
// Leaf: ?ClearAllActions@CScriptGameResourceObjectScriptedThing@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00903920 (vtable 0x0128D86C slot 21).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 four times (on PThing at +0x14), CThingCreatureBase::ClearQueuedActions @ 0x00663600,
//   CThingCreatureBase::FinishCurrentAction @ 0x00662650 (tail jump). Guards: null, CThing::Dead (bit 0 of +0x91), the creature type bit
//   (CThing::GetTypeFlags() & 8, `test byte ptr [eax+6Ch],8`).
// Identity (two sources): Ego_r.pdb 0x0092A230 + FableWin.pdb 0x02B4C080 ?ClearAllActions@CScriptGameResourceObjectScriptedThing@@UAEXXZ, the
//   override in vtable 0x0128D86C slot 21 (base slot 21 0x007E7400); no landed row references 0x00903920 (exe scan: the vtable only).
// NF171 (2026-11-20, q11936): ported onto the shared headers (engine/CScriptGameResourceObjectScriptedThing.h with the NF171 opt-in
//   FABLE_RETAIL_SCRIPTEDTHING_CLEARALLACTIONS, the retail CThing / CThingCreatureBase chain with FINISHACTION / CLEARQUEUED): 0x00662650 binds
//   ?FinishCurrentAction@CThingCreatureBase@@QAEXXZ and 0x00663600 ?ClearQueuedActions@CThingCreatureBase@@QAEXXZ (were CThing_Retail stand-ins);
//   own leaf was ?ClearAllActions@CScriptGameResourceObjectScriptedThing_Methods@@QAEXXZ. The creature is the CThing -> CThingCreatureBase
//   downcast after the type-bit test (both PDB classes); PThing is read through FableWin ?Get@?$CIntelligentPointer@VCThing@@@@QBEPAVCThing@@XZ
//   (inline: GetPItem 0x00A01B50 + static_cast).
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

void CScriptGameResourceObjectScriptedThing::ClearAllActions()
{
    if (PThing.Get()) {
        if (!PThing.Get()->IsDead()) {
            if (PThing.Get()->GetTypeFlags() & 8) {
                CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(PThing.Get());
                creature->ClearQueuedActions();
                creature->FinishCurrentAction();
            }
        }
    }
}
