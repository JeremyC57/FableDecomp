// Behaviour test for 0x00BA7B30 (source body copied from src/00ba7b30.cpp; the retail inline-asm idioms are
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
// CEngineInternalPrimitiveBase::GetAlphaIncludingDistanceFade(float distance) -- the primitive's alpha faded out linearly across its fade range: 0 when the alpha is 0 or the distance is beyond the draw distance, the full alpha before the fade start, else alpha * (255 - 255 * t) >> 8
// Grade: RELOCATION_MATCH (3 grades: the int result in its own {} block so VC homes it in the dead distance slot) -- rasm1 retail_asm: C++ body + a minimal 3-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: 0x00BA7BB2 `fstp [esp+0Ch]` / 0x00BA7BB6 `fld dword ptr [esp+0Ch]` + 0x00BA7BBA `mov edx,[esp+4]` + 0x00BA7BBE `fistp dword ptr [edx]` -- the engine's pointer-out inline-assembly float-to-int helper (as the landed retail_asm 0x00B1B5E0, 3 instr; the int homed in the dead `distance` slot). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Leaf: ?GetAlphaIncludingDistanceFade@CEngineInternalPrimitiveBase@@QAEEM
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BA7B30, 172 bytes, thiscall, `ret 4`, unsigned char in al.
// Identity: Ego_r.pdb ?GetAlphaIncludingDistanceFade@CEngineInternalPrimitiveBase@@QAEEM@Z (the only uchar(float) member
//   of the class; non-virtual). The body reads Alpha (+0x27), FadeDistance (+0x14) and the flag byte +0x34 bit 2
//   (OverrideFadeDistance, as the landed CEngineInternalPrimitiveMeshBase::GetFadeDistance 0x00BEB5C0 reads it), and calls
//   vtable slot 13 (+0x34) = GetFadeDistance (slot list as the landed 0x00B92660 models it). Catalog
//   "CFrustum_ComputeAlphaBlend" is wrong.
// Layout: engine/CEngineInternalPrimitiveBase.h models the vptr as a data member and keeps the bitfield byte opaque, so
//   the virtual GetFadeDistance call needs the polymorphic local model below (same offsets).
// Shape: one shared `xor al,al` return for Alpha == 0 and distance > MaxDrawDistance (`test ah,41h ; jne`); `<` begin is
//   `test ah,5 ; jp`; the fade pair is one local copied from either the virtual call's hidden-pointer result or the member
//   (CEngineFadeDistance needs a user constructor for the hidden-pointer return); 255.0f is the literal at 0x01230014.

// the engine's pointer-out float-to-int helper (round to nearest, the FPU default); expanded in place as a __forceinline,
// its two by-value parameters get frame copies
__forceinline void FloatToInt(int* result, float value)
{
    RA_FISTP_PTR(value, result)
}

class CEngineFadeDistance {                        // Ego_r.pdb, 8 bytes
public:
    float FadeBeginDistance;                       // +0x00
    float MaxDrawDistance;                         // +0x04
    CEngineFadeDistance(float begin, float maxDraw) : FadeBeginDistance(begin), MaxDrawDistance(maxDraw) { }
};

class CEngineInternalPrimitiveBase {               // retail layout of engine/CEngineInternalPrimitiveBase.h, polymorphic
public:
    virtual ~CEngineInternalPrimitiveBase();       // slot 0
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual CEngineFadeDistance GetFadeDistance() const;   // slot 13 (+0x34)

    unsigned long       IVRefCount;                // +0x04 (CIVCountedPointeeBase)
    long                Type;                      // +0x08 (CEnginePersistentPrimitiveBase)
    unsigned long       EnginePrimitiveFlags;      // +0x0c
    float               AlphaMipBias;              // +0x10
    CEngineFadeDistance FadeDistance;              // +0x14
    unsigned long       OcclusionLayerBitMask;     // +0x1c
    void*               GridCell;                  // +0x20
    signed char         GridLevel;                 // +0x24
    unsigned char       GridX, GridY;              // +0x25
    unsigned char       Alpha;                     // +0x27
    unsigned char       AlphaRef;                  // +0x28
    unsigned char       PreloadStat;               // +0x29
    int                 InterpolationSet;          // +0x2c
    unsigned long       RenderLayerMask;           // +0x30
    unsigned char       Enable : 1;                // +0x34
    unsigned char       NonPersistent : 1;
    unsigned char       OverrideFadeDistance : 1;
    unsigned char       PostRenderEnable : 1;

    unsigned char GetAlphaIncludingDistanceFade(float distance);
    CEngineInternalPrimitiveBase() : FadeDistance(0.0f, 0.0f) { }   // test only
};

unsigned char CEngineInternalPrimitiveBase::GetAlphaIncludingDistanceFade(float distance)
{
    if (Alpha == 0)
        return 0;
    CEngineFadeDistance fade = OverrideFadeDistance ? GetFadeDistance() : FadeDistance;
    if (distance > fade.MaxDrawDistance)
        return 0;
    if (distance < fade.FadeBeginDistance)
        return Alpha;
    {
        int fadeAmount;
        FloatToInt(&fadeAmount, (distance - fade.FadeBeginDistance) * 255.0f / (fade.MaxDrawDistance - fade.FadeBeginDistance));
        return (unsigned char)(((255 - fadeAmount) * Alpha) >> 8);
    }
}

// ---- harness ----

#include <math.h>
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
// independent reference: round half to even (the x87 default mode a bare fistp uses)
static long ref_rne(double v) { double f = floor(v); double d = v - f; long r = (long)f; if (d > 0.5 || (d == 0.5 && (r & 1))) ++r; return r; }
static long ref_trunc(double v) { long r = ref_rne(v - 0.5); if (r < 0) ++r; return r; }
static int near_(double a, double b, double eps) { return fabs(a - b) <= eps; }

CEngineInternalPrimitiveBase::~CEngineInternalPrimitiveBase() { }
void CEngineInternalPrimitiveBase::v01() { }
void CEngineInternalPrimitiveBase::v02() { }
void CEngineInternalPrimitiveBase::v03() { }
void CEngineInternalPrimitiveBase::v04() { }
void CEngineInternalPrimitiveBase::v05() { }
void CEngineInternalPrimitiveBase::v06() { }
void CEngineInternalPrimitiveBase::v07() { }
void CEngineInternalPrimitiveBase::v08() { }
void CEngineInternalPrimitiveBase::v09() { }
void CEngineInternalPrimitiveBase::v10() { }
void CEngineInternalPrimitiveBase::v11() { }
void CEngineInternalPrimitiveBase::v12() { }
CEngineFadeDistance CEngineInternalPrimitiveBase::GetFadeDistance() const { return CEngineFadeDistance(10.0f, 20.0f); }
int main()
{
    CEngineInternalPrimitiveBase* p = new CEngineInternalPrimitiveBase;
    p->FadeDistance = CEngineFadeDistance(100.0f, 200.0f);
    p->OverrideFadeDistance = 0;
    p->Alpha = 0;     check(p->GetAlphaIncludingDistanceFade(150.0f) == 0, "alpha 0");
    p->Alpha = 200;
    check(p->GetAlphaIncludingDistanceFade(250.0f) == 0, "beyond max draw");
    check(p->GetAlphaIncludingDistanceFade(50.0f) == 200, "before fade");
    check(p->GetAlphaIncludingDistanceFade(150.0f) == (((255 - 128) * 200) >> 8), "half fade (127.5 -> 128)");
    p->OverrideFadeDistance = 1;
    check(p->GetAlphaIncludingDistanceFade(15.0f) == (((255 - 128) * 200) >> 8), "override fade distance");
    check(p->GetAlphaIncludingDistanceFade(17.5f) == (((255 - ref_rne(7.5 * 255 / 10)) * 200) >> 8), "override 3/4");
    if (!g_fail) printf("00ba7b30_TEST PASS\n");
    return g_fail;
}