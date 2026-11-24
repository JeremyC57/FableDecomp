#pragma optimize("s",on)
// CAIStateGroup_BirdGroundWander::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&)
// Leaf: ?Initialise@CAIStateGroup_BirdGroundWander@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FD892, oracle length 13 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push ebp ; mov ebp,esp ; and dword ptr [ecx+14h],0 ; pop ebp ; jmp CAIStateGroupBase::Initialise
// Resets the group's frame counter at +0x14 and tail-calls the base Initialise @ 0x008FCEC0
// with all five arguments left in place. The frame pointer around a one-instruction body is the
// unoptimised build of this translation unit (lander pragma sweep).
// Body port (NF321, 2026-11-23): defined on the real class of engine/CAIStateGroup_BirdGroundWander.h (was the stand-in
//   CAIStateGroup_BirdGroundWander_Retail over CAIStateGroupBase_Retail); the override is declared by the add-only opt-in
//   FABLE_RETAIL_CAISTATEGROUP_BIRDGROUNDWANDER_INITIALISE_DECL (both PDB name sets; retail RTTI vtable 0x01289670 slot 1). The base call is
//   ?Initialise@CAIStateGroupBase@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z 0x008FCEC0 (RTTI label,
//   landed self row), FrameLastFinishedWalking the header's +0x14 member.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_BIRDGROUNDWANDER
#define FABLE_RETAIL_CAISTATEGROUP_BIRDGROUNDWANDER_INITIALISE_DECL
#include "engine/CAIStateGroup_BirdGroundWander.h"

void CAIStateGroup_BirdGroundWander::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name)
{
    this->FrameLastFinishedWalking = 0;
    CAIStateGroupBase::Initialise(brain, creature, priority, scriptPriority, name);
}
