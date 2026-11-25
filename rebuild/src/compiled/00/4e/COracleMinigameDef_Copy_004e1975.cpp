// COracleMinigameDef::Copy(const CDefClassBase* other) -- copy every member of another COracleMinigameDef: `*this = *static_cast<const COracleMinigameDef*>(other)` (tail call of the operator=)
// Leaf: ?Copy@COracleMinigameDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E1975, 5 bytes: a thiscall tail call, `jmp` to COracleMinigameDef::operator= 0x004E197A (the next body).
// Identity (NF425, 2026-11-25): self ?Copy@COracleMinigameDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin ; target
//   ??4COracleMinigameDef@@QAEAAV0@ABV0@@Z: bound at 0x004E197A (owner Leaf); Ego_r/FableWin name set; its body opens with a call of
//   the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Option S unit (NF425): this function's one body lives in its retail TU-mate's row 0x004E197A
//   (??4COracleMinigameDef@@QAEAAV0@ABV0@@Z), which already defines it (to emit the operator= it jumps to); both PDBs put the two in
//   one object: Ego_r tc_register.obj (Copy idx 1425, operator= idx 1276); FableWin tc_register.obj (Copy idx 2536, operator= idx
//   2539). The row includes that source (no code of its own), as the EH funclet rows include their parent; that object's Copy grades
//   against this address. Before the unit this row carried a second definition under the stand-in _COracleMinigameDef_Copy.
#include "../src/compiled/00/4e/COracleMinigameDef_operator_004e197a.cpp"
