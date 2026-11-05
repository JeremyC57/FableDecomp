// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CHighlightItemDef_Copy
extern "C" void ForwardTarget_004e0923();
extern "C" void CHighlightItemDef_Copy() { ForwardTarget_004e0923(); }
// Stand-in (owner rule 2026-11-05): CHighlightItemDef_Copy for CHighlightItemDef::Copy; RTTI vtable slot ?Copy@CHighlightItemDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
