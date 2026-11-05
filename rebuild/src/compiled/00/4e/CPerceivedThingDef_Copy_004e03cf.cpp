// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CPerceivedThingDef_Copy
extern "C" void ForwardTarget_004e03d4();
extern "C" void CPerceivedThingDef_Copy() { ForwardTarget_004e03d4(); }
// Stand-in (owner rule 2026-11-05): CPerceivedThingDef_Copy for CPerceivedThingDef::Copy; RTTI vtable slot ?Copy@CPerceivedThingDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
