// GFGetDistanceSquaredFromPointToLine(const C3DVector& lineStart, const C3DVector& lineEnd, const C3DVector& point, float* lineParam) -- squared distance from a point to the infinite line through two points; writes the projection parameter along start->end (0 when the two points coincide)
// Grade: RELOCATION_MATCH (agent902, 3rd grade) -- `if ((len2 = line.GetLengthSquared()) != 0.0f)` keeps |line|^2 on the x87 stack for the zero test and the divide (fst to its home), and the re-spelled line.GetLengthSquared() in the result reloads that home, as retail.
// Leaf: ?GFGetDistanceSquaredFromPointToLine@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A442E0, length 188 bytes (+4 int3 pad), `__fastcall` (start ecx, end edx,
//   point and the out float* on the stack, `ret 8`), float in st(0).
// Callers: 0x006A50CC, 0x006A707F, 0x006A7D6E, 0x006A7D9C (CThingPhysical-area code, each feeding the result to fsqrt),
//   0x00A1765D. Constants: 0.0f 0x0122DEDC.
// Name: descriptive (no PDB name with this signature was found; catalogued `Vector3_ClosestPointOnLineSegment`, but there is
//   no clamp to the segment and the result is a squared distance, not a point).
// Tried (chunk agent): a named len2 local, GetLengthSquared() spelled at each use, an early `return 0`, a by-value helper.
// Shape: line = end - start (kept on the x87 stack), offset = point - start (a frame C3DVector); `float len2; if ((len2 =
//   line.GetLengthSquared()) != 0.0f) { float t = (offset * line) / len2; *lineParam = t; return offset.GetLengthSquared()
//   - line.GetLengthSquared() * t * t; } return 0.0f;` -- the assigned value is compared and divided from the register
//   (`fst [esp+10h] ; fld 0 ; fld st(1)`, `fdivrp st(3)`), homed in the dead `point` slot; the re-spelled length in the
//   result is CSE'd into that home and reloaded (`fld [esp+10h] ; fmul st(2) ; fmul st(2)`).
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
    float len2;
    if ((len2 = line.GetLengthSquared()) != 0.0f) {
        float t = (offset * line) / len2;
        *lineParam = t;
        return offset.GetLengthSquared() - line.GetLengthSquared() * t * t;
    }
    return 0.0f;
}