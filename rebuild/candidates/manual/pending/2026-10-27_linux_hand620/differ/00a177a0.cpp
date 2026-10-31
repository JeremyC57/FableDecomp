// CPhysicsMesh::GetPointIntersectionFromToUsingSegmentation(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const -- ask the segmentation for the faces the segment from -> to can touch, then for each face facing along the segment intersect the segment's line with the face plane and, when the hit lies on the segment (within 1e-4) and inside the triangle, keep the hit nearest to `from`; true when anything was hit
// Leaf: ?GetPointIntersectionFromToUsingSegmentation@CPhysicsMesh@@ABE_NABVC3DVector@@0AAVCMeshIntersectionData@@@Z
// Grade: DIFFER(840v840) -- now same length: the two dot products with the direction go through a stepwise Y, Z, X sum
//   (DotDir), giving retail's Y-first order; the only residue is one integer load (mov ecx,[esp+28h], the normal's X
//   copy for the plane) scheduled after the Y-scale fld/fmul instead of before (sec5 retry: 8 grades, 15 -> 8 diff lines
//   incl. relocations)
// Chunk 620 (resume): 6 grades -- CPlane via ctor(normal, d) / ctor(normal, point) / Initialise(normal, point) / Initialise(normal, d) / `v0 * normal` / no user C3DVector copy ctor: all leave the same 4-line residue (or worse); scheduling-only lane gap. PDB names (GetSingleInstanceCollisionSet, GetMagnitudeAndNormalise, std::vector<long>) used here.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A177A0, 840 bytes (+ int3 padding), `ret 0Ch`.
// Callees: CPhysicsMesh::GetSingleInstanceCollisionSet 0x00A6CBB0 (PDB ?GetSingleInstanceCollisionSet@CPhysicsMesh@@ABEAAV?$vector@J...@XZ) (clears and returns the static vector<long> at 0x013D2D1C; manifest
//   "CEngineInternalPrimitiveBase::AddChildPrimitive" is propagated), CIPhysicsFacesSegmentBase::GetCollidedWithTriangles
//   0x00A86EA0, C3DVector::GetMagnitudeAndNormalise 0x00A14480 (landed), CPlane::
//   GetIntersectionWithLine 0x00A422C0, GFIsPointInTriangle 0x00A44420 (__fastcall). The face normal's fast Normalise
//   (mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C) is the landed CTCPhysicsRigidBody::GetRHSet 0x00759B40
//   one. 1e10f immediate, 0.0f 0x0122DEDC, 1e-4f 0x0129BA3C.
// Layout: retail CPhysicsMesh as the landed GetPointIntersectionFromTo 0x00A18760 model (PDB minus 0x1c): Faces start +0x30
//   (6-byte faces, three ushort vertex indices), Vertices start +0x3c, segmentation object +0xb4.
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
};
}
class C3DVector {
public:
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
    float GetMagnitudeAndNormalise();              // 0x00A14480
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
class CPlane {
public:
    C3DVector Normal;
    float     D;
    CPlane(const C3DVector& normal, float d) : Normal(normal), D(d) {}
    bool GetIntersectionWithLine(const C3DVector& point, const C3DVector& direction, C3DVector* result) const;   // 0x00A422C0
};
bool __fastcall GFIsPointInTriangle(const C3DVector& a, const C3DVector& b, const C3DVector& c, const C3DVector& normal, const C3DVector& point);   // 0x00A44420
class CMeshIntersectionData {
public:
    C3DVector IntersectionPosition;                // +0x00
    C3DVector Normal;                              // +0x0c
    float     DistanceSquared;                     // +0x18
};
struct CPhysicsFace { unsigned short V[3]; };
class CPhysicsMesh;
class CIPhysicsFacesSegmentBase {
public:
    void GetCollidedWithTriangles(const CPhysicsMesh* mesh, float radius, const C3DVector& from, const C3DVector& to, std::vector<long>& faces) const;   // 0x00A86EA0
};
extern const float PHYSICS_EPSILON;                 // 0x0129BA3C (1e-4f)
class CPhysicsMesh {
public:
    unsigned char              _pad00[0x30];
    CPhysicsFace*              Faces;              // +0x30 (vector start)
    unsigned char              _pad34[8];
    C3DVector*                 Vertices;           // +0x3c (vector start)
    unsigned char              _pad40[0xb4 - 0x40];
    CIPhysicsFacesSegmentBase* m_pSegmentation;    // +0xb4
private:
    std::vector<long>& GetSingleInstanceCollisionSet() const;   // 0x00A6CBB0
    bool GetPointIntersectionFromToUsingSegmentation(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const;
};

bool CPhysicsMesh::GetPointIntersectionFromToUsingSegmentation(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const
{
    std::vector<long>& faces = GetSingleInstanceCollisionSet();
    m_pSegmentation->GetCollidedWithTriangles(this, 0.0f, from, to, faces);
    float nearest = 1e10f;
    bool found = false;
    C3DVector direction = to - from;
    float length = direction.GetMagnitudeAndNormalise();
    for (long* it = faces.begin(); it != faces.end(); ++it) {
        const CPhysicsFace& face = Faces[*it];
        const C3DVector& v0 = Vertices[face.V[0]];
        const C3DVector& v1 = Vertices[face.V[1]];
        const C3DVector& v2 = Vertices[face.V[2]];
        C3DVector normal = (v1 - v0) ^ (v2 - v0);
        normal.Normalise();
        CPlane plane(normal, normal * v0);
        if (normal.DotDir(direction) > 0.0f) {
            C3DVector hit;
            if (plane.GetIntersectionWithLine(from, direction, &hit)) {
                float t = (hit - from).DotDir(direction);
                if (t > -PHYSICS_EPSILON && t < length + PHYSICS_EPSILON &&
                    GFIsPointInTriangle(v0, v1, v2, -normal, hit)) {
                    float distanceSquared = (hit - from).GetLengthSquared();
                    if (distanceSquared < nearest) {
                        data.DistanceSquared = distanceSquared;
                        nearest = distanceSquared;
                        data.IntersectionPosition = hit;
                        data.Normal = normal;
                        found = true;
                    }
                }
            }
        }
    }
    return found;
}
