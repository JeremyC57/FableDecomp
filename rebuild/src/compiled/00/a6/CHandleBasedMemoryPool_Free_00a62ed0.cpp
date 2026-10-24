// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: Free
extern "C" void ForwardTarget_00a02b30();
extern "C" void Forward() { ForwardTarget_00a02b30(); }