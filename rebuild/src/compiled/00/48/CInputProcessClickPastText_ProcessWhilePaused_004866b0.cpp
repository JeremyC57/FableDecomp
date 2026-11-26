// CInputProcessClickPastText::ProcessWhilePaused() const -- returns true: the process keeps running while the game is paused
// Leaf: ?ProcessWhilePaused@CInputProcessClickPastText@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004866B0, 3 bytes: `mov al,1 ; ret`; reached only through vftable slot 7.
// Identity: FableWin.pdb ?ProcessWhilePaused@CInputProcessClickPastText@@UBE_NXZ; retail vftable 0x012378D8 slot 7 (RTTI .?AVCInputProcessClickPastText@@).
// NF477 (r467d / r467b, 2026-11-26): was the stand-in ?CInputProcessClickPastText_ProcessWhilePaused@@YI_NXZ (no virtual on the shared header); respelt onto engine/CInputProcessClickPastText.h
//   FABLE_RETAIL_INPUTPROCESS_VTABLE (the family's retail virtual list, declaration only), so the vftables 0x00488D10 emits
//   name this row at the slot. No body change.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessClickPastText.h"

bool CInputProcessClickPastText::ProcessWhilePaused() const
{
    return true;
}
