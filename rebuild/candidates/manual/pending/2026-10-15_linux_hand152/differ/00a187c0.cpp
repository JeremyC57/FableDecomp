// CPhysicsMesh::GetSphereIntersectionFromTo(float radius, const C3DVector& from, const C3DVector& to, CMeshIntersectionData* data) -- swept-sphere query: a tiny radius degenerates to the point query; otherwise, when the swept box touches the mesh bounds, use the segmentation tree (if enabled and built) or the full collision test
// Grade: DIFFER(154v145) -- retail keeps `this` in edx across all four member calls (no esi save), which VC7.1 only does when the callees (0x00A18760, 0x00A17690, 0x00A17AF0, 0x00A182C0) are compiled EARLIER in the same TU and leave edx alone; a probe defining four edx-preserving stub bodies above this one under `#pragma auto_inline(off)` gives RELOCATION_MATCH, but stubs are not reconstruction -- next: author the real callee bodies in this TU (or accept the stub-prefixed TU)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A187C0, length 143 bytes.
// Leaf: GetSphereIntersectionFromTo
// Callees: CPhysicsMesh::GetPointIntersectionFromTo @ 0x00A18760, HasBoundingBoxIntersectedFromTo @ 0x00A17690,
// GetSphereIntersectionFromToUsingSegmentation @ 0x00A17AF0, GetSphereIntersectionFromToUsingFullCollision @ 0x00A182C0.
// Global: the use-segmentation switch byte @ 0x013984D0. Constant: 0.01f @ 0x01230A5C.
// Layout: Ego_r.pdb CPhysicsMesh with the retail containers: RootSegment (CCountedPointer) +0xb4 (PDB +0xd0).
// Shape: `this` parked in edx (no callee-saved push on the early point-query path, which forwards from/to/data and
// returns its result); the result byte is zeroed in bl before the box test and receives either query's al; `ret 10h`.
struct C3DVector { float x, y, z; };
struct CMeshIntersectionData;
struct CIPhysicsFacesSegmentBase;
template <class T> struct CCountedPointer {
    T*    Data;
    void* Count;
};

extern bool g_PhysicsUseSegmentation;              // 0x013984D0

struct CPhysicsMesh {
    unsigned char                              _pad00[0xb4];
    CCountedPointer<CIPhysicsFacesSegmentBase> RootSegment;   // +0xb4
    bool GetPointIntersectionFromTo(const C3DVector& from, const C3DVector& to, CMeshIntersectionData* data) const;   // 0x00A18760
    bool HasBoundingBoxIntersectedFromTo(float radius, const C3DVector& from, const C3DVector& to) const;            // 0x00A17690
    bool GetSphereIntersectionFromToUsingSegmentation(float radius, const C3DVector& from, const C3DVector& to, CMeshIntersectionData* data) const;    // 0x00A17AF0
    bool GetSphereIntersectionFromToUsingFullCollision(float radius, const C3DVector& from, const C3DVector& to, CMeshIntersectionData* data) const;   // 0x00A182C0
    bool GetSphereIntersectionFromTo(float radius, const C3DVector& from, const C3DVector& to, CMeshIntersectionData* data) const;
};

bool CPhysicsMesh::GetSphereIntersectionFromTo(float radius, const C3DVector& from, const C3DVector& to, CMeshIntersectionData* data) const
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
