// CThingFilter_IsInFOV::operator()(const C3DVector& pos) const -- true when the point is (almost) at the eye or within the cone: the normalised offset dotted with Direction beats CosFOV (Z ignored unless AngleTest3D)
// Leaf: ??RCThingFilter_IsInFOV@@QBE_NABVC3DVector
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A7270, 260 bytes (+12 int3), `ret 4`, no calls (the landed thing overload
//   0x006A7390 forwards here).
// Globals: 0.01f 0x01230A5C (the "at the eye" squared distance), the 128-entry reciprocal-square-root mantissa table
//   0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C.
// Layout: PDB CThingFilter_IsInFOV { C3DVector FromPos +0; C3DVector Direction +0xC; float CosFOV +0x18; float FOV +0x1C;
//   bool AngleTest3D +0x20; } (no engine header).
// Shape: the offset is a frame local; the squared length is taken twice and VC7.1 keeps the X and Y squares on the x87
//   stack between the two sums. The near-eye test is the plain left-to-right GetLengthSquared(); the normalisation is a
//   forced-inline FastNormalise(v) whose own sum is written `Y*Y + (X*X + Z*Z)` (so Z*Z pairs with X*X first, as in retail,
//   while the Y square is loaded first) and which scales the offset IN PLACE with `*=` (a by-value `offset * r` temporary
//   schedules the Z product before the Y one). The reciprocal length is the table-driven fast 1/sqrt with one Newton step
//   (its bit pattern built in the dead argument slot).
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

class C3DVector { public:
    float X;    // +0x00
    float Y;    // +0x04
    float Z;    // +0x08
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
inline C3DVector& operator*=(C3DVector& a, float s) { a.X *= s; a.Y *= s; a.Z *= s; return a; }
inline float operator*(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}

class CThingFilter_IsInFOV { public:
    C3DVector FromPos;                              // +0x00
    C3DVector Direction;                            // +0x0c
    float     CosFOV;                               // +0x18
    float     FOV;                                  // +0x1c
    bool      AngleTest3D;                          // +0x20
    bool operator()(const C3DVector& pos) const;
};

__forceinline void FastNormalise(C3DVector& v)
{
    v *= FastInvSqrt(v.Y * v.Y + (v.X * v.X + v.Z * v.Z));
}

bool CThingFilter_IsInFOV::operator()(const C3DVector& pos) const
{
    C3DVector offset = pos - FromPos;
    if (offset.GetLengthSquared() < 0.01f)
        return true;
    if (!AngleTest3D)
        offset.Z = 0.0f;
    FastNormalise(offset);
    if (offset * Direction > CosFOV)
        return true;
    return false;
}