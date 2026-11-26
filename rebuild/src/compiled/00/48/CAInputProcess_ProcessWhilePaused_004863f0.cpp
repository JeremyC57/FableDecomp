// CAInputProcess::ProcessWhilePaused() const -- returns false: the process does not run while the game is paused
// Leaf: ?ProcessWhilePaused@CAInputProcess@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004863F0, 3 bytes: `xor al,al ; ret`; reached only through vftable slot 7.
// Identity: FableWin.pdb ?ProcessWhilePaused@CAInputProcess@@UBE_NXZ; CAInputProcess vftable 0x0129CA10 slot 7 (RTTI .?AVCAInputProcess@@), inherited by CInputProcessGameBase 0x0125BE98 and 43 family tables.
// NF477 (r467d / r467b, 2026-11-26): was the stand-in ?CAInputProcess_ProcessWhilePaused@@YI_NXZ (no virtual on the shared header); respelt onto engine/CAInputProcess.h
//   FABLE_RETAIL_INPUTPROCESS_VTABLE (the family's retail virtual list, declaration only), so the vftables 0x00488D10 emits
//   name this row at the slot. No body change.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CAInputProcess.h"

bool CAInputProcess::ProcessWhilePaused() const
{
    return false;
}
