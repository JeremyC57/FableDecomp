// CCreatureModeDef::Copy(const CDefClassBase* other) -- copy every member of another CCreatureModeDef: `*this = *static_cast<const CCreatureModeDef*>(other)` (tail call of the operator=)
// Leaf: ?Copy@CCreatureModeDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E2963, 5 bytes: a thiscall tail call, `jmp` to CCreatureModeDef::operator= 0x004E2968 (the next body).
// Identity (NF425, 2026-11-25): self ?Copy@CCreatureModeDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin ; target
//   ??4CCreatureModeDef@@QAEAAV0@ABV0@@Z: bound at 0x004E2968 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Option S unit (NF425): this function's one body lives in its retail TU-mate's row 0x004E2968 (??4CCreatureModeDef@@QAEAAV0@ABV0@@Z),
//   which already defines it (to emit the operator= it jumps to); both PDBs put the two in one object: Ego_r tc_register.obj (Copy idx
//   1538, operator= idx 1500); FableWin tc_register.obj (Copy idx 1411, operator= idx 1414). The row includes that source (no code of
//   its own), as the EH funclet rows include their parent; that object's Copy grades against this address. Before the unit this row
//   carried a second definition under the stand-in _CCreatureModeDef_Copy.
#include "../src/compiled/00/4e/CCreatureModeDef_operator_004e2968.cpp"
