// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: Stub_ReturnVoid_004c72a0
extern "C" void ForwardTarget_00838940();
extern "C" void Forward() { ForwardTarget_00838940(); }