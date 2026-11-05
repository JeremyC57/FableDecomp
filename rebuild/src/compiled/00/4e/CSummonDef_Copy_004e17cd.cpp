// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CSummonDef_Copy
extern "C" void ForwardTarget_004e17d2();
extern "C" void CSummonDef_Copy() { ForwardTarget_004e17d2(); }
// Stand-in (owner rule 2026-11-05): CSummonDef_Copy for CSummonDef::Copy; RTTI vtable slot ?Copy@CSummonDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
