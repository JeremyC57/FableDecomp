// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_0079d2d0();
extern "C" void CTCBulletTime_FinalCleanup() { ForwardTarget_0079d2d0(); }