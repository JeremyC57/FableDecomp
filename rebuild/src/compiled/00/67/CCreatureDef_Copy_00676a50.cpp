// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CCreatureDef_Copy
extern "C" void ForwardTarget_00675c00();
extern "C" void CCreatureDef_Copy() { ForwardTarget_00675c00(); }
// Stand-in (owner rule 2026-11-05): CCreatureDef_Copy for CCreatureDef::Copy; RTTI vtable slot ?Copy@CCreatureDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
