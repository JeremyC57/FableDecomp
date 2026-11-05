// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CWillResponseDef_Copy
extern "C" void ForwardTarget_004e18ce();
extern "C" void CWillResponseDef_Copy() { ForwardTarget_004e18ce(); }
// Stand-in (owner rule 2026-11-05): CWillResponseDef_Copy for CWillResponseDef::Copy; RTTI vtable slot ?Copy@CWillResponseDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
