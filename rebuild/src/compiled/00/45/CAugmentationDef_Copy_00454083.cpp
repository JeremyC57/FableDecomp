// CAugmentationDef::Copy(const CDefClassBase* other) -- copy every member of another CAugmentationDef: `*this = *static_cast<const CAugmentationDef*>(other)` (tail call of the operator=)
// Leaf: ?Copy@CAugmentationDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00454083, 5 bytes: a thiscall tail call, `jmp` to CAugmentationDef::operator= 0x00454088 (the next body).
// Identity (NF425, 2026-11-25): self ?Copy@CAugmentationDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin ; target
//   ??4CAugmentationDef@@QAEAAV0@ABV0@@Z: bound at 0x00454088 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Option S unit (NF425): this function's one body lives in its retail TU-mate's row 0x00454088 (??4CAugmentationDef@@QAEAAV0@ABV0@@Z),
//   which already defines it (to emit the operator= it jumps to); both PDBs put the two in one object: Ego_r definition_manager.obj
//   (Copy idx 444, operator= idx 386); FableWin definition_manager.obj (Copy idx 1315, operator= idx 1318). The row includes that
//   source (no code of its own), as the EH funclet rows include their parent; that object's Copy grades against this address. Before
//   the unit this row carried a second definition under the stand-in ?CAugmentationDef_Copy@@YIXPAVCAugmentationDef@@PBV1@@Z.
#include "../src/compiled/00/45/CAugmentationDef_operator_00454088.cpp"
