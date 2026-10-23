// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_007ccd70();
extern "C" void CTCMultiStrike_FinalCleanup() { ForwardTarget_007ccd70(); }