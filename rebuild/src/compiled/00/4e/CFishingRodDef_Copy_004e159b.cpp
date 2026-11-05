// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CFishingRodDef_Copy
extern "C" void ForwardTarget_004e15a0();
extern "C" void CFishingRodDef_Copy() { ForwardTarget_004e15a0(); }
// Stand-in (owner rule 2026-11-05): CFishingRodDef_Copy for CFishingRodDef::Copy; RTTI vtable slot ?Copy@CFishingRodDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (fablewin).
