// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: COptimisedPrimitive>_>
extern "C" void ForwardTarget_004291de();
extern "C" void Forward() { ForwardTarget_004291de(); }