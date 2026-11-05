// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CShopDef_Copy
extern "C" void ForwardTarget_004e9813();
extern "C" void CShopDef_Copy() { ForwardTarget_004e9813(); }
// Stand-in (owner rule 2026-11-05): CShopDef_Copy for CShopDef::Copy; RTTI vtable slot ?Copy@CShopDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
