// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CKickableDef_Copy
extern "C" void ForwardTarget_004e0222();
extern "C" void CKickableDef_Copy() { ForwardTarget_004e0222(); }
// Stand-in (owner rule 2026-11-05): CKickableDef_Copy for CKickableDef::Copy; RTTI vtable slot ?Copy@CKickableDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
