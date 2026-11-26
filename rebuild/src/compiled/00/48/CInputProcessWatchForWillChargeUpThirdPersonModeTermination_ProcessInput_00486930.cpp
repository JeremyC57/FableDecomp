// CInputProcessWatchForWillChargeUpThirdPersonModeTermination::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input) -- ignores every event: an empty ProcessInput
// Leaf: ?ProcessInput@CInputProcessWatchForWillChargeUpThirdPersonModeTermination@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00486930, 3 bytes: `ret 8`; reached only through vftable slot 4.
// Identity: FableWin.pdb ?ProcessInput@CInputProcessWatchForWillChargeUpThirdPersonModeTermination@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z; retail vftable 0x01237BA8 slot 4 (RTTI .?AVCInputProcessWatchForWillChargeUpThirdPersonModeTermination@@).
// NF477 (r467d / r467b, 2026-11-26): was the stand-in ?CInputProcessWatchForWillChargeUpThirdPersonModeTermination_ProcessInput@@YGXHH@Z (no virtual on the shared header); respelt onto engine/CInputProcessWatchForWillChargeUpThirdPersonModeTermination.h
//   FABLE_RETAIL_INPUTPROCESS_VTABLE (the family's retail virtual list, declaration only), so the vftables 0x00488D10 emits
//   name this row at the slot. No body change.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessWatchForWillChargeUpThirdPersonModeTermination.h"

void CInputProcessWatchForWillChargeUpThirdPersonModeTermination::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input)
{
}
