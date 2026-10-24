// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: Release
extern "C" void ForwardTarget_00c0ba40();
extern "C" void Forward() { ForwardTarget_00c0ba40(); }