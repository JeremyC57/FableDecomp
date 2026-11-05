// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CTrapDef_Copy
extern "C" void ForwardTarget_004e5c89();
extern "C" void CTrapDef_Copy() { ForwardTarget_004e5c89(); }
// Stand-in (owner rule 2026-11-05): CTrapDef_Copy for CTrapDef::Copy; RTTI vtable slot ?Copy@CTrapDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
