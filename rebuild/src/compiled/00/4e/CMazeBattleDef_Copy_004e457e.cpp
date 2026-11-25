// CMazeBattleDef::Copy(const CDefClassBase* other) -- copy every member of another CMazeBattleDef: `*this = *static_cast<const CMazeBattleDef*>(other)` (tail call of the operator=)
// Leaf: ?Copy@CMazeBattleDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E457E, 5 bytes: a thiscall tail call, `jmp` to CMazeBattleDef::operator= 0x004E4583 (the next body).
// Identity (NF425, 2026-11-25): self ?Copy@CMazeBattleDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin ; target
//   ??4CMazeBattleDef@@QAEAAV0@ABV0@@Z: bound at 0x004E4583 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Option S unit (NF425): this function's one body lives in its retail TU-mate's row 0x004E4583 (??4CMazeBattleDef@@QAEAAV0@ABV0@@Z),
//   which already defines it (to emit the operator= it jumps to); both PDBs put the two in one object: Ego_r tc_register.obj (Copy idx
//   2515, operator= idx 2443); FableWin tc_register.obj (Copy idx 678, operator= idx 681). The row includes that source (no code of its
//   own), as the EH funclet rows include their parent; that object's Copy grades against this address. Before the unit this row carried
//   a second definition under the stand-in _CMazeBattleDef_Copy.
#include "../src/compiled/00/4e/CMazeBattleDef_operator_004e4583.cpp"
