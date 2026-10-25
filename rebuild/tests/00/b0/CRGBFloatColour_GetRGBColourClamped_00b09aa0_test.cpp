// Behaviour test for 0x00B09AA0 (source body copied from src/00b09aa0.cpp; the retail inline-asm idioms are
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
// CRGBFloatColour::GetRGBColourClamped() const -- converts a float RGBA colour to the 8-bit CRGBColour, each channel scaled by 255, rounded and clamped to [0, 255]
// Grade: RELOCATION_MATCH (2 grades: CRGBColour ctor assigns in its body, per GOTCHAS 0052f440) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: 0x00B09AB6, 0x00B09AD1, 0x00B09AEE, 0x00B09B09 `fistp dword ptr [esp+..]` each after `fstp [slot] ; fld [slot]` -- four expansions of the engine's inline-assembly GFFloatToLong helper (by-value copy stored and reloaded, bare dword fistp). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Leaf: ?GetRGBColourClamped@CRGBFloatColour@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B09AA0, length 233 bytes, thiscall `ret 4` (hidden result pointer).
// Signature from FableWin.pdb: ?GetRGBColourClamped@CRGBFloatColour@@QBE?AVCRGBColour@@XZ. No direct caller in retail.
// Constants: 255.0f @ 0x01230014.
// Layout: CRGBFloatColour R, G, B, A floats; CRGBColour the D3DCOLOR byte order B +0, G +1, R +2, A +3.
// Label: catalog "Color_FloatToRGBA_Clamp" is behavioural.
// the engine's inline-assembly float-to-long helper (round to nearest, the FPU default); expanded in place as a __forceinline
__forceinline long GFFloatToLong(float value)
{
    long result;
    RA_FISTP(value, result)
    return result;
}

class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

class CRGBFloatColour {
public:
    float R, G, B, A;
    CRGBColour GetRGBColourClamped() const;
};

CRGBColour CRGBFloatColour::GetRGBColourClamped() const
{
    long r = GFFloatToLong(R * 255.0f);
    long g = GFFloatToLong(G * 255.0f);
    long b = GFFloatToLong(B * 255.0f);
    long a = GFFloatToLong(A * 255.0f);
    if (r < 0) r = 0; else if (r > 255) r = 255;
    if (g < 0) g = 0; else if (g > 255) g = 255;
    if (b < 0) b = 0; else if (b > 255) b = 255;
    if (a < 0) a = 0; else if (a > 255) a = 255;
    return CRGBColour((unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a);
}

// ---- harness ----

#include <math.h>
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
// independent reference: round half to even (the x87 default mode a bare fistp uses)
static long ref_rne(double v) { double f = floor(v); double d = v - f; long r = (long)f; if (d > 0.5 || (d == 0.5 && (r & 1))) ++r; return r; }
static long ref_trunc(double v) { long r = ref_rne(v - 0.5); if (r < 0) ++r; return r; }
static int near_(double a, double b, double eps) { return fabs(a - b) <= eps; }

int main()
{
    CRGBFloatColour f; f.R = 1.0f; f.G = 0.5f; f.B = -0.2f; f.A = 2.0f;
    CRGBColour c = f.GetRGBColourClamped();
    check(c.R == 255 && c.G == 128 && c.B == 0 && c.A == 255, "clamp + 127.5 -> 128");
    f.R = 0.25f; f.G = 0.0f; f.B = 0.75f; f.A = 0.1f;
    c = f.GetRGBColourClamped();
    check(c.R == ref_rne(0.25f * 255.0f) && c.G == 0 && c.B == ref_rne(0.75f * 255.0f) && c.A == ref_rne(0.1f * 255.0f), "channels");
    if (!g_fail) printf("00b09aa0_TEST PASS\n");
    return g_fail;
}