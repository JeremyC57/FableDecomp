// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CTargetingDef_Copy
extern "C" void ForwardTarget_0045d729();
extern "C" void CTargetingDef_Copy() { ForwardTarget_0045d729(); }
// Stand-in (owner rule 2026-11-05): CTargetingDef_Copy for CTargetingDef::Copy; RTTI vtable slot ?Copy@CTargetingDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
