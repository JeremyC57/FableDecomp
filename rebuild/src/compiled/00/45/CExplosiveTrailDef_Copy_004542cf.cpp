// CExplosiveTrailDef::Copy(const CDefClassBase* other) -- copy every member of another CExplosiveTrailDef: `*this = *static_cast<const CExplosiveTrailDef*>(other)` (tail call of the operator=)
// Leaf: ?Copy@CExplosiveTrailDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004542CF, 5 bytes: a thiscall tail call, `jmp` to CExplosiveTrailDef::operator= 0x004542D4 (the next body).
// Identity (NF425, 2026-11-25): self ?Copy@CExplosiveTrailDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin ; target
//   ??4CExplosiveTrailDef@@QAEAAV0@ABV0@@Z: bound at 0x004542D4 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of
//   the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Option S unit (NF425): this function's one body lives in its retail TU-mate's row 0x004542D4
//   (??4CExplosiveTrailDef@@QAEAAV0@ABV0@@Z), which already defines it (to emit the operator= it jumps to); both PDBs put the two in
//   one object: Ego_r definition_manager.obj (Copy idx 451, operator= idx 398); FableWin definition_manager.obj (Copy idx 1371,
//   operator= idx 1374). The row includes that source (no code of its own), as the EH funclet rows include their parent; that object's
//   Copy grades against this address. Before the unit this row carried a second definition under the stand-in
//   ?CExplosiveTrailDef_Copy@@YIXPAVCExplosiveTrailDef@@PBX@Z.
#include "../src/compiled/00/45/CExplosiveTrailDef_operator_004542d4.cpp"
