// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CFishDef_Copy
extern "C" void ForwardTarget_004e12b8();
extern "C" void CFishDef_Copy() { ForwardTarget_004e12b8(); }
// Stand-in (owner rule 2026-11-05): CFishDef_Copy for CFishDef::Copy; RTTI vtable slot ?Copy@CFishDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
