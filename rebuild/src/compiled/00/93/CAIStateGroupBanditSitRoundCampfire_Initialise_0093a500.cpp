// CAIStateGroup_BanditSitRoundCampfire::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&)
// Leaf: ?Initialise@CAIStateGroup_BanditSitRoundCampfire@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093A500, length 48 bytes (+ int3 padding). Vtable slot 1 of
// CAIStateGroup_BanditSitRoundCampfire (0x0128B78C), its only retail reference.
// Callees: CAIStateGroupBase::Initialise @ 0x008FCEC0 (all five arguments forwarded).
// After the base Initialise the three bandit timers are reset in source order
// (FrameToNextSitOnSeat +0x24, WasCarryingWeapon +0x2c, FrameToNextCheckIsRunnable +0x28:
// retail stores them 0x24, 0x2c, 0x28 from one zeroed eax).
// Identity (NF476, the last emitters of the T3 nf374 long-priority stand-in): Ego_r.pdb / FableWin.pdb
//   ?Initialise@CAIStateGroup_BanditSitRoundCampfire@@UAE...JW4EScriptAIPriority@@...@Z (was ...JJ...). The class is the shared header's
//   (engine/CAIStateGroup_BanditSitRoundCampfire.h, both PDB records, the view of the landed sibling 0x0093A890) with the add-only
//   FABLE_RETAIL_CAISTATEGROUP_BANDITSITROUNDCAMPFIRE_INITIALISE slot-1 declaration; the base call binds the landed self
//   ?Initialise@CAIStateGroupBase@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z (was the
//   ?Initialise@CAIStateGroupBase@@UAE..JJ..@Z stand-in of the HEAD row's local base model and `typedef long EScriptAIPriority`).
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_BANDITSITROUNDCAMPFIRE_INITIALISE
#include "engine/CAIStateGroup_BanditSitRoundCampfire.h"

void CAIStateGroup_BanditSitRoundCampfire::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name)
{
    CAIStateGroupBase::Initialise(brain, creature, priority, scriptPriority, name);
    FrameToNextSitOnSeat = 0;
    WasCarryingWeapon = false;
    FrameToNextCheckIsRunnable = 0;
}
