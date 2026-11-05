// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CExplodingObjectDef_Copy
extern "C" void ForwardTarget_004e05ad();
extern "C" void CExplodingObjectDef_Copy() { ForwardTarget_004e05ad(); }
// Stand-in (owner rule 2026-11-05): CExplodingObjectDef_Copy for CExplodingObjectDef::Copy; RTTI vtable slot ?Copy@CExplodingObjectDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
