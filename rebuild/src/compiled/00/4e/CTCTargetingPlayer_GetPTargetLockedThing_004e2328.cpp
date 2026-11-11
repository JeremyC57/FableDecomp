// CTCTargetingPlayer::GetPTargetLockedThing() const -- the target-locked thing, or 0 (CIntelligentPointer<CThing>::Get on PTargetLockedThing, +0xe4)
// Leaf: ?GetPTargetLockedThing@CTCTargetingPlayer@@UBEPAVCThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E2328, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 36 of CTCTargetingPlayer's vtable 0x0124280C (the slot holds this body in 1 vtable(s), owner CTCTargetingPlayer); the Xbox 360 Test PDB virtual order (third source) aligned on 22 TLC-named slots of that vtable puts GetPTargetLockedThing(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetPTargetLockedThing@CTCTargetingPlayer@@UBEPAVCThing@@XZ; no `ret` to check (NORET).
// Was: CTCTargetingPlayer::vfunc_36 (rebuild/src/compiled/00/4e/CTCTargetingPlayer_vfunc36_004e2328.cpp).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCTARGETINGPLAYER
#define FABLE_RETAIL_CTCTARGETINGPLAYER_VSLOTS3
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CTCTargetingPlayer.h"
CThing* CTCTargetingPlayer::GetPTargetLockedThing() const
{
    return PTargetLockedThing.Get();
}
