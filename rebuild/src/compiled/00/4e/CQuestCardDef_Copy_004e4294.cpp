// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CQuestCardDef_Copy
extern "C" void ForwardTarget_004e4299();
extern "C" void CQuestCardDef_Copy() { ForwardTarget_004e4299(); }
// Stand-in (owner rule 2026-11-05): CQuestCardDef_Copy for CQuestCardDef::Copy; RTTI vtable slot ?Copy@CQuestCardDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
