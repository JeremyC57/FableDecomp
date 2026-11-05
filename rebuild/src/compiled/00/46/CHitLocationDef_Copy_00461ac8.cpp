// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CHitLocationDef_Copy
extern "C" void ForwardTarget_00461acd();
extern "C" void CHitLocationDef_Copy() { ForwardTarget_00461acd(); }
// Stand-in (owner rule 2026-11-05): CHitLocationDef_Copy for CHitLocationDef::Copy; RTTI vtable slot ?Copy@CHitLocationDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
