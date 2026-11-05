// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CJackOfBladesBattleDef_Copy
extern "C" void ForwardTarget_004e46cd();
extern "C" void CJackOfBladesBattleDef_Copy() { ForwardTarget_004e46cd(); }
// Stand-in (owner rule 2026-11-05): CJackOfBladesBattleDef_Copy for CJackOfBladesBattleDef::Copy; RTTI vtable slot ?Copy@CJackOfBladesBattleDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
