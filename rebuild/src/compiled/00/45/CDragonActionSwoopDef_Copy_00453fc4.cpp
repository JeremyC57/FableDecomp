// CDragonActionSwoopDef::Copy(const CDefClassBase* other) -- copy every member of another CDragonActionSwoopDef: `*this = *static_cast<const CDragonActionSwoopDef*>(other)` (tail call of the operator=)
// Leaf: ?Copy@CDragonActionSwoopDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00453FC4, 5 bytes: a thiscall tail call, `jmp` to CDragonActionSwoopDef::operator= 0x00453FC9 (the next body).
// Identity (NF425, 2026-11-25): self ?Copy@CDragonActionSwoopDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin ; target
//   ??4CDragonActionSwoopDef@@QAEAAV0@ABV0@@Z: bound at 0x00453FC9 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of
//   the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Option S unit (NF425): this function's one body lives in its retail TU-mate's row 0x00453FC9
//   (??4CDragonActionSwoopDef@@QAEAAV0@ABV0@@Z), which already defines it (to emit the operator= it jumps to); both PDBs put the two in
//   one object: Ego_r definition_manager.obj (Copy idx 441, operator= idx 383); FableWin definition_manager.obj (Copy idx 1283,
//   operator= idx 1286). The row includes that source (no code of its own), as the EH funclet rows include their parent; that object's
//   Copy grades against this address. Before the unit this row carried a second definition under the stand-in
//   ?CDragonActionSwoopDef_Copy@@YIXPAVCDragonActionSwoopDef@@PBV1@@Z.
#include "../src/compiled/00/45/CDragonActionSwoopDef_operator_00453fc9.cpp"
