// CRightHandedSet::MakePerpendicular() -- normalise Forward, remove its component from Up, normalise Up
// Grade: DIFFER(ours v retail) -- 358v358, scheduling only: retail puts the second `mov esi,0x17C` between `fld st(3)` and `fmul st(4)`, ours after the fmul
// Retail 0x00A22E10, 358 bytes, no calls; the table-driven fast 1/sqrt (0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C)
// is C3DVector::Normalise inlined twice (__forceinline: VC otherwise emits it out of line). Dot is a += chain Y, Z, X
// (a single X+Y+Z sum is canonicalised to Z,Y,X); `Forward * d` is a named temporary subtracted with -=.
// Tried: natural dot, Forward.Dot(Up)/Up.Dot(Forward), three sum orders, Up = Up - t, named temp, QFE toolset.
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator-=(const C3DVector& v) { X -= v.X; Y -= v.Y; Z -= v.Z; return *this; }
    float Dot(const C3DVector& v) const { float d = Y * v.Y; d += Z * v.Z; d += X * v.X; return d; }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};

struct CRightHandedSet {
    C3DVector Up;        // +0x00
    C3DVector Forward;   // +0x0C
    void MakePerpendicular();
};

void CRightHandedSet::MakePerpendicular()
{
    Forward.Normalise();
    C3DVector along = Forward * Up.Dot(Forward);
    Up -= along;
    Up.Normalise();
}
