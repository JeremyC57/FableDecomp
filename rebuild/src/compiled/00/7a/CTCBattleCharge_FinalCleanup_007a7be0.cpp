// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_007a75f0();
extern "C" void CTCBattleCharge_FinalCleanup() { ForwardTarget_007a75f0(); }