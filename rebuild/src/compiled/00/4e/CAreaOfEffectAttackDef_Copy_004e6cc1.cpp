// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CAreaOfEffectAttackDef_Copy
extern "C" void ForwardTarget_004e6cc6();
extern "C" void CAreaOfEffectAttackDef_Copy() { ForwardTarget_004e6cc6(); }
// Stand-in (owner rule 2026-11-05): CAreaOfEffectAttackDef_Copy for CAreaOfEffectAttackDef::Copy; RTTI vtable slot ?Copy@CAreaOfEffectAttackDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
