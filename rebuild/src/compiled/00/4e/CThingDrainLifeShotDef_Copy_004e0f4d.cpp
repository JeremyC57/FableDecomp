// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CThingDrainLifeShotDef_Copy
extern "C" void ForwardTarget_004e0f52();
extern "C" void CThingDrainLifeShotDef_Copy() { ForwardTarget_004e0f52(); }
// Stand-in (owner rule 2026-11-05): CThingDrainLifeShotDef_Copy for CThingDrainLifeShotDef::Copy; RTTI vtable slot ?Copy@CThingDrainLifeShotDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
