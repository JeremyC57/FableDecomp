// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CGameScriptInterface_GetWorldFrame
extern "C" void ForwardTarget_0049d870();
extern "C" void CGameScriptInterface_GetWorldFrame() { ForwardTarget_0049d870(); }
// Stand-in (owner rule 2026-11-05): CGameScriptInterface_GetWorldFrame for CGameScriptInterface::GetWorldFrame; RTTI vtable slot ?GetWorldFrame@CGameScriptInterface@@UBEJXZ + PDB name set (egor/fablewin).
