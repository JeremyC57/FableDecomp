// CEngineDebug::CPUAheadCounterCallback() -- read the CPU time-stamp counter (zeroes edx:eax first)
// Leaf: CPUAheadCounterCallback
// Retail-asm: 0x009E2543 `xor eax,eax` / 0x009E2545 `xor edx,edx` / 0x009E2547 `rdtsc` with edx:eax stored to an 8-byte local and reloaded; VC7.1 (13.10.3077/4035) has no rdtsc intrinsic (__rdtsc/<intrin.h> arrived with VC8), so the original read the counter with inline asm.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFGetCycleCount.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
#include "engine/GFGetCycleCount.h"
typedef unsigned __int64 uint64;
extern "C" uint64 __cdecl CPUAheadCounterCallback()
{
    return GFGetCycleCount();
}
