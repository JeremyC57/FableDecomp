// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CHeroStatsDef_Copy
extern "C" void ForwardTarget_00478132();
extern "C" void CHeroStatsDef_Copy() { ForwardTarget_00478132(); }
// Stand-in (owner rule 2026-11-05): CHeroStatsDef_Copy for CHeroStatsDef::Copy; RTTI vtable slot ?Copy@CHeroStatsDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
