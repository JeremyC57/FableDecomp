// CAIStateGroup_StealStealableItem::IsRunnable() -- unless a cleanup is pending, for a creature whose follow-state component allows stealing: reset the action state, latch the stealable item and an escape point; runnable when both are alive
// Leaf: ?IsRunnable@CAIStateGroup_StealStealableItem@@UAE_NXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00964DA0, length 336 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (inlined CThing::PeekTC of interface 0xAA, bit 10 of +0x34), FindPStealableItem
//          @ 0x00964360, FindPEscapePoint(bool) @ 0x00964CA0, CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor
//          (temporaries CIntelligentPointer<CThing> vtable 0x01238C6C and CIntelligentPointer<const CThing> vtable
//          0x01237F1C).
// Shape: the component lookup goes through the const overload (retail calls the TU's const LowerBound copy) with
//          the if/assign pick; both pointers are assigned through frame temporaries with the named-item operator=;
//          the same-TU noinline const LowerBound body (engine/CVectorMap.h) lets the temporaries reuse the dead key slot (8-byte frame);
//          the result is the int-valued `&&` of two inline IS_THING_ALIVE tests (`mov eax,1` / `xor eax,eax`).
// Layout: engine/CAIStateGroup_StealStealableItem.h (PStealableItem +0x14, PEscapePoint +0x1c, ActionPointIndex +0x2c,
//          GotActionPoint +0x30, ReachedDestination +0x31, PickUpActionID +0x34, NeedsCleanup +0x38); the 0xAA (TCI_AI_SCRATCHPAD)
//          component's +0x68 / +0x6a flags are CTCAIScratchpad::StealStealableItems / TakeStealableItemToRandomDestination (Ego_r.pdb).
// NF141 (2026-11-20, q4994 / q5140 family): ported onto the retail engine/CAIStateGroup_StealStealableItem.h class (overlay opt-ins
//   _ISRUNNABLE, _FINDPSTEALABLEITEM, _FINDPESCAPEPOINT). Own leaf: the stand-in ?IsRunnable@CAIStateGroupStealStealableItemMethods@@QAE_NXZ
//   -> the PDB ?IsRunnable@CAIStateGroup_StealStealableItem@@UAE_NXZ (Ego_r.pdb 0x00989760 + FableWin.pdb 0x02C38320, public virtual, slot 11;
//   no landed row references 0x00964DA0, exe scan). Callees: 0x00964CA0 -> ?FindPEscapePoint@CAIStateGroup_StealStealableItem@@ABEPBVCThing@@_N@Z
//   (its self row moves to it in this hand-back) and 0x00964360 -> ?FindPStealableItem@CAIStateGroup_StealStealableItem@@ABEPAVCThing@@XZ
//   (Ego_r.pdb 0x00988C30 + FableWin.pdb 0x02C3AE50; a PDB spelling replacing this row's stand-in: the address keeps two spellings, its
//   self row's _Methods stand-in waits, NAME_FIXES). Interface 0xAA is TCI_AI_SCRATCHPAD (engine/ETCInterfaceType.h): the component is
//   CTCAIScratchpad (engine/CTCAIScratchpad.h; Ego_r.pdb StealStealableItems +0x68, TakeStealableItemToRandomDestination +0x6a), read through
//   CThing::PeekTC (FABLE_RETAIL_CTHING_GETTC_EXPANDED: const LowerBound 0x004365B0, CVectorMap.h noinline body); the pointer temporaries are
//   CIntelligentPointer<CThing> / <const CThing> (engine/CIntelligentPointer.h); IS_THING_ALIVE from engine/ThingAlive.h. Local CInterfaceMap /
//   CThing / CTCFollowerState / CIntelligentPointer* / IS_THING_ALIVE / CAIBrain / Methods models dropped.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_STEALSTEALABLEITEM
#define FABLE_RETAIL_CAISTATEGROUP_STEALSTEALABLEITEM_ISRUNNABLE
#define FABLE_RETAIL_CAISTATEGROUP_STEALSTEALABLEITEM_FINDPSTEALABLEITEM
#define FABLE_RETAIL_CAISTATEGROUP_STEALSTEALABLEITEM_FINDPESCAPEPOINT
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_StealStealableItem.h"

bool CAIStateGroup_StealStealableItem::IsRunnable()
{
    if (!NeedsCleanup) {
        const CTCAIScratchpad* scratchpad;
        GetCreature().PeekTC(TCI_AI_SCRATCHPAD, &scratchpad);
        if (scratchpad->StealStealableItems) {
            ActionPointIndex = -1;
            ReachedDestination = false;
            GotActionPoint = false;
            PickUpActionID = -1;
            PStealableItem = CIntelligentPointer<CThing>(FindPStealableItem());
            PEscapePoint = CIntelligentPointer<const CThing>(FindPEscapePoint(!scratchpad->TakeStealableItemToRandomDestination));
            return IS_THING_ALIVE(PStealableItem) && IS_THING_ALIVE(PEscapePoint);
        }
    }
    return false;
}
