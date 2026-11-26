// CInputProcessWatchForRangedWeaponThirdPersonModeTermination::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input) -- ignores every event: an empty ProcessInput
// Leaf: ?ProcessInput@CInputProcessWatchForRangedWeaponThirdPersonModeTermination@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00486900, 3 bytes: `ret 8`; reached only through vftable slot 4.
// Identity: FableWin.pdb ?ProcessInput@CInputProcessWatchForRangedWeaponThirdPersonModeTermination@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z; retail vftable 0x01237B80 slot 4 (RTTI .?AVCInputProcessWatchForRangedWeaponThirdPersonModeTermination@@).
// NF477 (r467d / r467b, 2026-11-26): was the stand-in ?CInputProcessWatchForRangedWeaponThirdPersonModeTermination_ProcessInput@@YGXHH@Z (no virtual on the shared header); respelt onto engine/CInputProcessWatchForRangedWeaponThirdPersonModeTermination.h
//   FABLE_RETAIL_INPUTPROCESS_VTABLE (the family's retail virtual list, declaration only), so the vftables 0x00488D10 emits
//   name this row at the slot. No body change.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessWatchForRangedWeaponThirdPersonModeTermination.h"

void CInputProcessWatchForRangedWeaponThirdPersonModeTermination::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input)
{
}
