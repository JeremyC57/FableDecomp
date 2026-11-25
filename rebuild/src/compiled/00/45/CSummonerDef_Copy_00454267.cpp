// CSummonerDef::Copy(const CDefClassBase* other) -- copy every member of another CSummonerDef: `*this = *static_cast<const CSummonerDef*>(other)` (tail call of the operator=)
// Leaf: ?Copy@CSummonerDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00454267, 5 bytes: a thiscall tail call, `jmp` to CSummonerDef::operator= 0x0045426C (the next body).
// Identity (NF425, 2026-11-25): self ?Copy@CSummonerDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin ; target
//   ??4CSummonerDef@@QAEAAV0@ABV0@@Z: bound at 0x0045426C (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Option S unit (NF425): this function's one body lives in its retail TU-mate's row 0x0045426C (??4CSummonerDef@@QAEAAV0@ABV0@@Z),
//   which already defines it (to emit the operator= it jumps to); both PDBs put the two in one object: Ego_r definition_manager.obj
//   (Copy idx 450, operator= idx 396); FableWin definition_manager.obj (Copy idx 1363, operator= idx 1366). The row includes that
//   source (no code of its own), as the EH funclet rows include their parent; that object's Copy grades against this address. Before
//   the unit this row carried a second definition under the stand-in ?CSummonerDef_Copy@@YIXPAVCSummonerDef@@PBV1@@Z.
#include "../src/compiled/00/45/CSummonerDef_operator_0045426c.cpp"
