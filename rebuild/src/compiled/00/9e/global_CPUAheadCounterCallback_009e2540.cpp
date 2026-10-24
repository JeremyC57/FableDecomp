// CEngineDebug::CPUAheadCounterCallback() -- read the CPU time-stamp counter (zeroes edx:eax first)
// Leaf: CPUAheadCounterCallback
// Retail-asm: 0x009E2543 `xor eax,eax` / 0x009E2545 `xor edx,edx` / 0x009E2547 `rdtsc` with edx:eax stored to an 8-byte local and reloaded; VC7.1 (13.10.3077/4035) has no rdtsc intrinsic (__rdtsc/<intrin.h> arrived with VC8), so the original read the counter with inline asm.
typedef unsigned __int64 uint64;
extern "C" uint64 __cdecl CPUAheadCounterCallback()
{
    uint64 counter;
    __asm {
        xor eax, eax
        xor edx, edx
        rdtsc
        mov dword ptr counter, eax
        mov dword ptr counter+4, edx
    }
    return counter;
}
