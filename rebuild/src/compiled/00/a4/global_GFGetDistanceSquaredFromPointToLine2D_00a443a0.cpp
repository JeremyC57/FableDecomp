// GFGetDistanceSquaredFromPointToLine2D(const C2DVector& lineStart, const C2DVector& lineEnd, const C2DVector& point, float* lineParam) -- squared 2D distance from a point to the infinite line through two points; writes the projection parameter along start->end
// Leaf: ?GFGetDistanceSquaredFromPointToLine2D@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A443A0, length 122 bytes (+6 int3 pad), `__fastcall` (start ecx, end edx,
//   point and the out float* on the stack, `ret 8`), float in st(0).
// Callers: 0x006A5777 and 0x006CE0B8, both passing 2-word frame vectors built from the X/Y of C3DVectors (C2DVector).
// Name: descriptive (no PDB name with this signature was found; catalogued `GetClosestPointOnLineSegment_DistanceSquared`;
//   there is no clamp to the segment and no zero-length test, unlike the 3D twin 0x00A442E0).
// Shape: dir = start - end (x87 stack), toStart = start - point (a frame C2DVector); t = (toStart . dir) / (dir . dir) homed
//   in the dead `point` slot; offset = dir * t - toStart (a frame C2DVector); *lineParam = t; return |offset|^2.
class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    C2DVector operator*(float f) const { return C2DVector(X * f, Y * f); }
    float operator*(const C2DVector& o) const { return X * o.X + Y * o.Y; }
    float GetLengthSquared() const { return X * X + Y * Y; }
};

float __fastcall GFGetDistanceSquaredFromPointToLine2D(const C2DVector& lineStart, const C2DVector& lineEnd, const C2DVector& point,
    float* lineParam)
{
    C2DVector dir = lineStart - lineEnd;
    C2DVector toStart = lineStart - point;
    float t = (toStart * dir) / (dir * dir);
    C2DVector offset = dir * t - toStart;
    *lineParam = t;
    return offset.GetLengthSquared();
}