// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: OnReadFinished
extern "C" void ForwardTarget_004df554();
extern "C" void Forward() { ForwardTarget_004df554(); }