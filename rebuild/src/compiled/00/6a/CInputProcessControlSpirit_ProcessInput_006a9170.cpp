// CInputProcessControlSpirit::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input) -- the spirit-control process takes no input events
// Leaf: ?ProcessInput@CInputProcessControlSpirit@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A9170, 3 bytes: `ret 8`. CInputProcessControlSpirit vftable slot 4 (RTTI; NF477 audit: one `.rdata` dword, no call).
// Identity: Ego_r.pdb + FableWin.pdb ?ProcessInput@CInputProcessControlSpirit@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z (public virtual).
// NF478 (r467b, NAME_FIXES_QUEUE nf477, 2026-11-26): was the free stand-in ?CInputProcessControlSpirit_ProcessInput@@YGXHH@Z; respelt onto
//   engine/CInputProcessControlSpirit.h FABLE_RETAIL_INPUTPROCESS_VTABLE (resolves T3 nf324 "does not declare ProcessInput"). No body change.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessControlSpirit.h"

void CInputProcessControlSpirit::ProcessInput(const CInputEvent&, CProcessedInput&)
{
}
