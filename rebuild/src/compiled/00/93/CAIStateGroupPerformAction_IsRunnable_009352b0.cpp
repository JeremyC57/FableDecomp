// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CAIStateGroup_PerformAction_IsRunnable
extern "C" void ForwardTarget_00935160();
extern "C" void CAIStateGroup_PerformAction_IsRunnable() { ForwardTarget_00935160(); }
// Stand-in (owner rule 2026-11-05): CAIStateGroup_PerformAction_IsRunnable for CAIStateGroup_PerformAction::IsRunnable; RTTI vtable slot ?IsRunnable@CAIStateGroup_PerformAction@@UAE_NXZ + PDB name set (egor/fablewin).
