// CCreatureAction_UnsheatheItemFromInventory::CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase& creature, CThing& item,
//   CCreatureActionBase* followUp, long priority) -- the base action on the item's unsheathe animation (looked up by name, transition 4),
//   no sounds, interruption 100; keeps the item and the follow-up action
// Leaf: ??0CCreatureAction_UnsheatheItemFromInventory@@QAE@AAVCThingCreatureBase@@AAVCThing@@PAVCCreatureActionBase@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A0150, 273 bytes, thiscall `ret 10h` (0x006A0238), no EH frame.
// Shared headers (agentNF71 round 7; the landed row modelled ten classes locally): engine/CCreatureAction_UnsheatheItemFromInventory.h on the
//   retail CCreatureActionBase (FABLE_RETAIL_CCREATUREACTIONBASE + _ANIMCTOR: ctor 0x00693B30 and the protected static GetRandomAnimFromName
//   0x006924B0), the private static GetAnimationToUse 0x0069F980, engine/CAnimationEntry.h (overlay opt-in TRANSITION_CTOR: Ego_r.pdb /
//   FableWin.pdb ??0CAnimationEntry@@QAE@ABV0@J@Z 0x005DEC70) whose CIVCountedPointer<CAnimComponentMap> temporary is released in place
//   (FABLE_RETAIL_CIVCOUNTEDPOINTER + RELEASE_MEMBER + CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES, the chunk 851 form: `mov edi,[eax+4] ;
//   mov ecx,eax ; add eax,4 ; dec edi` at 0x006A01B3..0x006A01BB), CSoundPair, CIntelligentPointer<CThing> and CCountedPointer<CCreatureActionBase>
//   (its count block stores DeleteData 0x00668610).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_ANIMCTOR
#define FABLE_RETAIL_CANIMATIONENTRY_TRANSITION_CTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CSoundPair.h"
#include "engine/CAnimationEntry.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_UnsheatheItemFromInventory.h"
CCreatureAction_UnsheatheItemFromInventory::CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase& creature, CThing& item,
        CCreatureActionBase* followUp, long priority)
    : CCreatureActionBase(creature, -1.0f, 0.55f, CAnimationEntry(GetRandomAnimFromName(creature, GetAnimationToUse(creature, item)), 4),
                          CSoundPair(), 100, priority)
    , PUnsheatheItem(&item)
    , PFollowUpAction(followUp)
{
}
