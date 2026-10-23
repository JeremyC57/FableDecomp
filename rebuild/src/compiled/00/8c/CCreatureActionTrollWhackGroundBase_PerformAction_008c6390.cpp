// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_008c61c0();
extern "C" void CCreatureAction_TrollWhackGroundBase_PerformAction() { ForwardTarget_008c61c0(); }