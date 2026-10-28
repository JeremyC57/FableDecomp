// GFIsPointInTriangle(const C3DVector& a, const C3DVector& b, const C3DVector& c, const C3DVector& normal, const C3DVector& p) -- point-in-triangle test by edge-normal side signs
// Grade: DIFFER(414v424) -- the three edge tests dot the cross product in x, z, y order in retail (`fmul nx`, `fmul nz`, `fmul ny` then adds) where every source form tried gives z, y, x; the rest (subtractions, crosses, 0.0001f compares, early returns) lines up
// Leaf: ?GFIsPointInTriangle@@YI_NABVC3DVector@@0000@Z
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
inline float DotProduct(const C3DVector& a, const C3DVector& b)
{
    return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}

bool __fastcall GFIsPointInTriangle(const C3DVector& a, const C3DVector& b, const C3DVector& c, const C3DVector& normal, const C3DVector& p)
{
    C3DVector ab = b - a;
    C3DVector ac = c - a;
    C3DVector bc = c - b;
    C3DVector ap = p - a;
    if (DotProduct(CrossProduct(ab, ap), normal) <= 0.0001f) {
        C3DVector bp = p - b;
        if (DotProduct(CrossProduct(bc, bp), normal) <= 0.0001f) {
            C3DVector cp = p - c;
            if (DotProduct(CrossProduct(cp, ac), normal) <= 0.0001f)
                return true;
        }
    }
    return false;
}
