// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: Kill
extern "C" void ForwardTarget_004c7d50();
extern "C" void Forward() { ForwardTarget_004c7d50(); }