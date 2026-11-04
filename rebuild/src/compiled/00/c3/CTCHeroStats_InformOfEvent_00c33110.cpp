// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _Forward
// Vendor naming wave V1 (one address, one name): 0x00C3C370 _CodaFree (was _ForwardTarget_00c3c370)
extern "C" void CodaFree();
extern "C" void Forward() { CodaFree(); }