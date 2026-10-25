// MeasureCpuFrequency() -- measure the processor clock in Hz: sample the performance counter and the time-stamp counter around a 10 ms sleep (retrying when either counter runs backwards) and scale the TSC delta by the performance-counter frequency; 0 without a performance counter
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 3-instruction __asm (no control flow) + the Retail-asm evidence line
// Leaf: ?MeasureCpuFrequency@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C52CE0, length 186 bytes (+ int3 padding); plain `ret`, the result in
// edx:eax. Caller: the CPU identification at 0x00C5319E, which divides it by 1000000 and hands the MHz value to this
// chunk's RoundCpuSpeed 0x00C52DA0.
// Imports: QueryPerformanceFrequency 0x0143FE04, QueryPerformanceCounter 0x0143FE00 (held in ebp), Sleep 0x0143FE1C;
// CRT helpers __allmul 0x00BFEB50 and __aulldiv 0x00BFEE00 (an unsigned TSC delta times a signed LONGLONG frequency,
// divided by the signed counter delta: the product is unsigned, so the unsigned divide).
// Retail-asm: 0x00C52D17 / 0x00C52D30 `rdtsc` with edx:eax stored to the 8-byte local (0x00C52D19/0x00C52D1D and 0x00C52D32/0x00C52D36), the 3-instruction counter read of the landed CPUAheadCounterCallback (0x00C52A80) expanded twice; VC7.1 (13.10.3077/4035) has no rdtsc intrinsic (__rdtsc/<intrin.h> arrived with VC8), so the original read the counter with inline asm.
// The catalog label "CPU_CalibrateTSCFrequency" is descriptive.

typedef union _LARGE_INTEGER { struct { unsigned long LowPart; long HighPart; } u; __int64 QuadPart; } LARGE_INTEGER;
extern "C" __declspec(dllimport) int __stdcall QueryPerformanceFrequency(LARGE_INTEGER* frequency);
extern "C" __declspec(dllimport) int __stdcall QueryPerformanceCounter(LARGE_INTEGER* count);
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);

// the time-stamp counter read (expanded in place: rdtsc, spill edx:eax to the 8-byte local)
__forceinline unsigned __int64 ReadTimeStampCounter()
{
    unsigned __int64 counter;
    __asm {
        rdtsc
        mov dword ptr counter, eax
        mov dword ptr counter+4, edx
    }
    return counter;
}

unsigned __int64 MeasureCpuFrequency()
{
    LARGE_INTEGER frequency;
    if (!QueryPerformanceFrequency(&frequency))
        return 0;
    LARGE_INTEGER countStart, countEnd;
    unsigned __int64 tscStart, tscEnd;
    do {
        QueryPerformanceCounter(&countStart);
        tscStart = ReadTimeStampCounter();
        Sleep(10);
        QueryPerformanceCounter(&countEnd);
        tscEnd = ReadTimeStampCounter();
    } while (tscStart > tscEnd || countStart.QuadPart > countEnd.QuadPart);
    return (tscEnd - tscStart) * frequency.QuadPart / (countEnd.QuadPart - countStart.QuadPart);
}