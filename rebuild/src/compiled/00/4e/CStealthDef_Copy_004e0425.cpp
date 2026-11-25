// CStealthDef::Copy(const CDefClassBase* other) -- copy every member of another CStealthDef: `*this = *static_cast<const CStealthDef*>(other)` (tail call of the operator=)
// Leaf: ?Copy@CStealthDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0425, 5 bytes: a thiscall tail call, `jmp` to CStealthDef::operator= 0x004E042A (the next body).
// Identity (NF425, 2026-11-25): self ?Copy@CStealthDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin ; target
//   ??4CStealthDef@@QAEAAV0@ABV0@@Z: bound at 0x004E042A (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Option S unit (NF425): this function's one body lives in its retail TU-mate's row 0x004E042A (??4CStealthDef@@QAEAAV0@ABV0@@Z), which
//   already defines it (to emit the operator= it jumps to); both PDBs put the two in one object: Ego_r tc_register.obj (Copy idx 1327,
//   operator= idx 1195); FableWin tc_register.obj (Copy idx 649, operator= idx 652). The row includes that source (no code of its own),
//   as the EH funclet rows include their parent; that object's Copy grades against this address. Before the unit this row carried a
//   second definition under the stand-in _CStealthDef_Copy.
#include "../src/compiled/00/4e/CStealthDef_operator_004e042a.cpp"
