// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CThingShotDef_Copy
extern "C" void ForwardTarget_0046267d();
extern "C" void CThingShotDef_Copy() { ForwardTarget_0046267d(); }
// Stand-in (owner rule 2026-11-05): CThingShotDef_Copy for CThingShotDef::Copy; RTTI vtable slot ?Copy@CThingShotDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
