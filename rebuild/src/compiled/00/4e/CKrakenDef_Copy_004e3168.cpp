// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CKrakenDef_Copy
extern "C" void ForwardTarget_004e316d();
extern "C" void CKrakenDef_Copy() { ForwardTarget_004e316d(); }
// Stand-in (owner rule 2026-11-05): CKrakenDef_Copy for CKrakenDef::Copy; RTTI vtable slot ?Copy@CKrakenDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
