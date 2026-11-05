// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CClockDef_Copy
extern "C" void ForwardTarget_004e4450();
extern "C" void CClockDef_Copy() { ForwardTarget_004e4450(); }
// Stand-in (owner rule 2026-11-05): CClockDef_Copy for CClockDef::Copy; RTTI vtable slot ?Copy@CClockDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
