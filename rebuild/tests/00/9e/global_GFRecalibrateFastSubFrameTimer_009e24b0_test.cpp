// Behaviour test for 0x009E24B0 (source body copied from src/009e24b0.cpp; the retail inline-asm idioms are
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
// GFRecalibrateFastSubFrameTimer() -- once at least a second has passed, re-measure the CPU cycles per second that the fast sub-frame timer divides by
// Grade: RELOCATION_MATCH (2 grades: the delta written inline in the division) -- rasm1 retail_asm: C++ body + a minimal 5-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E24B0, length 139 bytes. PDB: ?GFRecalibrateFastSubFrameTimer@@YIXXZ (void __fastcall()).
// Retail-asm: 0x009E24D5 `xor eax,eax` / 0x009E24D7 `xor edx,edx` / 0x009E24D9 `rdtsc` with edx:eax stored to the 8-byte local (0x009E24DB/0x009E24DF) and reloaded -- the PDB's GFGetCycleCount inlined, the same block as the landed CPUAheadCounterCallback (0x009E2540); VC7.1 (13.10.3077/4035) has no rdtsc intrinsic (__rdtsc/<intrin.h> arrived with VC8), so the original read the counter with inline asm.
// Callees: GFGetTime @ 0x009E1BC0 (double). Globals: last recalibration time 0x013CAA20 (double), last recalibration cycle
//   count 0x013CAA18 (64-bit), frame-start cycle count 0x013CA9F8 (64-bit), cycles per second 0x01396680 (double); 1.0 is the
//   double pool constant 0x01237F00.
// Shape: `elapsed < 1.0` early return (`test ah,5 ; jnp`), the signed 64-bit delta converted with `fild qword`, the counter
//   published to both cycle globals, the time stored last.
// Leaf: ?GFRecalibrateFastSubFrameTimer@@YIXXZ
typedef unsigned __int64 UHUGE;

double __fastcall GFGetTime();                            // 0x009E1BC0
// the engine's cycle-counter read (expanded in place: zero edx:eax, rdtsc, spill to the 8-byte local)
__forceinline UHUGE GFGetCycleCount()
{
    UHUGE counter;
    RA_RDTSC0(counter)
    return counter;
}

extern double g_LastRecalibrateTime;                      // 0x013CAA20
extern UHUGE g_LastRecalibrateCycleCount;                 // 0x013CAA18
extern UHUGE g_FrameStartCycleCount;                      // 0x013CA9F8
extern double g_CyclesPerSecond;                          // 0x01396680

void __fastcall GFRecalibrateFastSubFrameTimer()
{
    double now = GFGetTime();
    double elapsed = now - g_LastRecalibrateTime;
    if (elapsed < 1.0)
        return;
    UHUGE cycles = GFGetCycleCount();
    g_CyclesPerSecond = (double)(__int64)(cycles - g_LastRecalibrateCycleCount) / elapsed;
    g_FrameStartCycleCount = cycles;
    g_LastRecalibrateCycleCount = cycles;
    g_LastRecalibrateTime = now;
}

// ---- harness ----

#include <math.h>
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
// independent reference: round half to even (the x87 default mode a bare fistp uses)
static long ref_rne(double v) { double f = floor(v); double d = v - f; long r = (long)f; if (d > 0.5 || (d == 0.5 && (r & 1))) ++r; return r; }
static long ref_trunc(double v) { long r = ref_rne(v - 0.5); if (r < 0) ++r; return r; }
static int near_(double a, double b, double eps) { return fabs(a - b) <= eps; }

static double g_now = 0.0;
double __fastcall GFGetTime() { return g_now; }
double g_LastRecalibrateTime = 0.0;
UHUGE g_LastRecalibrateCycleCount = 0;
UHUGE g_FrameStartCycleCount = 0;
double g_CyclesPerSecond = 1.0e9;
int main()
{
    g_now = 0.5; g_FrameStartCycleCount = 5;
    GFRecalibrateFastSubFrameTimer();
    check(g_FrameStartCycleCount == 5 && g_CyclesPerSecond == 1.0e9 && g_LastRecalibrateTime == 0.0, "under a second: untouched");
    g_now = 2.0;
    GFRecalibrateFastSubFrameTimer();
    check(g_FrameStartCycleCount != 5 && g_LastRecalibrateCycleCount == g_FrameStartCycleCount, "both cycle globals published");
    check(near_(g_CyclesPerSecond * 2.0, (double)(__int64)g_FrameStartCycleCount, 1.0), "cycles / elapsed");
    check(g_LastRecalibrateTime == 2.0, "time stored");
    if (!g_fail) printf("009e24b0_TEST PASS\n");
    return g_fail;
}