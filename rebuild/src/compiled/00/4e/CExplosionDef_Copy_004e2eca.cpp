// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CExplosionDef_Copy
extern "C" void ForwardTarget_004e2ecf();
extern "C" void CExplosionDef_Copy() { ForwardTarget_004e2ecf(); }
// Stand-in (owner rule 2026-11-05): CExplosionDef_Copy for CExplosionDef::Copy; RTTI vtable slot ?Copy@CExplosionDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
