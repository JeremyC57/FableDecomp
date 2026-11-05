// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CCreatureNavigationDef_Copy
extern "C" void ForwardTarget_004e001a();
extern "C" void CCreatureNavigationDef_Copy() { ForwardTarget_004e001a(); }
// Stand-in (owner rule 2026-11-05): CCreatureNavigationDef_Copy for CCreatureNavigationDef::Copy; RTTI vtable slot ?Copy@CCreatureNavigationDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
