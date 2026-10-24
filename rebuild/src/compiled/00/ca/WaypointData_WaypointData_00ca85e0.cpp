// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: ??1WaypointData@
extern "C" void ForwardTarget_00ca7c80();
extern "C" void Forward() { ForwardTarget_00ca7c80(); }