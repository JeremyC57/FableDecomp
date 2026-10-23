// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_007cf040();
extern "C" void CTCCreatureTintEffect_FinalCleanup() { ForwardTarget_007cf040(); }