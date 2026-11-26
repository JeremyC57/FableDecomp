// CInputProcessJumpingAndRolling::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- rolling first, then jumping, each given the event and the processed-input record
// Leaf: ?ProcessInput@CInputProcessJumpingAndRolling@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// NF478 (r467b, NAME_FIXES_QUEUE nf477, 2026-11-26): was the non-virtual ?ProcessInput@CInputProcessJumpingAndRolling@@QAE... on a local class
//   model; respelt onto engine/CInputProcessJumpingAndRolling.h FABLE_RETAIL_INPUTPROCESS_VTABLE (Ego_r.pdb / FableWin.pdb public virtual, retail
//   vftable slot 4), so the family vftables 0x00488D10 emits name this row. The base queries come from the retail view of
//   engine/CInputProcessGameBase.h; the two private parts from the add-only opt-in FABLE_RETAIL_CINPUTPROCESSJUMPINGANDROLLING_PROCESSPARTS. No body change.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A4700, length 35 bytes. Vtable 0x01238050 slot 4.
// Callees: CInputProcessJumpingAndRolling::ProcessRolling @ 0x006A40D0, CInputProcessJumpingAndRolling::ProcessJumping @ 0x006A4040.
// Shape: both references are kept in callee-saved registers (ebx, edi) across the first call and pushed again for the second, `ret 8`.
class CInputEvent;
class CProcessedInput;
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#define FABLE_RETAIL_CINPUTPROCESSJUMPINGANDROLLING_PROCESSPARTS   // NF478 overlay opt-in: the private ProcessJumping / ProcessRolling (Ego_r / FableWin)
#include "engine/CInputProcessJumpingAndRolling.h"

void CInputProcessJumpingAndRolling::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    ProcessRolling(event, processed);
    ProcessJumping(event, processed);
}