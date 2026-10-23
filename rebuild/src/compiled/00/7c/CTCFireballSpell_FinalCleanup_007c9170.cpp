// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_007c90a0();
extern "C" void CTCFireballSpell_FinalCleanup() { ForwardTarget_007c90a0(); }