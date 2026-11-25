// CDegradableDef::Copy(const CDefClassBase* other) -- copy every member of another CDegradableDef: `*this = *static_cast<const CDegradableDef*>(other)` (tail call of the operator=)
// Leaf: ?Copy@CDegradableDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045442F, 5 bytes: a thiscall tail call, `jmp` to CDegradableDef::operator= 0x00454434 (the next body).
// Identity (NF425, 2026-11-25): self ?Copy@CDegradableDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin ; target
//   ??4CDegradableDef@@QAEAAV0@ABV0@@Z: bound at 0x00454434 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Option S unit (NF425): this function's one body lives in its retail TU-mate's row 0x00454434 (??4CDegradableDef@@QAEAAV0@ABV0@@Z),
//   which already defines it (to emit the operator= it jumps to); both PDBs put the two in one object: Ego_r definition_manager.obj
//   (Copy idx 632, operator= idx 554); FableWin definition_manager.obj (Copy idx 433, operator= idx 435). The row includes that source
//   (no code of its own), as the EH funclet rows include their parent; that object's Copy grades against this address. Before the unit
//   this row carried a second definition under the stand-in ?CDegradableDef_Copy@@YIXPAVCDegradableDef@@PAXPBV1@@Z.
#include "../src/compiled/00/45/CDegradableDef_operator_00454434.cpp"
