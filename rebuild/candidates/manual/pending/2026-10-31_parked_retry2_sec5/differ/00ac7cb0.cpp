// QuadricOptimiserInternals::COptimisedPrimitive::GetFaceNormal(const COptimisedFace& face) const -- unit normal of a face: (p2 - p0) x (p1 - p0), normalised with the table-driven reciprocal square root unless it is (nearly) zero
// Leaf: GetFaceNormal
// Grade: DIFFER(346v350) -- the squares named once (zz, xx, yy) and re-used: the threshold sum, then a stepwise
//   lengthSquared (xx, += yy, += zz), as retail's compute-once / re-add after the fcomp guard; residue: the second/third
//   cross terms load the other operand first (VC7.1 value-numbering order, source operand swaps inert) and the final
//   scale fxch/fmul st(n) form
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC7CB0, length 348 bytes (padded to 352), `ret 8` (hidden C3DVector result, face).
// Ego_r.pdb: `?GetFaceNormal@COptimisedPrimitive@QuadricOptimiserInternals@@ABE?AVC3DVector@@ABVCOptimisedFace@2@@Z`.
// Globals: epsilon 0.0001f @ 0x0129BA3C, the reciprocal-square-root mantissa table @ 0x013CE558, 3.0f @ 0x0122DED4, 0.5f @ 0x0122F59C.
// Layout: as the landed PerformContraction 0x00ACD840: Vertices (12-byte retail CArray) at +0x10 of 0xb8-byte COptimisedVertex
//   records (Position +0); engine/COptimisedPrimitive.h is the PDB layout with 16-byte containers (Vertices +0x14), so a local
//   model is used. COptimisedFace VertexIndices[3] at +0.
// Shape: the guard is `length2 > eps * eps` (both squares of the constant kept), the normalise inline recomputes the sum; the
//   estimate is built from the exponent and the table mantissa through the dead face slot, one Newton step (3 - r*r*l2) * r * 0.5.
extern unsigned int g_InvSqrtMantissaTable[128];   // 0x013CE558
extern float g_NormaliseEpsilon;                    // 0x0129BA3C (0.0001f)

struct C3DVector {
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

template <class T> struct CArray {                 // retail CArray (STLport vector): first / last / end
    T* m_first;
    T* m_last;
    T* m_end;
    T& operator[](unsigned long n) const { return m_first[n]; }
};

namespace QuadricOptimiserInternals {

struct COptimisedVertex {                          // retail 0xb8 bytes
    C3DVector     Position;                        // +0x00
    unsigned char _rest[0xb8 - 0x0c];
};

struct COptimisedFace {
    unsigned long VertexIndices[3];                // +0x00
};

class COptimisedPrimitive {                        // retail layout: STLport containers are 12 bytes
public:
    long                     MaterialIndex;        // +0x00
    unsigned char            UniqueVertices[0xc];  // +0x04
    CArray<COptimisedVertex> Vertices;             // +0x10
    C3DVector GetFaceNormal(const COptimisedFace& face) const;
};

C3DVector COptimisedPrimitive::GetFaceNormal(const COptimisedFace& face) const
{
    const C3DVector& p0 = Vertices[face.VertexIndices[0]].Position;
    const C3DVector& p1 = Vertices[face.VertexIndices[1]].Position;
    const C3DVector& p2 = Vertices[face.VertexIndices[2]].Position;
    C3DVector n = CrossProduct(p2 - p0, p1 - p0);
    float zz = n.Z * n.Z, xx = n.X * n.X, yy = n.Y * n.Y;
    if (xx + yy + zz > g_NormaliseEpsilon * g_NormaliseEpsilon) {
        float lengthSquared = xx;
        lengthSquared += yy;
        lengthSquared += zz;
        unsigned int bits = *(unsigned int*)&lengthSquared;
        unsigned int estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) | (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        n.X *= r;
        n.Y *= r;
        n.Z *= r;
    }
    return n;
}

}
