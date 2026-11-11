// CTCTargetingPlayer::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCTargetingPlayer@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E231A, thiscall, `ret`.
// Identity: slot 8 of CTCTargetingPlayer's vtable 0x0124280C (defined here: the slot holds this body in 1 vtable(s), all of CTCTargetingPlayer or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 22 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCTargetingPlayer@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCTargetingPlayer.h (opt-ins as in the landed row CTCTargetingPlayer_SetAsTargetNextThingInDirection_008023c0.cpp).
#define FABLE_RETAIL_CTCTARGETINGPLAYER_VSLOTS
#include "engine/CTCTargetingPlayer.h"
bool CTCTargetingPlayer::HasFrameUpdate() const
{
    return true;
}
