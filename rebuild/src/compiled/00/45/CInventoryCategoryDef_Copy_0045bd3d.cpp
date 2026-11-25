// CInventoryCategoryDef::Copy(const CDefClassBase* other) -- copy every member of another CInventoryCategoryDef: `*this = *static_cast<const CInventoryCategoryDef*>(other)` (tail call of the operator=)
// Leaf: ?Copy@CInventoryCategoryDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045BD3D, 5 bytes: a thiscall tail call, `jmp` to CInventoryCategoryDef::operator= 0x0045BD42 (the next body).
// Identity (NF425, 2026-11-25): self ?Copy@CInventoryCategoryDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin ; target
//   ??4CInventoryCategoryDef@@QAEAAV0@ABV0@@Z: bound at 0x0045BD42 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of
//   the ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Option S unit (NF425): this function's one body lives in its retail TU-mate's row 0x0045BD42
//   (??4CInventoryCategoryDef@@QAEAAV0@ABV0@@Z), which already defines it (to emit the operator= it jumps to); both PDBs put the two in
//   one object: Ego_r definition_manager.obj (Copy idx 2596, operator= idx 2479); FableWin definition_manager.obj (Copy idx 260,
//   operator= idx 263). The row includes that source (no code of its own), as the EH funclet rows include their parent; that object's
//   Copy grades against this address. Before the unit this row carried a second definition under the stand-in
//   _CInventoryCategoryDef_Copy.
#include "../src/compiled/00/45/CInventoryCategoryDef_operator_0045bd42.cpp"
