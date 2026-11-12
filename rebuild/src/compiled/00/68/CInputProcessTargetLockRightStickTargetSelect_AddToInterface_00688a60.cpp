// CInputProcessTargetLockRightStickTargetSelect::AddToInterface(CInputProcessManager& manager) -- clear both changed-target flags, then the base registration
// Leaf: ?AddToInterface@CInputProcessTargetLockRightStickTargetSelect@@UAE_NAAVCInputProcessManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00688A60, 13 bytes: `xor al,al ; mov [ecx+28h],al ; mov [ecx+29h],al ; jmp 0x00687A70`.
// NF28 (2026-11-12, queue land1011eh): was landed through the stand-in _Methods::AddToInterface() calling a free __fastcall base_ati;
//   the jump target 0x00687A70 is ?AddToInterface@CInputProcessGameBase@@UAE_NAAVCInputProcessManager@@@Z (Ego_r.pdb 0x006B2B80 /
//   FableWin.pdb 0x021D3CB0). This override: Ego_r.pdb 0x006B41D0 ?AddToInterface@CInputProcessTargetLockRightStickTargetSelect@@UAE_NAAVCInputProcessManager@@@Z.
// Model: engine/CInputProcessTargetLockRightStickTargetSelect.h (ChangedTargetRight +0x28 / ChangedTargetLeft +0x29) with the
//   FABLE_RETAIL_CINPUTPROCESSTARGETLOCKRIGHTSTICKTARGETSELECT_ADDTOINTERFACE declaration opt-in.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CINPUTPROCESSTARGETLOCKRIGHTSTICKTARGETSELECT_ADDTOINTERFACE
#include "engine/CInputProcessTargetLockRightStickTargetSelect.h"

bool CInputProcessTargetLockRightStickTargetSelect::AddToInterface(CInputProcessManager& manager)
{
    ChangedTargetRight = false;
    ChangedTargetLeft = false;
    return CInputProcessGameBase::AddToInterface(manager);
}
