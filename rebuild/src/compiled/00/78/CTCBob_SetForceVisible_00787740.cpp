// CTCBob::SetForceVisible() -- force the bobbing thing visible and run its frame update at once
// Leaf: ?SetForceVisible@CTCBob@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00787740, 9 bytes: `mov eax,[ecx] ; mov byte [ecx+30h],1 ; jmp [eax+24h]` (virtual tail call).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?SetForceVisible@CTCBob@@QAEXXZ with Ego_r CTCBob::ForceVisible at +0x30 (the byte
//   stored) and slot 9 = CTCBase::FrameUpdate (CTCBob overrides it); the sole caller is the landed CGameScriptInterface::ForceShipsVisible
//   0x008A7F60, which queries TC 242 things and calls this on each CTCBob. The catalog's "CAIStateGroup_MinionBehaviourCrowdBase
//   Cleanup" was a propagated label.
// Callees: virtual CTCBob::FrameUpdate (slot 9).
// Header: engine/CTCBob.h (new).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBob.h"
void CTCBob::SetForceVisible()
{
    ForceVisible = true;
    FrameUpdate();
}