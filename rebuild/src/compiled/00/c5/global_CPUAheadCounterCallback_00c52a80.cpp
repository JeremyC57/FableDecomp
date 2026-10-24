// CEngineDebug::CPUAheadCounterCallback() -- read the CPU time-stamp counter
// Leaf: CPUAheadCounterCallback
// Retail-asm: 0x00C52A83 `rdtsc` with edx:eax stored to an 8-byte local (0x00C52A85/0x00C52A88) and reloaded; VC7.1 (13.10.3077/4035) has no rdtsc intrinsic (__rdtsc/<intrin.h> arrived with VC8), so the original read the counter with inline asm.
typedef unsigned __int64 uint64;
extern "C" uint64 __cdecl CPUAheadCounterCallback()
{
    uint64 counter;
    __asm {
        rdtsc
        mov dword ptr counter, eax
        mov dword ptr counter+4, edx
    }
    return counter;
}
