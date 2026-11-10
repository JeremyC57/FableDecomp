// CModeStandBase::OnDeactivate() -- leaving the stand mode: release the six cached locomotion animations (stand, slow walk, walk, jog,
// run, sprint) and mark the mode inactive
// Leaf: ?OnDeactivate@CModeStandBase@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00861D00, 252 bytes: six expanded CCountedPointer releases (count decrement, DeleteFunc(Data) and
//   operator delete 0x00BFE9BC at zero, then Info = 0, Data = 0) over +0x38..+0x60, then `mov byte ptr [esi+18h],0` (CModeBase::Active).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?OnDeactivate@CModeStandBase@@UAEXXZ (public virtual, CModeBase slot 6); the landed
//   NCreatureMode::CModeDrained::OnDeactivate 0x008DA7A0 calls it by name (`CModeStandBase::OnDeactivate()`), and engine/CModeStandBase.h
//   (retail) declares it under FABLE_RETAIL_CMODESTANDBASE_DEACTIVATE with the six CCountedPointer members at +0x38..+0x60.
// Name fix (agentNF5 2026-11-10): was the derived stand-in `CModeStandBaseNative::OnDeactivate` (?OnDeactivate@CModeStandBaseNative@@QAEXXZ)
//   over the flattened layout; now the shared retail headers (CCountedPointer::ReleaseData is the expanded release).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_DEACTIVATE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
void CModeStandBase::OnDeactivate()
{
    PStandAnimation.ReleaseData();
    PSlowWalkAnimation.ReleaseData();
    PWalkAnimation.ReleaseData();
    PJogAnimation.ReleaseData();
    PRunAnimation.ReleaseData();
    PSprintAnimation.ReleaseData();
    Active = false;
}
