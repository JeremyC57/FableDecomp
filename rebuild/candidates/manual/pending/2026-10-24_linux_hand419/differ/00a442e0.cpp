// GFGetDistanceSquaredFromPointToLine(const C3DVector& lineStart, const C3DVector& lineEnd, const C3DVector& point, float* lineParam) -- squared distance from a point to the infinite line through two points; writes the projection parameter along start->end (0 when the two points coincide)
// Grade: DIFFER(190v188) -- |line|^2 is stored with fstp and reloaded for the zero test and the divide, where retail keeps a register copy (`fst [esp+10h] ; fld 0 ; fld st(1)`, then `fdivrp st(3)` and one extra fstp st(0) on the zero path); everything else matches
// Leaf: ?GFGetDistanceSquaredFromPointToLine@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A442E0, length 188 bytes (+4 int3 pad), `__fastcall` (start ecx, end edx,
//   point and the out float* on the stack, `ret 8`), float in st(0).
// Callers: 0x006A50CC, 0x006A707F, 0x006A7D6E, 0x006A7D9C (CThingPhysical-area code, each feeding the result to fsqrt),
//   0x00A1765D. Constants: 0.0f 0x0122DEDC.
// Name: descriptive (no PDB name with this signature was found; catalogued `Vector3_ClosestPointOnLineSegment`, but there is
//   no clamp to the segment and the result is a squared distance, not a point).
// Tried: v1 a named `lineLengthSquared` local (also (t*t)*len2 at the end); v2 this (GetLengthSquared() spelled at each use,
//   end product matches); v3 `if (== 0) return 0;` first (return block moves to the fall-through); v4 a __forceinline helper
//   taking the length by value (sum order changes). Next: the same register-copy residue as 0x00A441E0 (same TU); try a
//   `double` local for |line|^2 or the assignment inside the condition `if ((len2 = line * line) != 0.0f)`.
// Shape: line = end - start (kept on the x87 stack), offset = point - start (a frame C3DVector), |line|^2 homed in the dead
//   `point` slot; `if (|line|^2 != 0) { t = (offset . line) / |line|^2; *lineParam = t; return |offset|^2 - |line|^2 * t * t; }
//   return 0;`.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float operator*(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};

float __fastcall GFGetDistanceSquaredFromPointToLine(const C3DVector& lineStart, const C3DVector& lineEnd, const C3DVector& point,
    float* lineParam)
{
    C3DVector line = lineEnd - lineStart;
    C3DVector offset = point - lineStart;
    if (line.GetLengthSquared() != 0.0f) {
        float t = (offset * line) / line.GetLengthSquared();
        *lineParam = t;
        return offset.GetLengthSquared() - line.GetLengthSquared() * t * t;
    }
    return 0.0f;
}
