// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_007cd930();
extern "C" void CTCBurntEffect_FinalCleanup() { ForwardTarget_007cd930(); }