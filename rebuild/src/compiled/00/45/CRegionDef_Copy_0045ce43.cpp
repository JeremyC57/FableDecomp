// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CRegionDef_Copy
extern "C" void ForwardTarget_0045ce48();
extern "C" void CRegionDef_Copy() { ForwardTarget_0045ce48(); }
// Stand-in (owner rule 2026-11-05): CRegionDef_Copy for CRegionDef::Copy; RTTI vtable slot ?Copy@CRegionDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
