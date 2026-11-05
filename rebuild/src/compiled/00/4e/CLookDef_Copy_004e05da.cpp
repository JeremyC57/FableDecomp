// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CLookDef_Copy
extern "C" void ForwardTarget_004e05df();
extern "C" void CLookDef_Copy() { ForwardTarget_004e05df(); }
// Stand-in (owner rule 2026-11-05): CLookDef_Copy for CLookDef::Copy; RTTI vtable slot ?Copy@CLookDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
