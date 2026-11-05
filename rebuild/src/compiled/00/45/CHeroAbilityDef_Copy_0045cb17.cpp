// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CHeroAbilityDef_Copy
extern "C" void ForwardTarget_0045cb1c();
extern "C" void CHeroAbilityDef_Copy() { ForwardTarget_0045cb1c(); }
// Stand-in (owner rule 2026-11-05): CHeroAbilityDef_Copy for CHeroAbilityDef::Copy; RTTI vtable slot ?Copy@CHeroAbilityDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
