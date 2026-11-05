// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CChestDef_Copy
extern "C" void ForwardTarget_004e0569();
extern "C" void CChestDef_Copy() { ForwardTarget_004e0569(); }
// Stand-in (owner rule 2026-11-05): CChestDef_Copy for CChestDef::Copy; RTTI vtable slot ?Copy@CChestDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
