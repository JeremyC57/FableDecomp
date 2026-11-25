// CBonusItemDef::Copy(const CDefClassBase* other) -- copy every member of another CBonusItemDef: `*this = *static_cast<const CBonusItemDef*>(other)` (tail call of the operator=)
// Leaf: ?Copy@CBonusItemDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00453C53, 5 bytes: a thiscall tail call, `jmp` to CBonusItemDef::operator= 0x00453C58 (the next body).
// Identity (NF425, 2026-11-25): self ?Copy@CBonusItemDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin ; target
//   ??4CBonusItemDef@@QAEAAV0@ABV0@@Z: bound at 0x00453C58 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Option S unit (NF425): this function's one body lives in its retail TU-mate's row 0x00453C58 (??4CBonusItemDef@@QAEAAV0@ABV0@@Z),
//   which already defines it (to emit the operator= it jumps to); both PDBs put the two in one object: Ego_r definition_manager.obj
//   (Copy idx 433, operator= idx 370); FableWin definition_manager.obj (Copy idx 1120, operator= idx 1123). The row includes that
//   source (no code of its own), as the EH funclet rows include their parent; that object's Copy grades against this address. Before
//   the unit this row carried a second definition under the stand-in ?CBonusItemDef_Copy@@YIXPAVCBonusItemDef@@PBV1@@Z.
#include "../src/compiled/00/45/CBonusItemDef_operator_00453c58.cpp"
