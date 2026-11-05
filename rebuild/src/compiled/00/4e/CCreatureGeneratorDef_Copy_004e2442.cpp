// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CCreatureGeneratorDef_Copy
extern "C" void ForwardTarget_004e2447();
extern "C" void CCreatureGeneratorDef_Copy() { ForwardTarget_004e2447(); }
// Stand-in (owner rule 2026-11-05): CCreatureGeneratorDef_Copy for CCreatureGeneratorDef::Copy; RTTI vtable slot ?Copy@CCreatureGeneratorDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
