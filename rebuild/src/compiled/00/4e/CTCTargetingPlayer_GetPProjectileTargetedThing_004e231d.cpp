// CTCTargetingPlayer::GetPProjectileTargetedThing() const -- the projectile-targeted thing, or 0 (CIntelligentPointer<CThing>::Get on PProjectileTargetedThing, +0xdc)
// Leaf: ?GetPProjectileTargetedThing@CTCTargetingPlayer@@UBEPAVCThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E231D, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 35 of CTCTargetingPlayer's vtable 0x0124280C (the slot holds this body in 1 vtable(s), owner CTCTargetingPlayer); the Xbox 360 Test PDB virtual order (third source) aligned on 22 TLC-named slots of that vtable puts GetPProjectileTargetedThing(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetPProjectileTargetedThing@CTCTargetingPlayer@@UBEPAVCThing@@XZ; no `ret` to check (NORET).
// Was: CTCTargetingPlayer::vfunc_35 (rebuild/src/compiled/00/4e/CTCTargetingPlayer_vfunc35_004e231d.cpp).
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
CThing* CTCTargetingPlayer::GetPProjectileTargetedThing() const
{
    return PProjectileTargetedThing.Get();
}
