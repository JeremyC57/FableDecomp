// CInputProcessDead::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input) -- ignores every event: an empty ProcessInput
// Leaf: ?ProcessInput@CInputProcessDead@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00688A00, 3 bytes: `ret 8`; reached only through vftable slot 4.
// Identity: Ego_r.pdb / FableWin.pdb ?ProcessInput@CInputProcessDead@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z; retail vftable 0x0123762C slot 4 (RTTI .?AVCInputProcessDead@@).
// NF477 (r467d / r467b, 2026-11-26): was the stand-in ?CInputProcessDead_ProcessInput@@YIXPAVCInputProcessDead@@HABVCInputEvent@@AAVCProcessedInput@@@Z (no virtual on the shared header); respelt onto engine/CInputProcessDead.h
//   FABLE_RETAIL_INPUTPROCESS_VTABLE (the family's retail virtual list, declaration only), so the vftables 0x00488D10 emits
//   name this row at the slot. No body change.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessDead.h"

void CInputProcessDead::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input)
{
}
