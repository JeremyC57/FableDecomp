// Behaviour test for 0x005BC3ED (source body copied from src/005bc3ed.cpp; the retail inline-asm idioms are
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
// `anonymous namespace'::GFGetGraphicFrame(float scale, unsigned long frameCount, unsigned long firstFrame, unsigned long frame) -- scale a sprite frame's offset from the first frame (wrapping round the frame count) and map the rounded result back into [0, frameCount)
// Grade: RELOCATION_MATCH (2 grades: an external keep-pointer replaces the parked call wrapper) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: 0x005BC417 `fstp [ebp+0Ch]` / 0x005BC41A `fld dword ptr [ebp+0Ch]` + 0x005BC41D `fistp dword ptr [ebp+8]` -- the engine's inline-assembly ROUND helper (the product stored to its by-value copy in the dead `frame` slot and reloaded, a bare dword fistp into the dead `scale` slot). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Leaf: ?GFGetGraphicFrame@?A
// Retail: Fable.exe @ 0x005BC3ED, 67 bytes, `ret 8`. Ego_r.pdb: `?GFGetGraphicFrame@?A0x9755b541@@YIKMKKK@Z` (anonymous
// namespace, __fastcall, unsigned long (float, unsigned long, unsigned long, unsigned long)): the two integral leading
// arguments go to ecx (frameCount) / edx (firstFrame), the float and the last word to the stack. No direct callers
// (xref finds none: reached through an inlined/indirect use in the sprite TU).
// Body: offset = frame >= firstFrame ? frame - firstFrame : frameCount - firstFrame + frame;
//       result = Round(scale * (float)offset) + firstFrame; if (result >= frameCount) result -= frameCount;
//   `fild ; jge ; fadd [2^32]` is the unsigned -> float conversion, `fmul [ebp+8]` the scale.
#pragma optimize("y", off)
#pragma optimize("s", on)
// the engine's inline-assembly ROUND helper (round to nearest, the FPU default); expanded in place as a __forceinline
__forceinline long ROUND(float value)
{
    long result;
    RA_FISTP(value, result)
    return result;
}
namespace {
unsigned long __fastcall GFGetGraphicFrame(float scale, unsigned long frameCount, unsigned long firstFrame, unsigned long frame)
{
    unsigned long offset;
    if (frame >= firstFrame)
        offset = frame - firstFrame;
    else
        offset = frameCount - firstFrame + frame;
    unsigned long result = ROUND(scale * (float)offset) + firstFrame;
    if (result >= frameCount)
        result -= frameCount;
    return result;
}
}
// keeps the internal-linkage body emitted (the retail copy has no direct caller); not part of the retail function
unsigned long (__fastcall *g_keepGFGetGraphicFrame_005BC3ED)(float, unsigned long, unsigned long, unsigned long) = &GFGetGraphicFrame;

// ---- harness ----

#include <math.h>
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
// independent reference: round half to even (the x87 default mode a bare fistp uses)
static long ref_rne(double v) { double f = floor(v); double d = v - f; long r = (long)f; if (d > 0.5 || (d == 0.5 && (r & 1))) ++r; return r; }
static long ref_trunc(double v) { long r = ref_rne(v - 0.5); if (r < 0) ++r; return r; }
static int near_(double a, double b, double eps) { return fabs(a - b) <= eps; }

static unsigned long ref_frame(float scale, unsigned long count, unsigned long first, unsigned long frame)
{
    unsigned long offset = frame >= first ? frame - first : count - first + frame;
    unsigned long r = (unsigned long)ref_rne((double)(scale * (float)offset)) + first;
    return r >= count ? r - count : r;
}
int main()
{
    check(GFGetGraphicFrame(0.5f, 10, 2, 7) == 4, "0.5 * 5 -> 2 (tie to even) + 2");
    check(GFGetGraphicFrame(1.0f, 10, 8, 3) == 3, "wrap");
    check(GFGetGraphicFrame(0.75f, 16, 0, 6) == ref_frame(0.75f, 16, 0, 6), "4.5 -> 4");
    check(GFGetGraphicFrame(2.0f, 12, 5, 9) == ref_frame(2.0f, 12, 5, 9), "wrap after scale");
    if (!g_fail) printf("005bc3ed_TEST PASS\n");
    return g_fail;
}