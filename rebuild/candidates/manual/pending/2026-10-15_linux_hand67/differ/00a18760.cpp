// CPhysicsMesh::GetPointIntersectionFromTo(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) -- bounding-box pre-test, then the segmentation or the full-collision intersection
// Grade: DIFFER(93v95) -- retail keeps `this` in edx across the pre-test call (`mov edx,ecx` before it, `[edx+0B4h]` / `mov ecx,edx` after: whole-program register knowledge of the callee) and materialises the false result as `xor bl,bl` before the call; the lane keeps `this` in esi and folds `xor al,al` into the exit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A18760, length 95 bytes.
// Identity: Ego_r.pdb `?GetPointIntersectionFromTo@CPhysicsMesh@@QBE_NABVC3DVector@@0AAVCMeshIntersectionData@@@Z`
// (and the callees' mangled names: HasBoundingBoxIntersectedFromTo `QBE_NMABVC3DVector@@0@Z`, the
// two private `ABE_N...` intersection helpers).
// Callees: HasBoundingBoxIntersectedFromTo(float, const C3DVector&, const C3DVector&) @ 0x00A17690,
// GetPointIntersectionFromToUsingSegmentation @ 0x00A177A0, GetPointIntersectionFromToUsingFullCollision
// @ 0x00A17F80 (all __thiscall on this mesh), the global enable byte @ 0x013984D0, the segmentation
// counted pointer's object at +0xB4.
// Shape: `xor bl,bl` is the false result held in a local across the pre-test call; the pre-test's
// float tolerance is the literal 0.0f (`push 0`); the two tail calls keep `call ; ret 0Ch`.
struct C3DVector { float x, y, z; };
struct CMeshIntersectionData;
struct CIPhysicsFacesSegmentBase;
extern unsigned char g_bUsePhysicsMeshSegmentation;   // 0x013984D0

struct CPhysicsMesh {
    unsigned char              _pad00[0xb4];
    CIPhysicsFacesSegmentBase* m_pSegmentation;        // +0xb4 (counted pointer's object)
    bool HasBoundingBoxIntersectedFromTo(float tolerance, const C3DVector& from, const C3DVector& to) const;            // 0x00A17690
    bool GetPointIntersectionFromToUsingSegmentation(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const;   // 0x00A177A0
    bool GetPointIntersectionFromToUsingFullCollision(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const;  // 0x00A17F80
    bool GetPointIntersectionFromTo(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const;
};

bool CPhysicsMesh::GetPointIntersectionFromTo(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const
{
    bool result = false;
    if (HasBoundingBoxIntersectedFromTo(0.0f, from, to)) {
        if (g_bUsePhysicsMeshSegmentation && m_pSegmentation)
            return GetPointIntersectionFromToUsingSegmentation(from, to, data);
        return GetPointIntersectionFromToUsingFullCollision(from, to, data);
    }
    return result;
}
