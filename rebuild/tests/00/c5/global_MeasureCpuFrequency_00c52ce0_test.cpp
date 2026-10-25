// Behaviour test for 0x00C52CE0 (source body copied from src/00c52ce0.cpp; the retail inline-asm idioms are
// spelled through RA_* macros: the same __asm under VC7.1, a round-half-to-even C fallback under g++).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
// the retail inline-asm idioms, exactly as in the source (one instruction per __asm, no trailing semicolon)
#define RA_FISTP(v, r) __asm fld v __asm fistp r
#define RA_FISTP_PTR(v, p) __asm fld v __asm mov edx, p __asm fistp dword ptr [edx]
#define RA_RDTSC0(c) __asm xor eax, eax __asm xor edx, edx __asm rdtsc __asm mov dword ptr c, eax __asm mov dword ptr c+4, edx
#define RA_RDTSC(c) __asm rdtsc __asm mov dword ptr c, eax __asm mov dword ptr c+4, edx
#else
// g++ C fallback (test only): fistp in the default FPU mode rounds to nearest, ties to even
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
static long ra_round(double v) { return (long)__builtin_nearbyint(v); }
static unsigned long long ra_tsc = 1000000;
#define RA_FISTP(v, r) (r) = ra_round(v);
#define RA_FISTP_PTR(v, p) *(p) = ra_round(v);
#define RA_RDTSC0(c) (c) = (ra_tsc += 123457);
#define RA_RDTSC(c) (c) = (ra_tsc += 123457);
#endif
// ---- source body ----
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
    RA_RDTSC(counter)
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

// ---- harness ----

#include <math.h>
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
// independent reference: round half to even (the x87 default mode a bare fistp uses)
static long ref_rne(double v) { double f = floor(v); double d = v - f; long r = (long)f; if (d > 0.5 || (d == 0.5 && (r & 1))) ++r; return r; }
static long ref_trunc(double v) { long r = ref_rne(v - 0.5); if (r < 0) ++r; return r; }
static int near_(double a, double b, double eps) { return fabs(a - b) <= eps; }
#ifndef _MSC_VER
static long long g_qpc = 0;
extern "C" int QueryPerformanceFrequency(LARGE_INTEGER* f) { f->QuadPart = 1000000; return 1; }
extern "C" int QueryPerformanceCounter(LARGE_INTEGER* c) { g_qpc += 10000; c->QuadPart = g_qpc; return 1; }
extern "C" void Sleep(unsigned long) { }
#endif
int main()
{
    unsigned __int64 hz = MeasureCpuFrequency();
    check(hz > 0, "a positive clock");
#ifndef _MSC_VER
    check(hz == (unsigned __int64)123457 * 1000000 / 10000, "fake counters: delta * freq / count delta");
#endif
    if (!g_fail) printf("00c52ce0_TEST PASS\n");
    return g_fail;
}