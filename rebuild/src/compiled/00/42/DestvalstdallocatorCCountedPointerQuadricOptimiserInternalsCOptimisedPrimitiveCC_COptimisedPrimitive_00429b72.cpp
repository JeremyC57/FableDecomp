// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _Forward
extern "C" void ForwardTarget_00429160();
extern "C" void Forward() { ForwardTarget_00429160(); }