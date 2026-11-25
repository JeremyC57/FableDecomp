// CScorpionKingBattleDef::Copy(const CDefClassBase* other) -- copy every member of another CScorpionKingBattleDef: `*this = *static_cast<const CScorpionKingBattleDef*>(other)` (tail call of the operator=)
// Leaf: ?Copy@CScorpionKingBattleDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E476F, 5 bytes: a thiscall tail call, `jmp` to CScorpionKingBattleDef::operator= 0x004E4774 (the next body).
// Identity (NF425, 2026-11-25): self ?Copy@CScorpionKingBattleDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin ; target
//   ??4CScorpionKingBattleDef@@QAEAAV0@ABV0@@Z: bound at 0x004E4774 (owner Leaf); Ego_r/FableWin name set; its body opens with a call
//   of the ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Option S unit (NF425): this function's one body lives in its retail TU-mate's row 0x004E4774
//   (??4CScorpionKingBattleDef@@QAEAAV0@ABV0@@Z), which already defines it (to emit the operator= it jumps to); both PDBs put the two
//   in one object: Ego_r tc_register.obj (Copy idx 2520, operator= idx 2447); FableWin tc_register.obj (Copy idx 718, operator= idx
//   721). The row includes that source (no code of its own), as the EH funclet rows include their parent; that object's Copy grades
//   against this address. Before the unit this row carried a second definition under the stand-in _CScorpionKingBattleDef_Copy.
#include "../src/compiled/00/4e/CScorpionKingBattleDef_operator_004e4774.cpp"
