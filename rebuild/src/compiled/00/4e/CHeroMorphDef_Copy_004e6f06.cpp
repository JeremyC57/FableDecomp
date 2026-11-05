// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CHeroMorphDef_Copy
extern "C" void ForwardTarget_004e6f0b();
extern "C" void CHeroMorphDef_Copy() { ForwardTarget_004e6f0b(); }
// Stand-in (owner rule 2026-11-05): CHeroMorphDef_Copy for CHeroMorphDef::Copy; RTTI vtable slot ?Copy@CHeroMorphDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
