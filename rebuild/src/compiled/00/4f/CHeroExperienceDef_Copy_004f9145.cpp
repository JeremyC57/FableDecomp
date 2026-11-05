// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CHeroExperienceDef_Copy
extern "C" void ForwardTarget_004f914a();
extern "C" void CHeroExperienceDef_Copy() { ForwardTarget_004f914a(); }
// Stand-in (owner rule 2026-11-05): CHeroExperienceDef_Copy for CHeroExperienceDef::Copy; RTTI vtable slot ?Copy@CHeroExperienceDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
