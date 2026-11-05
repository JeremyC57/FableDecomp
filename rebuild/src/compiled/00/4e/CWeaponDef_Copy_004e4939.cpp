// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CWeaponDef_Copy
extern "C" void ForwardTarget_004e493e();
extern "C" void CWeaponDef_Copy() { ForwardTarget_004e493e(); }
// Stand-in (owner rule 2026-11-05): CWeaponDef_Copy for CWeaponDef::Copy; RTTI vtable slot ?Copy@CWeaponDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
