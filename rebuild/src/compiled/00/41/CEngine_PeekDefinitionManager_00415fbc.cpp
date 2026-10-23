// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" unsigned GFGetPreMainMemoryUsedImpl_0044c6b0();
extern "C" unsigned GFGetPreMainMemoryUsed() { return GFGetPreMainMemoryUsedImpl_0044c6b0(); }