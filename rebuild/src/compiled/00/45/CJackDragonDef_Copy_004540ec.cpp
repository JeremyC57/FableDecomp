// CJackDragonDef::Copy(const CDefClassBase* other) -- copy every member of another CJackDragonDef: `*this = *static_cast<const CJackDragonDef*>(other)` (tail call of the operator=)
// Leaf: ?Copy@CJackDragonDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004540EC, 5 bytes: a thiscall tail call, `jmp` to CJackDragonDef::operator= 0x004540F1 (the next body).
// Identity (NF425, 2026-11-25): self ?Copy@CJackDragonDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin ; target
//   ??4CJackDragonDef@@QAEAAV0@ABV0@@Z: bound at 0x004540F1 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Option S unit (NF425): this function's one body lives in its retail TU-mate's row 0x004540F1 (??4CJackDragonDef@@QAEAAV0@ABV0@@Z),
//   which already defines it (to emit the operator= it jumps to); both PDBs put the two in one object: Ego_r definition_manager.obj
//   (Copy idx 446, operator= idx 389); FableWin definition_manager.obj (Copy idx 1331, operator= idx 1334). The row includes that
//   source (no code of its own), as the EH funclet rows include their parent; that object's Copy grades against this address. Before
//   the unit this row carried a second definition under the stand-in ?CJackDragonDef_Copy@@YIXPAVCJackDragonDef@@PAXPBV1@@Z.
#include "../src/compiled/00/45/CJackDragonDef_operator_004540f1.cpp"
