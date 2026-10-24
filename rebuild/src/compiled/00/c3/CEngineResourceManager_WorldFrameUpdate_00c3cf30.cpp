// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: WorldFrameUpdate
extern "C" void ForwardTarget_00c44550();
extern "C" void Forward() { ForwardTarget_00c44550(); }