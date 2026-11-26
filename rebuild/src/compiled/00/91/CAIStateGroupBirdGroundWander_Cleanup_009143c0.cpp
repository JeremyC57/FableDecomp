// CAIStateGroup_BirdGroundWander::Cleanup() -- records the frame the bird finished walking: FrameLastFinishedWalking = the world's frame
// Leaf: ?Cleanup@CAIStateGroup_BirdGroundWander@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009143C0, thiscall; vftable 0x01289670 slot 9 (its only retail reference).
// Identity (NF475, r467b unit with ctor 0x008FD880): RTTI .?AVCAIStateGroup_BirdGroundWander@@ vtable 0x01289670 slot 9 = 0x009143C0 +
//   Ego_r.pdb / FableWin.pdb ?Cleanup@CAIStateGroup_BirdGroundWander@@UAEXXZ (public override). Was the free __fastcall stand-in
//   ?CAIStateGroup_BirdGroundWander_Cleanup@@YIXPAUCAIStateGroup_BirdGroundWander@@@Z.
// Callees: CAIStateGroupBase::PeekWorld @ 0x008FCFA0 (?PeekWorld@CAIStateGroupBase@@IBEABVCWorld@@XZ; result unused: the object expression
//   of the static call, as in the landed sibling 0x009143E0 IsRunnable; was the stand-in ?h1_9143c0@@YIXPAUCAIStateGroup_BirdGroundWander@@@Z),
//   CWorld::GetFrame @ 0x0049D870 (?GetFrame@CWorld@@SIJXZ).
// Model: engine/CAIStateGroup_BirdGroundWander.h retail view FABLE_RETAIL_CAISTATEGROUP_BIRDGROUNDWANDER (the ctor row's class).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUP_BIRDGROUNDWANDER
#include "engine/CWorld.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIStateGroup_BirdGroundWander.h"

void CAIStateGroup_BirdGroundWander::Cleanup()
{
    FrameLastFinishedWalking = PeekWorld().GetFrame();
}
