// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CVillageDef_Copy
extern "C" void ForwardTarget_004e1e06();
extern "C" void CVillageDef_Copy() { ForwardTarget_004e1e06(); }
// Stand-in (owner rule 2026-11-05): CVillageDef_Copy for CVillageDef::Copy; RTTI vtable slot ?Copy@CVillageDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
