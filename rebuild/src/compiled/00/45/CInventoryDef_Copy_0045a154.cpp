// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CInventoryDef_Copy
extern "C" void ForwardTarget_0045a159();
extern "C" void CInventoryDef_Copy() { ForwardTarget_0045a159(); }
// Stand-in (owner rule 2026-11-05): CInventoryDef_Copy for CInventoryDef::Copy; RTTI vtable slot ?Copy@CInventoryDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
