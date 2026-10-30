// NBBBMaths::IsPosInFOVWithWeight(const C3DVector& pos, const C3DVector& dir, float fov, const C3DVector& target, float& weight) -- is target inside the view cone of half-angle fov/2 around dir from pos; weight = how far inside (0 on the edge, 1 dead centre)
// Grade: DIFFER(312v314) retry1 -- 2 more forms; residue: Newton r spill vs register
// Retail-asm: 0x00A4B68E `fstp [esp+8]` / 0x00A4B692 `fld dword ptr [esp+8]` + 0x00A4B696 `fistp dword ptr [esp+4]` -- the engine's inline-assembly float-to-long helper inside the inlined GFFastCos (landed retail_asm 0x00A14840): a bare dword fistp, which VC7.1 C++ `(long)x` (__ftol2) or /QIfist (`fistp qword`) never emits.
// Leaf: ?IsPosInFOVWithWeight@NBBBMaths@@YI_NABVC3DVector@@0M0AAM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4B670, 314 bytes, `__fastcall` (pos ecx, dir edx), `ret 0Ch`.
// Identity: both PDBs name `?IsPosInFOVWithWeight@NBBBMaths@@YI_NABVC3DVector@@0M0AAM@Z` (bool, fastcall, two vector
//   references, a float, a vector reference, a float reference), which is exactly this body's convention; callers
//   0x006EB7DA (CCombatManager::GetBestEnemyForCombatant) and 0x00A4B80F.
// Callees: none. Globals: the 1025-entry cosine table 0x013CD550, 0.5f 0x0122F59C, 1024.0f 0x01230010, 1.0f 0x0122DED8,
//   3.0f 0x0122DED4, the reciprocal-square-root mantissa table 0x013CE558.
// Shape: cos(fov/2) through the inlined GFFastCos (landed 0x00A14840: the scaled angle written back to the fov slot);
//   the direction to the target fast-normalised in place (the engine's inline Normalise, landed 0x00759B40); the dot
//   product with dir against the cosine; on success the weight is (dot - cos) / (1 - cos), else 0.
extern float g_CosTable[1025];                      // 0x013CD550: cos(2*pi*k/1024), k = 0..1024
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

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

class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float operator*(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    __forceinline void Normalise()
    {
        float lengthSquared = Z * Z;
        lengthSquared += X * X;
        lengthSquared += Y * Y;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r;
        r *= 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};

namespace NBBBMaths {

bool __fastcall IsPosInFOVWithWeight(const C3DVector& pos, const C3DVector& dir, float fov, const C3DVector& target,
                                     float& weight)
{
    float cosHalfFov = GFFastCos(fov * 0.5f);
    C3DVector toTarget = target - pos;
    toTarget.Normalise();
    float dot = toTarget * dir;
    if (dot > cosHalfFov) {
        weight = (dot - cosHalfFov) / (1.0f - cosHalfFov);
        return true;
    }
    weight = 0.0f;
    return false;
}

}
