// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_009ff440();
extern "C" void CGraphicDataBank_NotifyBankOpened() { ForwardTarget_009ff440(); }