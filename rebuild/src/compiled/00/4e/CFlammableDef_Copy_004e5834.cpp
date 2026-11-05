// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CFlammableDef_Copy
extern "C" void ForwardTarget_004e5839();
extern "C" void CFlammableDef_Copy() { ForwardTarget_004e5839(); }
// Stand-in (owner rule 2026-11-05): CFlammableDef_Copy for CFlammableDef::Copy; RTTI vtable slot ?Copy@CFlammableDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
