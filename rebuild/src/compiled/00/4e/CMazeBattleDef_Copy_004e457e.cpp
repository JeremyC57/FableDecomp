// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CMazeBattleDef_Copy
extern "C" void ForwardTarget_004e4583();
extern "C" void CMazeBattleDef_Copy() { ForwardTarget_004e4583(); }
// Stand-in (owner rule 2026-11-05): CMazeBattleDef_Copy for CMazeBattleDef::Copy; RTTI vtable slot ?Copy@CMazeBattleDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
