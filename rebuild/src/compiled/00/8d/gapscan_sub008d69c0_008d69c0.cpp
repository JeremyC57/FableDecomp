// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_00861d00();
extern "C" void Forward() { ForwardTarget_00861d00(); }