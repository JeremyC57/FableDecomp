// COptimisedPrimitive::FindTextureTangent(const C3DVector&, const C3DVector&, const C3DVector&, float, float, float) -- the texture-space tangent of a triangle for one texture coordinate: interpolate along the min..max coordinate edge to the middle vertex's coordinate, take the in-plane direction perpendicular to that iso-line, and scale it so it spans one coordinate unit
// Grade: DIFFER(1075v1091) -- index selection, epsilon early return, interpolation, three fast Normalises match; left: x87 scheduling (retail spills the first Newton factor and the final scale to the dead t1 slot [esp+0x6c], keeps a dead duplicate in the third cross component, and loads all three scaled components before storing the result)
// Leaf: FindTextureTangent
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC7300, 1091 bytes (+ int3 padding), `ret 0x1c` (hidden C3DVector result + 6 args); `this` unused.
// Callees: none. Globals: tangent epsilon 0x0129BA3C, 1.0f 0x0122DED8, 3.0f 0x0122DED4, 0.5f 0x0122F59C, the reciprocal
//   square root mantissa table 0x013CE558 (the engine's inline fast Normalise, as in the landed GetRHSet 0x00759B40).
// Caller: the landed COptimisedPrimitive::FindFaceTextureTangents 0x00AC7E10 (U then V).
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
extern const float g_fTangentEpsilon;               // 0x0129BA3C

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    __forceinline void Normalise()
    {
        float lengthSquared = Z * Z;
        lengthSquared += Y * Y;
        lengthSquared += X * X;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r;
        r *= 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
inline float DotProduct(const C3DVector& a, const C3DVector& b)
{
    return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}

class COptimisedPrimitive {
public:
    C3DVector FindTextureTangent(const C3DVector& p0, const C3DVector& p1, const C3DVector& p2,
                                 float t0, float t1, float t2);
};

C3DVector COptimisedPrimitive::FindTextureTangent(const C3DVector& p0, const C3DVector& p1, const C3DVector& p2,
                                                  float t0, float t1, float t2)
{
    C3DVector points[3] = { p0, p1, p2 };
    float coords[3] = { t0, t1, t2 };
    unsigned long maxIndex = (t0 > t1 && t0 > t2) ? 0 : (t1 > t2 ? 1 : 2);
    unsigned long minIndex = (t0 < t1 && t0 < t2) ? 0 : (t1 < t2 ? 1 : 2);
    unsigned long midIndex = 0;
    if (coords[minIndex] + g_fTangentEpsilon >= coords[maxIndex])
        return C3DVector(0.0f, 0.0f, 0.0f);
    {
        for (unsigned long i = 0; i < 3; ++i) {
            if (i != maxIndex && i != minIndex) {
                midIndex = i;
                break;
            }
        }
        float minCoord = coords[minIndex];
        float range = coords[maxIndex] - minCoord;
        float frac = (coords[midIndex] - minCoord) / range;
        const C3DVector& minPoint = points[minIndex];
        const C3DVector& maxPoint = points[maxIndex];
        const C3DVector& midPoint = points[midIndex];
        C3DVector isoDir = maxPoint * frac + minPoint * (1.0f - frac) - midPoint;
        isoDir.Normalise();
        C3DVector normal = CrossProduct(p2 - p0, p1 - p0);
        normal.Normalise();
        C3DVector tangent = CrossProduct(isoDir, normal);
        tangent.Normalise();
        float scale = range / DotProduct(maxPoint - minPoint, tangent);
        return tangent * scale;
    }
}
