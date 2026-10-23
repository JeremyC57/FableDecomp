// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_008b7640();
extern "C" void CCreatureAction_RiseUp_PerformAction() { ForwardTarget_008b7640(); }