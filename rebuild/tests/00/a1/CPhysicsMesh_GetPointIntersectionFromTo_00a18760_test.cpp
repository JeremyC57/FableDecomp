// Behaviour test for 00a18760: src/00a18760.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CPhysicsMesh::GetPointIntersectionFromTo(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
extern unsigned char g_bUsePhysicsMeshSegmentation;   // 0x013984D0

struct CPhysicsMesh {
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

// ---- stubs and driver ----
unsigned char g_bUsePhysicsMeshSegmentation = 0;
static int g_seg = 0, g_full = 0; static bool g_ret = true; static const C3DVector* g_from; static const C3DVector* g_to; static CMeshIntersectionData* g_data;
bool CPhysicsMesh::GetPointIntersectionFromToUsingSegmentation(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const
{ ++g_seg; g_from = &from; g_to = &to; g_data = &data; return g_ret; }
bool CPhysicsMesh::GetPointIntersectionFromToUsingFullCollision(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const
{ ++g_full; g_from = &from; g_to = &to; g_data = &data; return g_ret; }
int main()
{
    static CPhysicsMesh mesh; memset(&mesh, 0, sizeof(mesh));
    CHECK((char*)&mesh.BoundingBox - (char*)&mesh == 0x84 && (char*)&mesh.m_pSegmentation - (char*)&mesh == 0xb4);
    mesh.BoundingBox.MinCoord = C3DVector(0, 0, 0); mesh.BoundingBox.MaxCoord = C3DVector(100, 100, 10);
    static char buf[16]; CMeshIntersectionData& data = *(CMeshIntersectionData*)buf;
    C3DVector from(10, 10, 0), to(20, 20, 0);
    // inside the box: full collision (segmentation disabled)
    CHECK(mesh.GetPointIntersectionFromTo(from, to, data) && g_full == 1 && g_seg == 0 && g_from == &from && g_to == &to && g_data == &data);
    g_ret = false;
    CHECK(!mesh.GetPointIntersectionFromTo(from, to, data) && g_full == 2);
    // segmentation enabled but no segment object: still full collision; with one: segmentation
    g_bUsePhysicsMeshSegmentation = 1; g_ret = true;
    CHECK(mesh.GetPointIntersectionFromTo(from, to, data) && g_full == 3 && g_seg == 0);
    mesh.m_pSegmentation = (CIPhysicsFacesSegmentBase*)buf;
    CHECK(mesh.GetPointIntersectionFromTo(from, to, data) && g_seg == 1);
    // far outside in X (beyond the 50-unit margin): false without either call
    C3DVector farFrom(300, 10, 0), farTo(310, 10, 0);
    CHECK(!mesh.GetPointIntersectionFromTo(farFrom, farTo, data) && g_seg == 1 && g_full == 3);
    // just within the margin: |to.x - from.x| + 50 reaches back to the box
    C3DVector nearFrom(140, 10, 0), nearTo(145, 10, 0);
    CHECK(mesh.GetPointIntersectionFromTo(nearFrom, nearTo, data) && g_seg == 2);
    // below in Y beyond the margin
    C3DVector lowFrom(10, -200, 0), lowTo(10, -190, 0);
    CHECK(!mesh.GetPointIntersectionFromTo(lowFrom, lowTo, data) && g_seg == 2);
    if (fails == 0) printf("00a18760_TEST PASS\n");
    return fails;
}
