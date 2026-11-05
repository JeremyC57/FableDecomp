// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CTimeAppearanceFadeDef_Copy
extern "C" void ForwardTarget_004e0750();
extern "C" void CTimeAppearanceFadeDef_Copy() { ForwardTarget_004e0750(); }
// Stand-in (owner rule 2026-11-05): CTimeAppearanceFadeDef_Copy for CTimeAppearanceFadeDef::Copy; RTTI vtable slot ?Copy@CTimeAppearanceFadeDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (fablewin).
