// CTrollBattleDef::Copy(const CDefClassBase* other) -- copy every member of another CTrollBattleDef: `*this = *static_cast<const CTrollBattleDef*>(other)` (tail call of the operator=)
// Leaf: ?Copy@CTrollBattleDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E47E3, 5 bytes: a thiscall tail call, `jmp` to CTrollBattleDef::operator= 0x004E47E8 (the next body).
// Identity (NF425, 2026-11-25): self ?Copy@CTrollBattleDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin ; target
//   ??4CTrollBattleDef@@QAEAAV0@ABV0@@Z: bound at 0x004E47E8 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Option S unit (NF425): this function's one body lives in its retail TU-mate's row 0x004E47E8 (??4CTrollBattleDef@@QAEAAV0@ABV0@@Z),
//   which already defines it (to emit the operator= it jumps to); both PDBs put the two in one object: Ego_r tc_register.obj (Copy idx
//   2521, operator= idx 2448); FableWin tc_register.obj (Copy idx 726, operator= idx 729). The row includes that source (no code of its
//   own), as the EH funclet rows include their parent; that object's Copy grades against this address. Before the unit this row carried
//   a second definition under the stand-in _CTrollBattleDef_Copy.
#include "../src/compiled/00/4e/CTrollBattleDef_operator_004e47e8.cpp"
