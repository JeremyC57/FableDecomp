// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CMaterialDef_Copy
extern "C" void ForwardTarget_0045c112();
extern "C" void CMaterialDef_Copy() { ForwardTarget_0045c112(); }
// Stand-in (owner rule 2026-11-05): CMaterialDef_Copy for CMaterialDef::Copy; RTTI vtable slot ?Copy@CMaterialDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
