// Behaviour test for 00a187c0: src/00a187c0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CPhysicsMesh::GetSphereIntersectionFromTo(float radius, const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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

// ---- stubs and driver ----
bool g_PhysicsUseSegmentation = false;
static int g_pseg = 0, g_pfull = 0, g_sseg = 0, g_sfull = 0; static bool g_ret = true; static float g_radius;
static const C3DVector* g_from; static const C3DVector* g_to; static CMeshIntersectionData* g_data;
bool CPhysicsMesh::GetPointIntersectionFromToUsingSegmentation(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const
{ ++g_pseg; g_from = &from; g_to = &to; g_data = &data; return g_ret; }
bool CPhysicsMesh::GetPointIntersectionFromToUsingFullCollision(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const
{ ++g_pfull; g_from = &from; g_to = &to; g_data = &data; return g_ret; }
bool CPhysicsMesh::GetSphereIntersectionFromToUsingSegmentation(float radius, const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const
{ ++g_sseg; g_radius = radius; g_from = &from; g_to = &to; g_data = &data; return g_ret; }
bool CPhysicsMesh::GetSphereIntersectionFromToUsingFullCollision(float radius, const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const
{ ++g_sfull; g_radius = radius; g_from = &from; g_to = &to; g_data = &data; return g_ret; }
int main()
{
    static CPhysicsMesh mesh; memset(&mesh, 0, sizeof(mesh));
    CHECK((char*)&mesh.BoundingBox - (char*)&mesh == 0x84 && (char*)&mesh.RootSegment - (char*)&mesh == 0xb4);
    mesh.BoundingBox.MinCoord = C3DVector(0, 0, 0); mesh.BoundingBox.MaxCoord = C3DVector(100, 100, 10);
    static char buf[16]; CMeshIntersectionData& data = *(CMeshIntersectionData*)buf;
    C3DVector from(10, 10, 0), to(20, 20, 0);
    // tiny radius: the point query (full collision here)
    CHECK(mesh.GetSphereIntersectionFromTo(0.001f, from, to, data) && g_pfull == 1 && g_sfull == 0 && g_data == &data);
    // a real radius: sphere full collision, then segmentation once enabled and built
    CHECK(mesh.GetSphereIntersectionFromTo(2.0f, from, to, data) && g_sfull == 1 && g_radius == 2.0f && g_from == &from && g_to == &to);
    g_PhysicsUseSegmentation = true;
    CHECK(mesh.GetSphereIntersectionFromTo(2.0f, from, to, data) && g_sfull == 2 && g_sseg == 0);
    mesh.RootSegment.Data = (CIPhysicsFacesSegmentBase*)buf;
    g_ret = false;
    CHECK(!mesh.GetSphereIntersectionFromTo(2.0f, from, to, data) && g_sseg == 1);
    g_ret = true;
    CHECK(mesh.GetSphereIntersectionFromTo(0.0f, from, to, data) && g_pseg == 1);     // point query through segmentation
    // box test fails (the radius widens the swept box: 300 - 2 - 60 > 100, but a radius of 200 reaches it)
    C3DVector farFrom(300, 10, 0), farTo(310, 10, 0);
    CHECK(!mesh.GetSphereIntersectionFromTo(2.0f, farFrom, farTo, data) && g_sseg == 1 && g_sfull == 2);
    CHECK(mesh.GetSphereIntersectionFromTo(200.0f, farFrom, farTo, data) && g_sseg == 2);
    if (fails == 0) printf("00a187c0_TEST PASS\n");
    return fails;
}
