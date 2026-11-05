// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CTrollBattleDef_Copy
extern "C" void ForwardTarget_004e47e8();
extern "C" void CTrollBattleDef_Copy() { ForwardTarget_004e47e8(); }
// Stand-in (owner rule 2026-11-05): CTrollBattleDef_Copy for CTrollBattleDef::Copy; RTTI vtable slot ?Copy@CTrollBattleDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
