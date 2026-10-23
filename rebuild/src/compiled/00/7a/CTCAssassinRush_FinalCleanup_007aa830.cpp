// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_007aa4d0();
extern "C" void CTCAssassinRush_FinalCleanup() { ForwardTarget_007aa4d0(); }