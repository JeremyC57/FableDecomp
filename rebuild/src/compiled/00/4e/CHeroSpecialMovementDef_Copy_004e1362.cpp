// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_004e1367();
extern "C" void Forward() { ForwardTarget_004e1367(); }