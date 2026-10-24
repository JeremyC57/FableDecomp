// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: InvalidateAndClearAllSecuritySweepTasks
extern "C" void ForwardTarget_006590a0();
extern "C" void Forward() { ForwardTarget_006590a0(); }