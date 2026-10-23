// Grade: DIFFER(699v707) -- one x87 residue: retail homes |dir|^2 in a compiler temp (fst [esp+2Ch]; fxch; fcompp) and copies it to a with integer moves; ours keeps it on the stack (fld st(1); fcompp; fstp [a]). Advisor form (Dot by const reference, !(len > eps*eps)); also tried an array/struct home and a forceinline solver taking a by value
// so dir's components are first loaded directly at the dir*t products (fld dir.x ; fmul [t]); predicate closed: `!(len > eps*eps)`
// gives retail's `test ah,41h ; jne`. Left (8 bytes): the length-squared CSE value is duplicated on the x87 stack (fld st(1) ;
// fcompp ; fstp [a]) where retail homes it (fst [temp] ... fxch ; fcompp ... mov eax,[temp] ; mov [a],eax).
// CSphere::GetFirstIntersectionWithFiniteLine(const C3DVector& start, const C3DVector& end, C3DVector* point, C3DVector* normal) -- first point where the segment start..end enters the sphere (or start itself when the segment starts inside and leaves past end), with the outward unit normal there
// Leaf: GetFirstIntersectionWithFiniteLine
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A58790, 707 bytes, `ret 10h`. Called from the landed
//   CPhysicalPrimitiveSphere::GetFirstIntersectionWithFiniteLine 0x00814D10.
// Callees: C3DVector::Normalise 0x00A14440 (thiscall). Pool: 1e-4f 0x0129BA3C (squared at run time), 4.0f 0x0123088C,
//   0.0f 0x0122DEDC, 1.0f 0x0122DED8, 2.0f 0x0122DCDC.
// Layout: Ego_r.pdb CSphere {float Radius +0, C3DVector Base +4} (16 bytes).
#include <math.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    void Normalise();                                   // 0x00A14440
    float GetLengthSquared() const
    {
        float d = Z * Z;
        d += Y * Y;
        d += X * X;
        return d;
    }
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
__forceinline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
__forceinline C3DVector operator*(const C3DVector& a, float s) { return C3DVector(a.X * s, a.Y * s, a.Z * s); }
__forceinline float Dot(const C3DVector& a, const C3DVector& b)
{
    float d = a.Z * b.Z;
    d += a.Y * b.Y;
    d += a.X * b.X;
    return d;
}
extern const float g_epsilon_0129ba3c;                  // 1e-4f
struct CSphere {
    float     Radius;
    C3DVector Base;
    bool GetFirstIntersectionWithFiniteLine(const C3DVector& start, const C3DVector& end, C3DVector* point, C3DVector* normal) const;
};
bool CSphere::GetFirstIntersectionWithFiniteLine(const C3DVector& start, const C3DVector& end, C3DVector* point, C3DVector* normal) const
{
    C3DVector dir = end - start;
    if (!(dir.GetLengthSquared() > g_epsilon_0129ba3c * g_epsilon_0129ba3c))
        return false;
    float a = dir.GetLengthSquared();
    C3DVector offset = start - Base;
    float b = Dot(dir, offset);
    b *= 2.0f;
    float c = offset.Z * offset.Z;
    c += offset.Y * offset.Y;
    c += offset.X * offset.X;
    float disc = b * b - (c - Radius * Radius) * a * 4.0f;
    if (disc < 0.0f)
        return false;
    float root = sqrtf(disc);
    float t1 = (root - b) / (2.0f * a);
    float t2 = (-b - root) / (2.0f * a);
    float best = 2.0f;
    C3DVector hit;
    if (t1 > 0.0f && t1 < 1.0f && t1 < best) {
        best = t1;
        hit = start + dir * t1;
    }
    if (t2 > 0.0f && t2 < 1.0f && t2 < best) {
        best = t2;
        hit = start + dir * t2;
    }
    if ((t1 < 0.0f && t2 > 1.0f) || (t2 < 0.0f && t1 > 1.0f))
        hit = start;
    else if (!(best > 0.0f && best < 1.0f))
        return false;
    *point = hit;
    *normal = *point - Base;
    normal->Normalise();
    return true;
}
