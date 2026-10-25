// MeasureCpuFrequency() -- measure the processor clock in Hz: sample the performance counter and the time-stamp counter around a 10 ms sleep (retrying when either counter runs backwards) and scale the TSC delta by the performance-counter frequency; 0 without a performance counter
// Leaf: ?MeasureCpuFrequency@@
// Grade: DIFFER(172v186) -- retail_asm candidate: retail reads the TSC with inline-asm `rdtsc` (two stores per read) inside an `and esp,-8` ebp frame; the C++ reference calls a stand-in instead
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C52CE0, length 186 bytes (+ int3 padding); plain `ret`, the result in
// edx:eax. Caller: the CPU identification at 0x00C5319E, which divides it by 1000000 and hands the MHz value to this
// chunk's RoundCpuSpeed 0x00C52DA0.
// Imports: QueryPerformanceFrequency 0x0143FE04, QueryPerformanceCounter 0x0143FE00 (held in ebp), Sleep 0x0143FE1C;
// CRT helpers __allmul 0x00BFEB50 and __aulldiv 0x00BFEE00 (an unsigned TSC delta times a signed LONGLONG frequency,
// divided by the signed counter delta: the product is unsigned, so the unsigned divide).
// Retail-asm evidence: each TSC read is `rdtsc ; mov [lo],eax ; mov [hi],edx` (a two-store inline-asm block; VC7.1 has
// no __rdtsc intrinsic), and the frame is `push ebp ; mov ebp,esp ; and esp,-8` (8-aligned __int64 locals with inline
// asm). The lane rules forbid inline asm in sources; this is the C++ reference body, with an out-of-line stand-in for rdtsc.
// The catalog label "CPU_CalibrateTSCFrequency" is descriptive.

typedef union _LARGE_INTEGER { struct { unsigned long LowPart; long HighPart; } u; __int64 QuadPart; } LARGE_INTEGER;
extern "C" __declspec(dllimport) int __stdcall QueryPerformanceFrequency(LARGE_INTEGER* frequency);
extern "C" __declspec(dllimport) int __stdcall QueryPerformanceCounter(LARGE_INTEGER* count);
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);

unsigned __int64 ReadTimeStampCounter();            // stands for `rdtsc` (edx:eax)

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
