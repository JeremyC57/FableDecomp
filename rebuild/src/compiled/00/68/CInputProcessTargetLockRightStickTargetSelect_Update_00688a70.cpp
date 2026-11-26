// CInputProcessTargetLockRightStickTargetSelect::Update(CProcessedInput& processed_input) -- nothing to do per frame: an empty Update
// Leaf: ?Update@CInputProcessTargetLockRightStickTargetSelect@@UAEXAAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00688A70, 3 bytes: `ret 4`; reached only through vftable slot 6.
// Identity: Ego_r.pdb / FableWin.pdb ?Update@CInputProcessTargetLockRightStickTargetSelect@@UAEXAAVCProcessedInput@@@Z; retail vftable 0x012376A4 slot 6 (RTTI .?AVCInputProcessTargetLockRightStickTargetSelect@@).
// NF477 (r467d / r467b, 2026-11-26): was the stand-in ?CInputProcessTargetLockRightStickTargetSelect_Update@@YGXH@Z (no virtual on the shared header); respelt onto engine/CInputProcessTargetLockRightStickTargetSelect.h
//   FABLE_RETAIL_INPUTPROCESS_VTABLE (the family's retail virtual list, declaration only), so the vftables 0x00488D10 emits
//   name this row at the slot. No body change.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessTargetLockRightStickTargetSelect.h"

void CInputProcessTargetLockRightStickTargetSelect::Update(CProcessedInput& processed_input)
{
}
