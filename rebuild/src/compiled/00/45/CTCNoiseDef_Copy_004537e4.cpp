// CTCNoiseDef::Copy(const CDefClassBase* other) -- copy every member of another CTCNoiseDef: `*this = *static_cast<const CTCNoiseDef*>(other)` (tail call of the operator=)
// Leaf: ?Copy@CTCNoiseDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004537E4, 5 bytes: a thiscall tail call, `jmp` to CTCNoiseDef::operator= 0x004537E9 (the next body).
// Identity (NF425, 2026-11-25): self ?Copy@CTCNoiseDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin ; target
//   ??4CTCNoiseDef@@QAEAAV0@ABV0@@Z: bound at 0x004537E9 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Option S unit (NF425): this function's one body lives in its retail TU-mate's row 0x004537E9 (??4CTCNoiseDef@@QAEAAV0@ABV0@@Z), which
//   already defines it (to emit the operator= it jumps to); both PDBs put the two in one object: Ego_r definition_manager.obj (Copy idx
//   419, operator= idx 339); FableWin definition_manager.obj (Copy idx 387, operator= idx 390). The row includes that source (no code
//   of its own), as the EH funclet rows include their parent; that object's Copy grades against this address. Before the unit this row
//   carried a second definition under the stand-in ?CTCNoiseDef_Copy@@YIXPAVCTCNoiseDef@@PAXPBVCDefClassBase@@@Z.
#include "../src/compiled/00/45/CTCNoiseDef_operator_004537e9.cpp"
