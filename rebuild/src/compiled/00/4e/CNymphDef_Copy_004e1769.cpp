// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CNymphDef_Copy
extern "C" void ForwardTarget_004e176e();
extern "C" void CNymphDef_Copy() { ForwardTarget_004e176e(); }
// Stand-in (owner rule 2026-11-05): CNymphDef_Copy for CNymphDef::Copy; RTTI vtable slot ?Copy@CNymphDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
