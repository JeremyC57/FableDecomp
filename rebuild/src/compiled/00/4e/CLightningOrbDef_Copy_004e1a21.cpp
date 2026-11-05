// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CLightningOrbDef_Copy
extern "C" void ForwardTarget_004e1a26();
extern "C" void CLightningOrbDef_Copy() { ForwardTarget_004e1a26(); }
// Stand-in (owner rule 2026-11-05): CLightningOrbDef_Copy for CLightningOrbDef::Copy; RTTI vtable slot ?Copy@CLightningOrbDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
