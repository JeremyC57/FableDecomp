// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _GFGetPreMainMemoryUsed
extern "C" unsigned GFGetPreMainMemoryUsedImpl();
extern "C" unsigned GFGetPreMainMemoryUsed() { return GFGetPreMainMemoryUsedImpl(); }