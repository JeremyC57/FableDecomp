// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_009cd850();
extern "C" void Forward() { ForwardTarget_009cd850(); }