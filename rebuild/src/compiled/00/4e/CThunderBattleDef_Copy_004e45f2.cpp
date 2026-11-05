// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CThunderBattleDef_Copy
extern "C" void ForwardTarget_004e45f7();
extern "C" void CThunderBattleDef_Copy() { ForwardTarget_004e45f7(); }
// Stand-in (owner rule 2026-11-05): CThunderBattleDef_Copy for CThunderBattleDef::Copy; RTTI vtable slot ?Copy@CThunderBattleDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
