// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CTrophyDef_Copy
extern "C" void ForwardTarget_004e0474();
extern "C" void CTrophyDef_Copy() { ForwardTarget_004e0474(); }
// Stand-in (owner rule 2026-11-05): CTrophyDef_Copy for CTrophyDef::Copy; RTTI vtable slot ?Copy@CTrophyDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
