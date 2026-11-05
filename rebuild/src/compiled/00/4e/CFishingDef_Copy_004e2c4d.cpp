// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CFishingDef_Copy
extern "C" void ForwardTarget_004e2c52();
extern "C" void CFishingDef_Copy() { ForwardTarget_004e2c52(); }
// Stand-in (owner rule 2026-11-05): CFishingDef_Copy for CFishingDef::Copy; RTTI vtable slot ?Copy@CFishingDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
