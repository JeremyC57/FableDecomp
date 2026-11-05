// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CCarryableDef_Copy
extern "C" void ForwardTarget_004dfeb8();
extern "C" void CCarryableDef_Copy() { ForwardTarget_004dfeb8(); }
// Stand-in (owner rule 2026-11-05): CCarryableDef_Copy for CCarryableDef::Copy; RTTI vtable slot ?Copy@CCarryableDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
