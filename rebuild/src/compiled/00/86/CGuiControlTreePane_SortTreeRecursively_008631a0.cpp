// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _Forward
extern "C" void ForwardTarget_006f09d0();
extern "C" void Forward() { ForwardTarget_006f09d0(); }