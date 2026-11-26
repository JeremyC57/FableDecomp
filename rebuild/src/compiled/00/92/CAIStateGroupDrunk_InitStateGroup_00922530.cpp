// CAIStateGroup_Drunk::InitStateGroup(const CWorld&, CThingAICreature&) -- the drunk may talk to himself at once: FrameToNextTalkToSelf = 0
// Leaf: ?InitStateGroup@CAIStateGroup_Drunk@@UAEXABVCWorld@@AAVCThingAICreature@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00922530, thiscall, `ret 8`; vftable 0x0128A9A4 slot 13 (its only retail reference).
// Identity (NF475, r467b unit with ctor 0x008FE5B8): RTTI .?AVCAIStateGroup_Drunk@@ vtable 0x0128A9A4 slot 13 = 0x00922530 + Ego_r.pdb / FableWin.pdb
//   ?InitStateGroup@CAIStateGroup_Drunk@@UAEXABVCWorld@@AAVCThingAICreature@@@Z (public override; both arguments unused). Was the stand-in class
//   ?InitStateGroup@CAIStateGroup_Drunk_Methods@@QAEXHH@Z (T3 nf320: the class existed only under FABLE_RETAIL_CAISTATEGROUP_DRUNK_UPDATE_PASS27, the view
//   the ctor row 0x008FE5B8 compiles; InitStateGroup is declared in its add-only FABLE_RETAIL_CAISTATEGROUP_DRUNK_CTOR block, r474c).
// Model: engine/CAIStateGroup_Drunk.h retail view FABLE_RETAIL_CAISTATEGROUP_DRUNK_UPDATE_PASS27 + FABLE_RETAIL_CAISTATEGROUP_DRUNK_CTOR (the ctor row's class).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_DRUNK_UPDATE_PASS27
#define FABLE_RETAIL_CAISTATEGROUP_DRUNK_CTOR
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIStateGroup_Drunk.h"

void CAIStateGroup_Drunk::InitStateGroup(const CWorld&, CThingAICreature&)
{
    FrameToNextTalkToSelf = 0;
}
