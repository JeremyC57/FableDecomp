// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CHeroSpecialMovementDef_Copy
extern "C" void ForwardTarget_004e1367();
extern "C" void CHeroSpecialMovementDef_Copy() { ForwardTarget_004e1367(); }
// Stand-in (owner rule 2026-11-05): CHeroSpecialMovementDef_Copy for CHeroSpecialMovementDef::Copy; RTTI vtable slot ?Copy@CHeroSpecialMovementDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
