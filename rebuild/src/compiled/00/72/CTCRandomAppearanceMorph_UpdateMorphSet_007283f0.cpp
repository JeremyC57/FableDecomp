// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_007280c0();
extern "C" void Forward() { ForwardTarget_007280c0(); }