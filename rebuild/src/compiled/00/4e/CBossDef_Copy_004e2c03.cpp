// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CBossDef_Copy
extern "C" void ForwardTarget_004e2c08();
extern "C" void CBossDef_Copy() { ForwardTarget_004e2c08(); }
// Stand-in (owner rule 2026-11-05): CBossDef_Copy for CBossDef::Copy; RTTI vtable slot ?Copy@CBossDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
