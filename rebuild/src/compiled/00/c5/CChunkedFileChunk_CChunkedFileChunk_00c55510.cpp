// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: ??1CChunkedFileChunk@
extern "C" void ForwardTarget_00c6da30();
extern "C" void Forward() { ForwardTarget_00c6da30(); }