// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CExperienceDef_Copy
extern "C" void ForwardTarget_004e4d88();
extern "C" void CExperienceDef_Copy() { ForwardTarget_004e4d88(); }
// Stand-in (owner rule 2026-11-05): CExperienceDef_Copy for CExperienceDef::Copy; RTTI vtable slot ?Copy@CExperienceDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
