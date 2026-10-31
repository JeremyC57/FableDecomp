// CRightHandedSet::Interpolate(const CRightHandedSet& other, float t) -- blend this transform towards another: the rotation quaternion by a table-driven slerp (shortest arc, plain lerp when the two are within 0.05 of parallel), the translation linearly
// Grade: DIFFER(422v671) -- retail_asm candidate: the C++ cast stand-in replaced by the landed __forceinline inline-asm
//   helper (FloatToLong / FloorToLong: bare dword fistp after the frame store/reload) with the Retail-asm evidence line;
//   137 diff lines from 176; residue: surrounding x87 scheduling, frame slots and inlining extent
// Retail-asm: 0x00AA3CDE `fistp dword ptr [esp + 4]` (and the other fistp sites) -- the engine's inline-assembly FloatToLong under FastFloor (`x - 0.5` stored to a frame word, reloaded by the asm fld, bare dword fistp; the landed FloatToLong of CPSCEmitterGeneric::CalcIsFinishedGeneratingParticles 0x00B10220); VC7.1 emits no bare dword fistp from C++.
// Leaf: Interpolate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA3C50, length 671 bytes (ret 8). No callees.
// Data: the arc-cosine table 0x013CB530 (indexed by (cos + 1) * 512.5, lerped between neighbours, times 1/(2 pi)
// 0x01237F08) and the cosine table 0x013CD550 (1024 entries per turn; sine = cosine 256 entries back, `& 0x3ff`).
// Layout: CRightHandedSet = the rotation quaternion (X, Y, Z, W) +0 and the translation (4 floats) +0x10.
#include <math.h>
extern const float g_ACosTable[];        // 0x013CB530
extern const float g_CosTable[];         // 0x013CD550

struct CQuaternion {
    float X, Y, Z, W;
};
struct C4DVector {
    float X, Y, Z, W;
};

__forceinline long FloatToLong(float value)   // the engine's inline-assembly float-to-long (current rounding mode)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
__forceinline long FastFloor(float value)
{
    return FloatToLong(value - 0.5f);
}
static inline float TableACos(float cosine)
{
    float index = (cosine + 1.0f) * 512.5f;
    long i = FastFloor(index);
    float frac = index - (float)i;
    return (g_ACosTable[i] * (1.0f - frac) + g_ACosTable[i + 1] * frac) * 0.15915494f;
}
static inline float TableSin(float turns)
{
    float index = turns * 1024.0f;
    long i = FastFloor(index);
    float frac = index - (float)i;
    i = (i - 256) & 0x3ff;
    return g_CosTable[i] * (1.0f - frac) + g_CosTable[i + 1] * frac;
}

struct CRightHandedSet {
    CQuaternion Rotation;                 // +0x00
    C4DVector   Translation;              // +0x10
    void Interpolate(const CRightHandedSet& other, float t);
};

void CRightHandedSet::Interpolate(const CRightHandedSet& other, float t)
{
    float sign = 1.0f;
    float cosom = other.Rotation.Z * Rotation.Z + other.Rotation.Y * Rotation.Y + other.Rotation.X * Rotation.X
        + Rotation.W * other.Rotation.W;
    if (cosom < 0.0f) {
        cosom = -cosom;
        sign = -1.0f;
    }
    float scale0, scale1;
    if (1.0f - cosom > 0.05f) {
        float omega = TableACos(cosom);
        float invSin = 1.0f / sqrtf(1.0f - cosom * cosom);
        scale0 = TableSin((1.0f - t) * omega) * invSin;
        scale1 = TableSin(omega * t) * invSin;
    } else {
        scale0 = 1.0f - t;
        scale1 = t;
    }
    scale1 *= sign;
    CQuaternion q;
    q.X = scale1 * other.Rotation.X + scale0 * Rotation.X;
    q.Y = scale1 * other.Rotation.Y + scale0 * Rotation.Y;
    q.Z = scale1 * other.Rotation.Z + scale0 * Rotation.Z;
    q.W = scale0 * Rotation.W + scale1 * other.Rotation.W;
    Rotation = q;
    C4DVector d;
    d.X = (other.Translation.X - Translation.X) * t + Translation.X;
    d.Y = (other.Translation.Y - Translation.Y) * t + Translation.Y;
    d.Z = (other.Translation.Z - Translation.Z) * t + Translation.Z;
    d.W = (other.Translation.W - Translation.W) * t + Translation.W;
    Translation = d;
}
