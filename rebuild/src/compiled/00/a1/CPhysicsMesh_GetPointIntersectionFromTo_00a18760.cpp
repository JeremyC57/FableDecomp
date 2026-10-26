// CPhysicsMesh::GetPointIntersectionFromTo(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) -- bounding-box pre-test, then the segmentation or the full-collision intersection
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A18760, length 95 bytes.
// Identity: Ego_r.pdb `?GetPointIntersectionFromTo@CPhysicsMesh@@QBE_NABVC3DVector@@0AAVCMeshIntersectionData@@@Z`
// (and the callees' mangled names: HasBoundingBoxIntersectedFromTo `QBE_NMABVC3DVector@@0@Z`, the
// two private `ABE_N...` intersection helpers).
// Callees: HasBoundingBoxIntersectedFromTo(float, const C3DVector&, const C3DVector&) @ 0x00A17690,
// GetPointIntersectionFromToUsingSegmentation @ 0x00A177A0, GetPointIntersectionFromToUsingFullCollision
// @ 0x00A17F80 (all __fastcall on this mesh), the global enable byte @ 0x013984D0, the segmentation
// counted pointer's object at +0xB4.
// Shape: `bool result = false;` assigned from either intersection call inside the pre-test's success branch (the false
// result lives in bl across the pre-test call; the two calls stay tail calls); the pre-test's float tolerance is the
// literal 0.0f (`push 0`). Retail keeps `this` in edx across the pre-test call because HasBoundingBoxIntersectedFromTo
// is compiled above the caller in the same TU and never touches edx (VC7.1 callee register-usage knowledge): its real
// body is written out below and reproduces 0x00A17690 byte for byte (centre via an inline GetCentre()).
// Layout: Ego_r.pdb CPhysicsMesh BoundingBox (C3DBoundingBox MinCoord/MaxCoord) sits at +0xA0 and RootSegment at +0xD0;
// retail reads them at +0x84 / +0xB4 (0x1C smaller ahead of them), so the class is modelled locally.
class C3DVector { public:
    float x, y, z;
    C3DVector() {}
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
    C3DVector operator+(const C3DVector& v) const { return C3DVector(x + v.x, y + v.y, z + v.z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(x - v.x, y - v.y, z - v.z); }
    C3DVector operator*(float f) const { return C3DVector(x * f, y * f, z * f); }
};
class C3DBoundingBox { public:
    C3DVector MinCoord, MaxCoord;
    C3DVector GetCentre() const { return (MinCoord + MaxCoord) * 0.5f; }
};
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(fabs)
class CMeshIntersectionData;
class CIPhysicsFacesSegmentBase;
extern unsigned char g_bUsePhysicsMeshSegmentation;   // 0x013984D0

class CPhysicsMesh { public:
    unsigned char              _pad00[0x84];
    C3DBoundingBox             BoundingBox;            // +0x84 (PDB +0xA0; retail's three vector maps are smaller)
    unsigned char              _pad9c[0xb4 - 0x9c];
    CIPhysicsFacesSegmentBase* m_pSegmentation;        // +0xb4 (counted pointer's object)
    bool HasBoundingBoxIntersectedFromTo(float tolerance, const C3DVector& from, const C3DVector& to) const;            // 0x00A17690
    bool GetPointIntersectionFromToUsingSegmentation(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const;   // 0x00A177A0
    bool GetPointIntersectionFromToUsingFullCollision(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const;  // 0x00A17F80
    bool GetPointIntersectionFromTo(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const;
};

// Same-TU body of HasBoundingBoxIntersectedFromTo @ 0x00A17690 (retail's TU, above the caller): it never touches edx,
// so VC7.1 keeps `this` there across the call, as retail does.
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

bool CPhysicsMesh::GetPointIntersectionFromTo(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const
{
    bool result = false;
    if (HasBoundingBoxIntersectedFromTo(0.0f, from, to)) {
        if (g_bUsePhysicsMeshSegmentation && m_pSegmentation)
            result = GetPointIntersectionFromToUsingSegmentation(from, to, data);
        else
            result = GetPointIntersectionFromToUsingFullCollision(from, to, data);
    }
    return result;
}