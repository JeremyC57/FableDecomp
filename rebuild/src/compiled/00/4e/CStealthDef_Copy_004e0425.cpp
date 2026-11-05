// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CStealthDef_Copy
extern "C" void ForwardTarget_004e042a();
extern "C" void CStealthDef_Copy() { ForwardTarget_004e042a(); }
// Stand-in (owner rule 2026-11-05): CStealthDef_Copy for CStealthDef::Copy; RTTI vtable slot ?Copy@CStealthDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
