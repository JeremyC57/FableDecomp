// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: InformOfEvent
extern "C" void ForwardTarget_00c3c360();
extern "C" void Forward() { ForwardTarget_00c3c360(); }