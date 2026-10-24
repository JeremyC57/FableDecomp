// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: CEntry>_const&>
extern "C" void ForwardTarget_00c271e0();
extern "C" void Forward() { ForwardTarget_00c271e0(); }