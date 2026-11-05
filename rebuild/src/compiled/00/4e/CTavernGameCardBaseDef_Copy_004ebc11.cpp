// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CTavernGameCardBaseDef_Copy
extern "C" void ForwardTarget_004ebc16();
extern "C" void CTavernGameCardBaseDef_Copy() { ForwardTarget_004ebc16(); }
// Stand-in (owner rule 2026-11-05): CTavernGameCardBaseDef_Copy for CTavernGameCardBaseDef::Copy; RTTI vtable slot ?Copy@CTavernGameCardBaseDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
