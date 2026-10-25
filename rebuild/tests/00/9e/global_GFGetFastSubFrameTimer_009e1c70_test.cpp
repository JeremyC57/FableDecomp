// Behaviour test for 0x009E1C70 (source body copied from src/009e1c70.cpp; the retail inline-asm idioms are
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
// GFGetFastSubFrameTimer() -- cycle-counter time: (rdtsc - base) / cycles-per-second, a cheap sub-frame timer
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 5-instruction __asm (no control flow) + the Retail-asm evidence line
// Leaf: ?GFGetFastSubFrameTimer@@YINXZ
// Retail-asm: 0x009E1C73 `xor eax,eax` / 0x009E1C75 `xor edx,edx` / 0x009E1C77 `rdtsc` with edx:eax stored to the 8-byte local at [esp] (0x009E1C79/0x009E1C7C); VC7.1 (13.10.3077/4035) has no rdtsc intrinsic (__rdtsc/<intrin.h> arrived with VC8), so the original read the counter with inline asm (same block as the landed 0x009E2540 CPUAheadCounterCallback).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E1C70, length 58 bytes (+ int3 padding).
// PDB (FableWin): ?GFGetFastSubFrameTimer@@YINXZ -- __fastcall, no arguments, double result on the FPU.
// Globals: the 64-bit counter base at 0x013CA9F8 / 0x013CA9FC (written by the timer initialisation) and the double
// cycles-per-second at 0x01396680 (a .data global initialised to 1.0e9, not a literal-pool constant).
// Body after the asm block is ordinary C++: the 64-bit subtraction through the frame words, `fild qword [esp] ;
// fdiv qword [g]`.
extern __int64 g_FastTimerBase;           // 0x013CA9F8
extern double g_CyclesPerSecond;          // 0x01396680 (1.0e9)

double __fastcall GFGetFastSubFrameTimer()
{
    __int64 now;
    RA_RDTSC0(now)
    now -= g_FastTimerBase;
    return (double)now / g_CyclesPerSecond;
}

// ---- harness ----

#include <math.h>
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
// independent reference: round half to even (the x87 default mode a bare fistp uses)
static long ref_rne(double v) { double f = floor(v); double d = v - f; long r = (long)f; if (d > 0.5 || (d == 0.5 && (r & 1))) ++r; return r; }
static long ref_trunc(double v) { long r = ref_rne(v - 0.5); if (r < 0) ++r; return r; }
static int near_(double a, double b, double eps) { return fabs(a - b) <= eps; }

__int64 g_FastTimerBase = 0;
double g_CyclesPerSecond = 1.0e9;
int main()
{
    double a = GFGetFastSubFrameTimer();
    double b = GFGetFastSubFrameTimer();
    check(a > 0.0 && b >= a, "counter runs forward");
    g_FastTimerBase = (__int64)((b + 1000.0) * 1.0e9);
    check(GFGetFastSubFrameTimer() < 0.0, "base after now gives a negative time");
    g_FastTimerBase = 0; g_CyclesPerSecond = 2.0e9;
    double c = GFGetFastSubFrameTimer();
    check(c > 0.0 && c < b, "rate divides");
    if (!g_fail) printf("009e1c70_TEST PASS\n");
    return g_fail;
}