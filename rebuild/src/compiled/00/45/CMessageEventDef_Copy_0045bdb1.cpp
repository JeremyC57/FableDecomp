// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CMessageEventDef_Copy
extern "C" void ForwardTarget_0045bdb6();
extern "C" void CMessageEventDef_Copy() { ForwardTarget_0045bdb6(); }
// Stand-in (owner rule 2026-11-05): CMessageEventDef_Copy for CMessageEventDef::Copy; RTTI vtable slot ?Copy@CMessageEventDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
