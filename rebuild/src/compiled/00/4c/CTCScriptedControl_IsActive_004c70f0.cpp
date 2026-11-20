// CTCScriptedControl::IsActive() const -- the scripted control is active while it is locked (+0x18) or its action list is not empty
// Leaf: ?IsActive@CTCScriptedControl@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C70F0, length 25 bytes: `mov al,[ecx+18h] ; test al,al ; jne T ; call 0x007122C0 ;
//   test al,al ; je T ; xor eax,eax ; ret ; T: mov eax,1 ; ret`.
// Callee: CTCScriptedControl::IsActionListEmpty() const @ 0x007122C0 (Ego_r.pdb / FableWin.pdb). Callers: 0x00659412, 0x00659478,
//   0x00667EB6, 0x0076CF0D.
// Naming (NF108, 2026-11-19, q4265): Ego_r.pdb 0x004C8FE0 + FableWin.pdb 0x01CF8CC0 ?IsActive@CTCScriptedControl@@QBE_NXZ (const bool);
//   was a local model returning long (QAEJXZ) with the callee as AreTasksEmpty. Re-authored on the shared engine/CTCScriptedControl.h
//   (Locked +0x18; overlay opt-in ISACTIONLISTEMPTY).
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ISACTIONLISTEMPTY
#include "engine/ETCInterfaceType.h"
#include "engine/CTCScriptedControl.h"

bool CTCScriptedControl::IsActive() const
{
    return Locked || !IsActionListEmpty();
}
