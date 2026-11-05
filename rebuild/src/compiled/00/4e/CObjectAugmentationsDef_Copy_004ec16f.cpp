// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CObjectAugmentationsDef_Copy
extern "C" void ForwardTarget_004ec174();
extern "C" void CObjectAugmentationsDef_Copy() { ForwardTarget_004ec174(); }
// Stand-in (owner rule 2026-11-05): CObjectAugmentationsDef_Copy for CObjectAugmentationsDef::Copy; RTTI vtable slot ?Copy@CObjectAugmentationsDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
