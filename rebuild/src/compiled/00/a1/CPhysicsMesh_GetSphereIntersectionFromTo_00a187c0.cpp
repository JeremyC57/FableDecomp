// CPhysicsMesh::GetSphereIntersectionFromTo(float radius, const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) -- swept-sphere query: a tiny radius degenerates to the point query; otherwise, when the swept box touches the mesh bounds, use the segmentation tree (if enabled and built) or the full collision test
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A187C0, length 143 bytes.
// Leaf: GetSphereIntersectionFromTo
// Callees: CPhysicsMesh::GetPointIntersectionFromTo @ 0x00A18760, HasBoundingBoxIntersectedFromTo @ 0x00A17690,
// GetSphereIntersectionFromToUsingSegmentation @ 0x00A17AF0, GetSphereIntersectionFromToUsingFullCollision @ 0x00A182C0.
// Global: the use-segmentation switch byte @ 0x013984D0. Constant: 0.01f @ 0x01230A5C.
// Layout: Ego_r.pdb CPhysicsMesh with the retail containers: BoundingBox +0x84 (PDB +0xa0), RootSegment (CCountedPointer)
// +0xb4 (PDB +0xd0). Data is a reference (Ego_r `...0AAVCMeshIntersectionData@@@Z`).
// Shape: `this` parked in edx (no callee-saved push on the early point-query path, which forwards from/to/data and
// returns its result); the result byte is zeroed in bl before the box test and receives either query's al; `ret 10h`.
// `this` stays in edx across the box test because its body (and the point query's) is compiled above this function
// in the same TU and never touches edx (VC7.1 callee register-usage knowledge).
struct C3DVector {
    float x, y, z;
    C3DVector() {}
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
    C3DVector operator+(const C3DVector& v) const { return C3DVector(x + v.x, y + v.y, z + v.z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(x - v.x, y - v.y, z - v.z); }
    C3DVector operator*(float f) const { return C3DVector(x * f, y * f, z * f); }
};
struct C3DBoundingBox {
    C3DVector MinCoord, MaxCoord;
    C3DVector GetCentre() const { return (MinCoord + MaxCoord) * 0.5f; }
};
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(fabs)
struct CMeshIntersectionData;
struct CIPhysicsFacesSegmentBase;
template <class T> struct CCountedPointer {
    T*    Data;
    void* Count;
};

extern bool g_PhysicsUseSegmentation;              // 0x013984D0

struct CPhysicsMesh {
    unsigned char                              _pad00[0x84];
    C3DBoundingBox                             BoundingBox;   // +0x84 (PDB +0xA0)
    unsigned char                              _pad9c[0xb4 - 0x9c];
    CCountedPointer<CIPhysicsFacesSegmentBase> RootSegment;   // +0xb4 (PDB +0xD0)
    bool HasBoundingBoxIntersectedFromTo(float radius, const C3DVector& from, const C3DVector& to) const;            // 0x00A17690
    bool GetPointIntersectionFromToUsingSegmentation(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const;   // 0x00A177A0
    bool GetPointIntersectionFromToUsingFullCollision(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const;  // 0x00A17F80
    bool GetPointIntersectionFromTo(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const;   // 0x00A18760
    bool GetSphereIntersectionFromToUsingSegmentation(float radius, const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const;    // 0x00A17AF0
    bool GetSphereIntersectionFromToUsingFullCollision(float radius, const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const;   // 0x00A182C0
    bool GetSphereIntersectionFromTo(float radius, const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const;
};

// Same-TU bodies of HasBoundingBoxIntersectedFromTo @ 0x00A17690 and GetPointIntersectionFromTo @ 0x00A18760 (retail's
// TU, compiled above this function; both reproduce retail byte for byte): neither touches edx, so VC7.1 keeps `this`
// there across the calls, as retail does.
__declspec(noinline) bool CPhysicsMesh::HasBoundingBoxIntersectedFromTo(float tolerance, const C3DVector& from, const C3DVector& to) const
{
    C3DVector centre = BoundingBox.GetCentre();
    C3DVector half = (BoundingBox.MaxCoord - BoundingBox.MinCoord) * 0.5f;
    C3DVector delta = to - from;
    float dx = (float)fabs(delta.x) + 50.0f;
    float dy = (float)fabs(delta.y) + 50.0f;
    if (from.x - tolerance - dx < centre.x + half.x &&
        from.y - tolerance - dy < centre.y + half.y &&
        dx + from.x + tolerance > centre.x - half.x &&
        dy + from.y + tolerance > centre.y - half.y)
        return true;
    return false;
}

__declspec(noinline) bool CPhysicsMesh::GetPointIntersectionFromTo(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const
{
    bool result = false;
    if (HasBoundingBoxIntersectedFromTo(0.0f, from, to)) {
        if (g_PhysicsUseSegmentation && RootSegment.Data)
            result = GetPointIntersectionFromToUsingSegmentation(from, to, data);
        else
            result = GetPointIntersectionFromToUsingFullCollision(from, to, data);
    }
    return result;
}

bool CPhysicsMesh::GetSphereIntersectionFromTo(float radius, const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const
{
    if (radius < 0.01f)
        return GetPointIntersectionFromTo(from, to, data);
    bool result = false;
    if (HasBoundingBoxIntersectedFromTo(radius, from, to)) {
        if (g_PhysicsUseSegmentation && RootSegment.Data)
            result = GetSphereIntersectionFromToUsingSegmentation(radius, from, to, data);
        else
            result = GetSphereIntersectionFromToUsingFullCollision(radius, from, to, data);
    }
    return result;
}