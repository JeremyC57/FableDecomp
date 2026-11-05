// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CRumbleDef_Copy
extern "C" void ForwardTarget_004e64f3();
extern "C" void CRumbleDef_Copy() { ForwardTarget_004e64f3(); }
// Stand-in (owner rule 2026-11-05): CRumbleDef_Copy for CRumbleDef::Copy; RTTI vtable slot ?Copy@CRumbleDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
