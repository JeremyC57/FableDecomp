// CInputProcessPhotojournalCapture::ProcessWhilePaused() const -- returns true: the process keeps running while the game is paused
// Leaf: ?ProcessWhilePaused@CInputProcessPhotojournalCapture@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00486C30, 3 bytes: `mov al,1 ; ret`; reached only through vftable slot 7.
// Identity: FableWin.pdb ?ProcessWhilePaused@CInputProcessPhotojournalCapture@@UBE_NXZ; retail vftable 0x01237EC8 slot 7 (RTTI .?AVCInputProcessPhotojournalCapture@@).
// NF477 (r467d / r467b, 2026-11-26): was the stand-in ?ProcessWhilePaused@CInputProcessPhotojournalCapture@@QAE_NXZ (no virtual on the shared header); respelt onto engine/CInputProcessPhotojournalCapture.h
//   FABLE_RETAIL_INPUTPROCESS_VTABLE (the family's retail virtual list, declaration only), so the vftables 0x00488D10 emits
//   name this row at the slot. No body change.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessPhotojournalCapture.h"

bool CInputProcessPhotojournalCapture::ProcessWhilePaused() const
{
    return true;
}
