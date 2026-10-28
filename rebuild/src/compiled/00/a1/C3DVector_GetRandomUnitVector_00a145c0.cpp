// C3DVector::GetRandomUnitVector(unsigned long* seed) -- a random direction on the unit sphere: a uniform height z in [-1, 1), then a random bearing on the circle of radius sqrt(1 - z*z)
// Leaf: ?GetRandomUnitVector@C3DVector@@SI?AV1@PAK@Z
// Retail-asm: two inlined copies of the engine's inline-assembly float-to-long helper, 0x00A14613 `fld [esp+14h]` +
//   0x00A14617 `fistp dword ptr [esp+0Ch]` and 0x00A14664 `fld [esp+18h]` + 0x00A14668 `fistp dword ptr [esp+8]`
//   (GFFastCos 0x00A14840 and GFFastSin 0x00A147E0 expanded in place); VC7.1 C++ `(long)x` calls __ftol2 and /QIfist
//   gives `fistp qword`, so the bare dword fistp is the original's `__asm` (the landed C2DVector::GetRandomUnitVector
//   0x00A13EB0 carries the same helper).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A145C0, 248 bytes, plain `ret` (static __fastcall: hidden result in ecx, the
//   seed in edx). Callers 0x00B0CC5E, 0x00B0D17A, 0x00B1118F, 0x00B114AC, 0x00B115DE (particle emitters).
// Identity: Ego_r.pdb / FableWin.pdb `?GetRandomUnitVector@C3DVector@@SI?AV1@PAK@Z`; donor order puts it after
//   C2DVector::Normalise (retail 0x00A14540) and before GetRandomPointWithinUnitSphere (0x00A146C0), as in retail.
// Callees: GFFloatRandom 0x004978A0 (`?GFFloatRandom@@YIMMPAK@Z`, seed in ecx, range pushed: 2.0f then 1.0f); the
//   cosine table 0x013CD550 of the inlined GFFastCos / GFFastSin.
// Shape: result.Z = random(2) - 1 (written straight into the result local), r = sqrt(1 - Z*Z), then the C2DVector::GetRandomUnitVector body (cos and sin of one random
//   turn fraction, scaled by 1024 once per inline) times r; X kept in a frame word, Y stored straight from st(0).
#include <math.h>
#pragma intrinsic(sqrt)

extern float g_CosTable[1025];          // 0x013CD550: cos(2*pi*k/1024), k = 0..1024

float __fastcall GFFloatRandom(float range, unsigned long* seed);    // 0x004978A0

// the engine's inline-assembly float-to-long helper (round to nearest, the FPU default); expanded in place
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

__forceinline float __fastcall GFFastCos(float angle)
{
    angle = angle * 1024.0f;
    long i = FloatToLong(angle - 0.5f);
    float frac = angle - (float)i;
    i &= 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}

__forceinline float __fastcall GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = FloatToLong(angle - 0.5f);
    float frac = angle - (float)i;
    i = (i - 256) & 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    static C3DVector __fastcall GetRandomUnitVector(unsigned long* seed);
};

C3DVector __fastcall C3DVector::GetRandomUnitVector(unsigned long* seed)
{
    C3DVector result;
    result.Z = GFFloatRandom(2.0f, seed) - 1.0f;
    float r = (float)sqrt(1.0f - result.Z * result.Z);
    float angle = GFFloatRandom(1.0f, seed);
    result.X = GFFastCos(angle) * r;
    result.Y = GFFastSin(angle) * r;
    return result;
}