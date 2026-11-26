// CInputProcessInventoryMagicScreen::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input) -- forwards the event to the base screen handling, CInputProcessInventoryBase::ProcessInput (a tail jump)
// Leaf: ?ProcessInput@CInputProcessInventoryMagicScreen@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00689C20, 5 bytes: `jmp 0x006894C0` (CInputProcessInventoryBase::ProcessInput); reached only through vftable slot 4.
// Identity: Ego_r.pdb / FableWin.pdb ?ProcessInput@CInputProcessInventoryMagicScreen@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z; retail vftable 0x01237858 slot 4 (RTTI .?AVCInputProcessInventoryMagicScreen@@).
// NF477 (r467d / r467b, 2026-11-26): was the stand-in _CInputProcessInventoryMagicScreen_ProcessInput (no virtual on the shared header); respelt onto engine/CInputProcessInventoryMagicScreen.h
//   FABLE_RETAIL_INPUTPROCESS_VTABLE (the family's retail virtual list, declaration only), so the vftables 0x00488D10 emits
//   name this row at the slot. No body change.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessInventoryMagicScreen.h"

void CInputProcessInventoryMagicScreen::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input)
{
    CInputProcessInventoryBase::ProcessInput(event, processed_input);
}
