// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CThingBuildingDef_Copy
extern "C" void ForwardTarget_004643cf();
extern "C" void CThingBuildingDef_Copy() { ForwardTarget_004643cf(); }
// Stand-in (owner rule 2026-11-05): CThingBuildingDef_Copy for CThingBuildingDef::Copy; RTTI vtable slot ?Copy@CThingBuildingDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
