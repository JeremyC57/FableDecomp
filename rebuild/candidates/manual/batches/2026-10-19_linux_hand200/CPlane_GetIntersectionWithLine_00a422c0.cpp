// CPlane::GetIntersectionWithLine(const C3DVector& point, const C3DVector& direction, C3DVector& intersection) -- intersect the line point + t*direction with the plane; false when the line is parallel to it
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A422C0, 165 bytes, `ret 0Ch`.
// Callees: none (float constant 0.0f at 0x0122DEDC).
// Shape: CPlane = Normal (C3DVector) +0, Distance +0xC (Ego_r.pdb, as the landed CPlane::Initialise 0x00A42140).
//   denominator = Normal . direction; the parallel test is `if (denominator != 0.0f) { ... return true; } return false;`
//   (`fucompp ; test ah,44h ; jnp` to the false exit at the end, which pops the denominator); t = (Distance - Normal . point) / denominator; the result is built as C3DVector temporaries
//   (`direction * t`, then `+ point`) and copied into `intersection` dword by dword (the 0x18-byte frame holds the
//   two spilled scaled components and the sum).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
inline float DotProduct(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
inline float DotProductByValue(C3DVector a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

struct CPlane {
    C3DVector Normal;                              // +0x00
    float     Distance;                            // +0x0c
    bool GetIntersectionWithLine(const C3DVector& point, const C3DVector& direction, C3DVector& intersection) const;
};

bool CPlane::GetIntersectionWithLine(const C3DVector& point, const C3DVector& direction, C3DVector& intersection) const
{
    float denominator = DotProduct(Normal, direction);
    if (denominator != 0.0f) {
        float t = (Distance - DotProductByValue(point, Normal)) / denominator;
        intersection = direction * t + point;
        return true;
    }
    return false;
}
