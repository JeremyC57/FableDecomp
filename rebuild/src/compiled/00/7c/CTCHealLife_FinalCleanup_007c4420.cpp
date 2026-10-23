// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_007c4320();
extern "C" void CTCHealLife_FinalCleanup() { ForwardTarget_007c4320(); }