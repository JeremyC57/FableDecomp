// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CBettingDef_Copy
extern "C" void ForwardTarget_004e1918();
extern "C" void CBettingDef_Copy() { ForwardTarget_004e1918(); }
// Stand-in (owner rule 2026-11-05): CBettingDef_Copy for CBettingDef::Copy; RTTI vtable slot ?Copy@CBettingDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
