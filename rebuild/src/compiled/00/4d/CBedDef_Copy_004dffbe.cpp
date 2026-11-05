// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CBedDef_Copy
extern "C" void ForwardTarget_004dffc3();
extern "C" void CBedDef_Copy() { ForwardTarget_004dffc3(); }
// Stand-in (owner rule 2026-11-05): CBedDef_Copy for CBedDef::Copy; RTTI vtable slot ?Copy@CBedDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
