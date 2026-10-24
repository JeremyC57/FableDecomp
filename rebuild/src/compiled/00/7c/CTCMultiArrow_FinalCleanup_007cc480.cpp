// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: FinalCleanup
extern "C" void ForwardTarget_007cc410();
extern "C" void CTCMultiArrow_FinalCleanup() { ForwardTarget_007cc410(); }