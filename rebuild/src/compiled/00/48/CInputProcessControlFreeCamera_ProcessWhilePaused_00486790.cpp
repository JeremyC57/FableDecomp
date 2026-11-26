// CInputProcessControlFreeCamera::ProcessWhilePaused() const -- returns true: the process keeps running while the game is paused
// Leaf: ?ProcessWhilePaused@CInputProcessControlFreeCamera@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00486790, 3 bytes: `mov al,1 ; ret`; reached only through vftable slot 7.
// Identity: FableWin.pdb ?ProcessWhilePaused@CInputProcessControlFreeCamera@@UBE_NXZ; retail vftable 0x012379C8 slot 7 (RTTI .?AVCInputProcessControlFreeCamera@@).
// NF477 (r467d / r467b, 2026-11-26): was the stand-in ?CInputProcessControlFreeCamera_ProcessWhilePaused@@YI_NXZ (no virtual on the shared header); respelt onto engine/CInputProcessControlFreeCamera.h
//   FABLE_RETAIL_INPUTPROCESS_VTABLE (the family's retail virtual list, declaration only), so the vftables 0x00488D10 emits
//   name this row at the slot. No body change.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessControlFreeCamera.h"

bool CInputProcessControlFreeCamera::ProcessWhilePaused() const
{
    return true;
}
