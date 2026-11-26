// CAIStateGroup_BanditKingLunge::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&) -- clear the
// last-lunge time, then the base initialisation (a tail jump).
// Leaf: ?Initialise@CAIStateGroup_BanditKingLunge@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009612E0, 12 bytes: `and dword ptr [ecx+1Ch],0 ; jmp 0x008FCEC0`. Vtable slot 1 of
// CAIStateGroup_BanditKingLunge (0x0128CA74), its only retail reference.
// Identity (NF476, T3 nf374 binder): Ego_r.pdb / FableWin.pdb ?Initialise@CAIStateGroup_BanditKingLunge@@UAE...JW4EScriptAIPriority@@...@Z
//   (was the made-up free __fastcall @CAIStateGroup_BanditKingLunge_Initialise@28 over `typedef long EScriptAIPriority`). The class is the
//   header's retail view (engine/CAIStateGroup_BanditKingLunge.h FABLE_RETAIL_CAISTATEGROUP_BANDITKINGLUNGE, the view of the landed
//   siblings 0x00961880 / 0x00901298, + the add-only FABLE_RETAIL_CAISTATEGROUP_BANDITKINGLUNGE_INITIALISE slot-1 declaration;
//   Ego_r TimeOfLastLunge +0x1c), so the base call binds the landed self
//   ?Initialise@CAIStateGroupBase@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z (was the made-up
//   @CAIStateGroupBase_Initialise@28).
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_BANDITKINGLUNGE
#define FABLE_RETAIL_CAISTATEGROUP_BANDITKINGLUNGE_INITIALISE
#include "engine/CAIStateGroup_BanditKingLunge.h"

void CAIStateGroup_BanditKingLunge::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name)
{
    TimeOfLastLunge = 0;
    CAIStateGroupBase::Initialise(brain, creature, priority, scriptPriority, name);
}
