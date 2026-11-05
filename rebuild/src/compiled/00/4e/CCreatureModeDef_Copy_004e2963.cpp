// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CCreatureModeDef_Copy
extern "C" void ForwardTarget_004e2968();
extern "C" void CCreatureModeDef_Copy() { ForwardTarget_004e2968(); }
// Stand-in (owner rule 2026-11-05): CCreatureModeDef_Copy for CCreatureModeDef::Copy; RTTI vtable slot ?Copy@CCreatureModeDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
