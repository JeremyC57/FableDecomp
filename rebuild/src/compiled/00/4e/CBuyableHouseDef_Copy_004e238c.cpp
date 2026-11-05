// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CBuyableHouseDef_Copy
extern "C" void ForwardTarget_004e2391();
extern "C" void CBuyableHouseDef_Copy() { ForwardTarget_004e2391(); }
// Stand-in (owner rule 2026-11-05): CBuyableHouseDef_Copy for CBuyableHouseDef::Copy; RTTI vtable slot ?Copy@CBuyableHouseDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
