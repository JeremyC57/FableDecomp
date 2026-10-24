// C2DVector::GetRandomUnitVector(unsigned long* seed) -- a unit vector at a random angle: (cos, sin) of a random turn fraction from the cosine table
// Grade: DIFFER(165v191) -- retail_asm candidate: both inlined table lookups round with a bare `fld [mem] ; fistp dword [mem]` (inline asm in the engine's float-to-long helper, each through its own parameter copy: the scaled angle is stored to two frame words); C++ `(long)x` calls __ftol2 twice and the lookups interleave differently. next: retail_asm with the landed float-to-long helper form (a __forceinline FloatToLong(float value) holding the bare fld/fistp; see rebuild/src/compiled/00/af/NParticleEngineCPSCLight_SmoothFadeValue_00af9000.cpp); the GFFastCos/GFFastSin inlines then need the by-value argument copies
// Leaf: ?GetRandomUnitVector@C2DVector@@SI?AV1@PAK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A13EB0, 191 bytes, plain `ret` (static __fastcall: hidden result pointer in ecx,
//   seed in edx). Caller 0x00B110BD. PDB name (Ego_r and FableWin) ?GetRandomUnitVector@C2DVector@@SI?AV1@PAK@Z.
// Callees: GFFloatRandom 0x004978A0 (`?GFFloatRandom@@YIMMPAK@Z`, seed in ecx, max 1.0f pushed); GFFastCos 0x00A14840
//   and GFFastSin 0x00A147E0 inlined (cosine table 0x013CD550).
// Shape: the random turn fraction is scaled by 1024 once (shared by both inlines), X = cos lookup (no shift), Y = sin
//   lookup (-256), X kept in ecx until the end, Y stored straight from st(0).
extern float g_CosTable[1025];          // 0x013CD550: cos(2*pi*k/1024), k = 0..1024

float __fastcall GFFloatRandom(float max, unsigned long* seed);    // 0x004978A0

__forceinline float __fastcall GFFastCos(float angle)
{
    angle = angle * 1024.0f;
    long i = (long)(angle - 0.5f);
    float frac = angle - (float)i;
    i &= 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}

__forceinline float __fastcall GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = (long)(angle - 0.5f);
    float frac = angle - (float)i;
    i = (i - 256) & 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}

class C2DVector {
public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}
    static C2DVector __fastcall GetRandomUnitVector(unsigned long* seed);
};

C2DVector __fastcall C2DVector::GetRandomUnitVector(unsigned long* seed)
{
    float angle = GFFloatRandom(1.0f, seed);
    C2DVector result(GFFastCos(angle), GFFastSin(angle));
    return result;
}
