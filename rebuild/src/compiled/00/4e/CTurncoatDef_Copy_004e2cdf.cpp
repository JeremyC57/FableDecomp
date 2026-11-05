// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CTurncoatDef_Copy
extern "C" void ForwardTarget_004e2ce4();
extern "C" void CTurncoatDef_Copy() { ForwardTarget_004e2ce4(); }
// Stand-in (owner rule 2026-11-05): CTurncoatDef_Copy for CTurncoatDef::Copy; RTTI vtable slot ?Copy@CTurncoatDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
