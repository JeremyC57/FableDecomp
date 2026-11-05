// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CGuardDef_Copy
extern "C" void ForwardTarget_004e0e28();
extern "C" void CGuardDef_Copy() { ForwardTarget_004e0e28(); }
// Stand-in (owner rule 2026-11-05): CGuardDef_Copy for CGuardDef::Copy; RTTI vtable slot ?Copy@CGuardDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
