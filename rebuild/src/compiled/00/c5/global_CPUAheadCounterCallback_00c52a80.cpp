// CEngineDebug::CPUAheadCounterCallback() -- read the CPU time-stamp counter
// Leaf: CPUAheadCounterCallback
// Retail-asm: 0x00C52A83 `rdtsc` with edx:eax stored to an 8-byte local (0x00C52A85/0x00C52A88) and reloaded; VC7.1 (13.10.3077/4035) has no rdtsc intrinsic (__rdtsc/<intrin.h> arrived with VC8), so the original read the counter with inline asm.
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared libpk/ReadTimeStampCounter.h (byte-matching path + FABLE_PORTABLE twin);
//   the row has no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family RDTSC.
#include "libpk/ReadTimeStampCounter.h"
typedef unsigned __int64 uint64;
extern "C" uint64 __cdecl CPUAheadCounterCallback()
{
    uint64 counter;
    LIBPK_READ_TIME_STAMP_COUNTER(counter);
    return counter;
}
