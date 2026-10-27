// QuadricOptimiserInternals::COptimisedPrimitive::MakeOrthogonal(const C3DVector& v, const C3DVector& normal) const -- remove v's component along the normal and rescale the remainder back to v's original length (v unchanged when it is near zero, unscaled when the remainder is)
// Grade: DIFFER(255v255) -- only the dot product's term order: retail `v.x*n.x ; v.y*n.y ; v.z*n.z` (fld [ecx+8] ; fmul [eax+8] last), ours schedules the z term second or loads n.z first; it flips with unrelated declarations in the TU (probed Dot written 12 ways, as += chain, operator*, named local)
// Leaf: ?MakeOrthogonal@COptimisedPrimitive@QuadricOptimiserInternals@@ABE?AVC3DVector@@ABV3@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC7200, 255 bytes, thiscall `ret 0Ch` (hidden result pointer, v, normal; `this`
//   is never read, ecx is reused at once).
// Identity: Ego_r.pdb `?MakeOrthogonal@COptimisedPrimitive@QuadricOptimiserInternals@@ABE?AVC3DVector@@ABV3@0@Z`
//   (private, const): in Ego_r donor order it sits between CQuadric::GetVectorPart / GetMatrixPart and
//   COptimisedPrimitive::FindTextureTangent, exactly as retail 0x00AC70F0 (CQuadric::GetMatrixPart) / 0x00AC7200 /
//   0x00AC7300 (FindTextureTangent, which calls it). The catalog's "Vector_ProjectOntoPlane" is descriptive.
// Callees: none; the epsilon is the shared float 0x0129BA3C (1e-4), read twice for its square (a non-literal const,
//   so VC7.1 does not fold `eps * eps`).
// Shape: C3DVector with user constructors (FableWin.pdb ??0C3DVector@@QAE@ABV0@@Z), so the result stays in x87
//   registers; <math.h>'s float sqrt overload (sqrt(float) -> sqrtf -> (float)sqrt(double)) gives the magnitude its
//   rounding store and reload before GFFloatAbs; inline C3DVector helpers (GetMagnitude, Dot, GetSquaredMagnitude, operator*, -=, *=) and GFFloatAbs, whose
//   integer-mask body stores the magnitude, reloads it as a dword and ands off the sign bit.
#include "engine/COptimisedPrimitive.h"

#include <math.h>

extern const float g_floatEpsilon;   // 0x0129BA3C (1e-4)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& operator-=(const C3DVector& o) { X -= o.X; Y -= o.Y; Z -= o.Z; return *this; }
    C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
    float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    float GetSquaredMagnitude() const { return X * X + Y * Y + Z * Z; }
    float GetMagnitude() const { return sqrt(X * X + Y * Y + Z * Z); }
};

inline float __fastcall GFFloatAbs(float f)
{
    long bits = *(long*)&f;
    bits &= 0x7fffffff;
    return *(float*)&bits;
}

namespace QuadricOptimiserInternals {
class COptimisedPrimitive : public ::COptimisedPrimitive {
private:
    C3DVector MakeOrthogonal(const C3DVector& v, const C3DVector& normal) const;
};

C3DVector COptimisedPrimitive::MakeOrthogonal(const C3DVector& v, const C3DVector& normal) const
{
    C3DVector result = v;
    float magnitude = v.GetMagnitude();
    if (GFFloatAbs(magnitude) > g_floatEpsilon) {
        result -= normal * v.Dot(normal);
        float squared = result.GetSquaredMagnitude();
        if (squared > g_floatEpsilon * g_floatEpsilon)
            result *= magnitude / sqrt(squared);
    }
    return result;
}
}
