// CPhysicsMesh::GetPointIntersectionFromToUsingFullCollision(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const -- brute-force twin of the segmentation query: test every face of the mesh; for each face facing along the segment intersect the segment's line with the face plane and, when the hit lies on the segment (within 1e-4) and inside the triangle, keep the hit nearest to `from` (position, face normal, distance squared); true when anything was hit
// Leaf: GetPointIntersectionFromToUsingFullCollision
// Grade: DIFFER(818v818) -- now same length: the twin lever of 0x00A177A0 (stepwise Y, Z, X dot with the direction) plus
//   nearest stored before data.DistanceSquared (fst/fstp, no fld st(0) copy); residue: the /6 add esi,edx one slot
//   early, the plane-copy integer load a slot late, one fld/fmul pair order and the fst/fstp destinations swapped in the
//   distance store (sec5 retry: 4 grades, 822 -> 818 bytes)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A17F80, 818 bytes (+ int3 padding), `ret 0Ch`.
// Callees: C3DVector::Normalise() returning the length 0x00A14480 (manifest "GetScaled"), CPlane::GetIntersectionWithLine
//   0x00A422C0, GFIsPointInTriangle 0x00A44420 (__fastcall). Fast Normalise as the landed CTCPhysicsRigidBody::GetRHSet
//   0x00759B40 (mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C). 1e10f immediate, 0.0f 0x0122DEDC,
//   1e-4f 0x0129BA3C.
// Layout: retail CPhysicsMesh as the landed GetPointIntersectionFromTo 0x00A18760 model (PDB minus 0x1c): Faces vector
//   +0x30/+0x34 (6-byte faces, three ushort vertex indices), Vertices start +0x3c. Twin of 0x00A177A0.
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    C3DVector operator^(const C3DVector& v) const { return C3DVector(Y * v.Z - Z * v.Y, Z * v.X - X * v.Z, X * v.Y - Y * v.X); }
    float operator*(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
    __forceinline float DotDir(const C3DVector& v) const { float d = Y * v.Y; d += Z * v.Z; d += X * v.X; return d; }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    float Normalise();                             // 0x00A14480, returns the length
    __forceinline void FastNormalise()
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
struct CPlane {
    C3DVector Normal;
    float     D;
    CPlane(const C3DVector& normal, float d) : Normal(normal), D(d) {}
    bool GetIntersectionWithLine(const C3DVector& point, const C3DVector& direction, C3DVector& result) const;   // 0x00A422C0
};
bool __fastcall GFIsPointInTriangle(const C3DVector& a, const C3DVector& b, const C3DVector& c, const C3DVector& normal, const C3DVector& point);   // 0x00A44420
struct CMeshIntersectionData {
    C3DVector IntersectionPosition;                // +0x00
    C3DVector Normal;                              // +0x0c
    float     DistanceSquared;                     // +0x18
};
struct CLongVector {                               // STLport vector<long>
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
};
CLongVector& GetScratchFaceList();                 // 0x00A6CBB0
struct CPhysicsFace { unsigned short V[3]; };
class CPhysicsMesh;
struct CIPhysicsFacesSegmentBase {
    void GetCollidedWithTriangles(const CPhysicsMesh* mesh, float radius, const C3DVector& from, const C3DVector& to, CLongVector& faces);   // 0x00A86EA0
};
extern const float PHYSICS_EPSILON;                 // 0x0129BA3C (1e-4f)
class CPhysicsMesh {
public:
    unsigned char              _pad00[0x30];
    CPhysicsFace*              Faces;              // +0x30 (vector start)
    CPhysicsFace*              FacesEnd;           // +0x34 (vector finish)
    unsigned char              _pad38[4];
    C3DVector*                 Vertices;           // +0x3c (vector start)
    unsigned char              _pad40[0xb4 - 0x40];
    CIPhysicsFacesSegmentBase* m_pSegmentation;    // +0xb4
    long GetNoFaces() const { return FacesEnd - Faces; }
    bool GetPointIntersectionFromToUsingFullCollision(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const;
};

bool CPhysicsMesh::GetPointIntersectionFromToUsingFullCollision(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const
{
    long count = GetNoFaces();
    float nearest = 1e10f;
    bool found = false;
    C3DVector direction = to - from;
    float length = direction.Normalise();
    for (long i = 0; i < count; ++i) {
        const CPhysicsFace& face = Faces[i];
        const C3DVector& v0 = Vertices[face.V[0]];
        const C3DVector& v1 = Vertices[face.V[1]];
        const C3DVector& v2 = Vertices[face.V[2]];
        C3DVector normal = (v1 - v0) ^ (v2 - v0);
        normal.FastNormalise();
        CPlane plane(normal, normal * v0);
        if (normal.DotDir(direction) > 0.0f) {
            C3DVector hit;
            if (plane.GetIntersectionWithLine(from, direction, hit)) {
                float t = (hit - from).DotDir(direction);
                if (t > -PHYSICS_EPSILON && t < length + PHYSICS_EPSILON &&
                    GFIsPointInTriangle(v0, v1, v2, -normal, hit)) {
                    float distanceSquared = (hit - from).GetLengthSquared();
                    if (distanceSquared < nearest) {
                        nearest = distanceSquared;
                        data.DistanceSquared = distanceSquared;
                        data.Normal = normal;
                        data.IntersectionPosition = hit;
                        found = true;
                    }
                }
            }
        }
    }
    return found;
}
