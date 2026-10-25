// Behaviour test for 0x00BE6800 (source body copied from src/00be6800.cpp; the retail inline-asm idioms are
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
// CLandscapeBackgroundPatch::GetPackedNormalFromTargetVertex(const CTVertexLandscapeBackground& vertex) -- pack the vertex's float normal into the 11:11:10 word (x*1023 bits 0-10, y*1023 bits 11-21, z*511 bits 22-31, each rounded)
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: three expansions of the engine's inline-assembly ROUND helper (each component product stored to the by-value copy in a dead argument slot, reloaded and rounded with a bare `fistp dword`), the same helper as the landed retail_asm PackVertexNormal 0x00BE6790/0x00BFDF60. VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Leaf: ?GetPackedNormalFromTargetVertex@CLandscapeBackgroundPatch@@SIKABVCTVertexLandscapeBackground@@@Z
// Retail-asm evidence: 0x00BE681A `fstp dword ptr [esp] ; fld dword ptr [esp] ; fistp dword ptr [esp+8]` (x3, z then y then x), the same
// helper the landed CPackedNormalOwner_00be6790::PackVertexNormal (grade retail_asm) expands; VC7.1 C++ `(long)x` calls __ftol2 and
// /QIfist gives `fistp qword` without the reload (GOTCHAS wave 25 chunk 145).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE6800, 113 bytes. Ego_r.pdb:
// ?GetPackedNormalFromTargetVertex@CLandscapeBackgroundPatch@@SIKABVCTVertexLandscapeBackground@@@Z (static __fastcall, vertex in ecx).
// Float pool: 0x0129EF90 = 1023.0f, 0x0129EF94 = 511.0f. No callers by direct call (reached through an inlined/indirect path).
// Shape: the normal is first copied into a C3DVector local (X/Y as integer moves into the frame, Z kept on the FPU stack), then
// PackVertexNormal's body runs inline: z, y, x rounded into a 3-word frame and combined as ((y & 0x7ff) | (z << 11)) << 11 | (x & 0x7ff).
// The ROUND helper is the one of the landed retail_asm source 0x00BE6790.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CTVertexLandscapeBackground {
public:
    unsigned short X, Y;                           // +0x00 / +0x02
    float          Z;                              // +0x04
    float          NormalX, NormalY, NormalZ;      // +0x08 / +0x0c / +0x10
    unsigned char  UVPad1;                         // +0x14
};
// the engine's inline-assembly ROUND helper (round to nearest, the FPU default); expanded in place as a __forceinline
__forceinline long ROUND(float value)
{
    long result;
    RA_FISTP(value, result)
    return result;
}

__forceinline unsigned long PackVertexNormal(const C3DVector& normal)
{
    long z = ROUND(normal.Z * 511.0f);
    long y = ROUND(normal.Y * 1023.0f);
    long x = ROUND(normal.X * 1023.0f);
    return (((y & 0x7ff) | (z << 11)) << 11) | (x & 0x7ff);
}

class CLandscapeBackgroundPatch {
public:
    static unsigned long __fastcall GetPackedNormalFromTargetVertex(const CTVertexLandscapeBackground& vertex);
};

unsigned long __fastcall CLandscapeBackgroundPatch::GetPackedNormalFromTargetVertex(const CTVertexLandscapeBackground& vertex)
{
    C3DVector normal(vertex.NormalX, vertex.NormalY, vertex.NormalZ);
    return PackVertexNormal(normal);
}

// ---- harness ----

#include <math.h>
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
// independent reference: round half to even (the x87 default mode a bare fistp uses)
static long ref_rne(double v) { double f = floor(v); double d = v - f; long r = (long)f; if (d > 0.5 || (d == 0.5 && (r & 1))) ++r; return r; }
static long ref_trunc(double v) { long r = ref_rne(v - 0.5); if (r < 0) ++r; return r; }
static int near_(double a, double b, double eps) { return fabs(a - b) <= eps; }
#define VTX CTVertexLandscapeBackground
#define CALL(v) CLandscapeBackgroundPatch::GetPackedNormalFromTargetVertex(v)

static unsigned long ref_pack(float x, float y, float z)
{
    long rz = ref_rne(z * 511.0f), ry = ref_rne(y * 1023.0f), rx = ref_rne(x * 1023.0f);
    return (((ry & 0x7ff) | (rz << 11)) << 11) | (rx & 0x7ff);
}
int main()
{
    VTX v; memset(&v, 0, sizeof(v));
    float n[][3] = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {-1, 0, 0}, {0.5f, 0.5f, 0.5f}, {0.25f, -0.75f, -0.125f} };
    for (int i = 0; i < 6; ++i) {
        v.NormalX = n[i][0]; v.NormalY = n[i][1]; v.NormalZ = n[i][2];
        check(CALL(v) == ref_pack(n[i][0], n[i][1], n[i][2]), "pack");
    }
    v.NormalX = 0.5f; v.NormalY = 0.5f; v.NormalZ = 0.5f;
    check(CALL(v) == ((((512 & 0x7ff) | (256 << 11)) << 11) | 512), "ties to even (511.5 -> 512, 255.5 -> 256)");
    if (!g_fail) printf("00be6800_TEST PASS\n");
    return g_fail;
}