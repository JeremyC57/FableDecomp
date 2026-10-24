// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: UnsheatheMeleeWeapon
extern "C" void ForwardTarget_006649c0();
extern "C" void Forward() { ForwardTarget_006649c0(); }