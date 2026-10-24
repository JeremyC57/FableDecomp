// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: IsBetting
extern "C" void ForwardTarget_00c46dc0();
extern "C" void Forward() { ForwardTarget_00c46dc0(); }