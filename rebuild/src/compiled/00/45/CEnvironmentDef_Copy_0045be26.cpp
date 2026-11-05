// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CEnvironmentDef_Copy
extern "C" void ForwardTarget_0045be2b();
extern "C" void CEnvironmentDef_Copy() { ForwardTarget_0045be2b(); }
// Stand-in (owner rule 2026-11-05): CEnvironmentDef_Copy for CEnvironmentDef::Copy; RTTI vtable slot ?Copy@CEnvironmentDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
