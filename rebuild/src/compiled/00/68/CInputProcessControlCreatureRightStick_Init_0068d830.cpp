// CInputProcessControlCreatureRightStick::Init() -- clear the camera-mode push and look-around flags when the process is added
// Leaf: ?Init@CInputProcessControlCreatureRightStick@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0068D830; CInputProcessControlCreatureRightStick vftable 0x01237A40 slot 2 (RTTI; NF477 audit: one `.rdata` dword, no call).
// Identity: Ego_r.pdb + FableWin.pdb ?Init@CInputProcessControlCreatureRightStick@@UAEXXZ (public virtual, the CAInputProcess slot 2 override).
// NF478 (r467b, NAME_FIXES_QUEUE nf477, 2026-11-26): was the free stand-in ?CInputProcessControlCreatureRightStick_Init@@YIXPAVCInputProcessControlCreatureRightStick@@@Z;
//   respelt onto engine/CInputProcessControlCreatureRightStick.h FABLE_RETAIL_INPUTPROCESS_VTABLE (resolves T3 nf324 "does not declare Init"). No body change.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessControlCreatureRightStick.h"

void CInputProcessControlCreatureRightStick::Init()
{
    PushingCameraModeUp = 0;
    PushingCameraModeDown = 0;
    LookingAround = 0;
}
