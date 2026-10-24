// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: CEntry>_const&>
extern "C" void ForwardTarget_0099eae0();
extern "C" void Forward() { ForwardTarget_0099eae0(); }