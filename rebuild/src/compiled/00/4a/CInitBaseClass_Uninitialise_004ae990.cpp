// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: Uninitialise
extern "C" void ForwardTarget_0099a360();
extern "C" void Forward() { ForwardTarget_0099a360(); }