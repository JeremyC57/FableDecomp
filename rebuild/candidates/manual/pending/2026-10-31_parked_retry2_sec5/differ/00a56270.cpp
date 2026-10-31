// CMatrix3x4::Orthonormalise() -- rebuild an orthonormal basis from the first two rows (X, Cross(X, Y), Cross(Z, X), each
// Grade: DIFFER(720v693) -- y copied before x.Normalise (218 diff lines v 293); left: Normalise/cross x87 schedule,
//   retail frame copies
//   normalised with the fast inverse square root) and clear the translation row
// Leaf: ?Orthonormalise@CMatrix3x4@@
// Retail: Fable.exe @ 0x00A56270, 693 bytes (+ int3 padding), plain `ret`; a leaf (no calls).
// Data: fast inverse-sqrt mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C.
// Layout: CMatrix3x4 (48 bytes, Ego_r.pdb: a union of float[12] / rows) as four C3DVector rows.
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    __forceinline void Normalise()
    {
        float inv = FastInvSqrt(Z * Z + Y * Y + X * X);
        X *= inv;
        Y *= inv;
        Z *= inv;
    }
};
__forceinline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
struct CMatrix3x4 {
    C3DVector XAxis;                                     // +0x00
    C3DVector YAxis;                                     // +0x0c
    C3DVector ZAxis;                                     // +0x18
    C3DVector Pos;                                       // +0x24
    void Orthonormalise();
};

void CMatrix3x4::Orthonormalise()
{
    C3DVector x = XAxis;
    C3DVector y = YAxis;
    x.Normalise();
    C3DVector z = CrossProduct(x, y);
    z.Normalise();
    y = CrossProduct(z, x);
    y.Normalise();
    Pos = C3DVector(0.0f, 0.0f, 0.0f);
    XAxis = x;
    YAxis = y;
    ZAxis = z;
}
