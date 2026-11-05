// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CLightDef_Copy
extern "C" void ForwardTarget_004e027e();
extern "C" void CLightDef_Copy() { ForwardTarget_004e027e(); }
// Stand-in (owner rule 2026-11-05): CLightDef_Copy for CLightDef::Copy; RTTI vtable slot ?Copy@CLightDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
