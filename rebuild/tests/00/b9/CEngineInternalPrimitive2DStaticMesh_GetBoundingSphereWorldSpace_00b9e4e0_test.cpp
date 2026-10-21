// Behaviour test for 00b9e4e0: src/00b9e4e0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CEngineInternalPrimitive2DStaticMesh::GetBoundingSphereWorldSpace(C3DBoundingSphere&)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CEngineInternalPrimitive2DStaticMesh::GetBoundingSphereWorldSpace(C3DBoundingSphere&) -- the mesh's local bounding sphere (record at +0x50, sphere at +0x28) moved into world space: centre through the object transform at +0x70, radius times the scale at +0x58; always true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B9E4E0, length 130 bytes; byte-identical to 0x00B9DBD0 (CEngineInternalPrimitiveStaticMesh). No callees.
// Shape: the centre is read through a reference (`add eax,28h`) into three float locals kept on the x87 stack; each row is
// `x*m[i] + y*m[i+3] + z*m[i+6] + m[i+9]`. Retail evaluates the X row z,y,x but the Y and Z rows z,x,y: the parenthesised
// `y*m + (x*m + z*m) + t` form reproduces that (plain sums in any term order are canonicalised to z,y,x). The radius tail
// goes through a double local in the inline GetRadius (`double radius = R; *r = (float)(radius * scale);`, the landed
// twin 0x00B9DBD0 form), which loads the radius first; `R * scale` in any spelling loads the scale first.
struct C3DVector { float X, Y, Z; };
struct C3DBoundingSphere { C3DVector Center; float Radius; };
struct CMatrix3x4 { float m[12]; };
struct C3DMeshRecord {
    unsigned char _pad[0x28];
    C3DBoundingSphere m_sphere;   // +0x28
};
struct CEngineInternalPrimitive2DStaticMesh {
    unsigned char _pad[0x50];
    C3DMeshRecord* m_pMesh;       // +0x50
    unsigned char _pad54[4];
    float m_scale;                // +0x58
    unsigned char _pad5c[0x14];
    CMatrix3x4 m_transform;       // +0x70
    void GetRadius(float* r) { double radius = m_pMesh->m_sphere.Radius; *r = (float)(radius * m_scale); }
    bool GetBoundingSphereWorldSpace(C3DBoundingSphere& out);
};
bool CEngineInternalPrimitive2DStaticMesh::GetBoundingSphereWorldSpace(C3DBoundingSphere& out)
{
    const C3DVector& c = m_pMesh->m_sphere.Center;
    float x = c.X, y = c.Y, z = c.Z;
    out.Center.X = x * m_transform.m[0] + y * m_transform.m[3] + z * m_transform.m[6] + m_transform.m[9];
    out.Center.Y = y * m_transform.m[4] + (x * m_transform.m[1] + z * m_transform.m[7]) + m_transform.m[10];
    out.Center.Z = y * m_transform.m[5] + (x * m_transform.m[2] + z * m_transform.m[8]) + m_transform.m[11];
    GetRadius(&out.Radius);
    return true;
}

// ---- stubs and driver ----
int main()
{
    static double mb[(sizeof(CEngineInternalPrimitive2DStaticMesh) + 7) / 8];
    CEngineInternalPrimitive2DStaticMesh& p = *(CEngineInternalPrimitive2DStaticMesh*)mb; memset(mb, 0, sizeof(mb));
    static double rb[(sizeof(C3DMeshRecord) + 7) / 8]; C3DMeshRecord& rec = *(C3DMeshRecord*)rb; memset(rb, 0, sizeof(rb));
    p.m_pMesh = &rec; p.m_scale = 2.0f;
    rec.m_sphere.Center.X = 1.0f; rec.m_sphere.Center.Y = 2.0f; rec.m_sphere.Center.Z = 3.0f; rec.m_sphere.Radius = 1.5f;
    float m[12] = { 1, 0, 0,  0, 1, 0,  0, 0, 1,  10, 20, 30 };
    for (int i = 0; i < 12; ++i) p.m_transform.m[i] = m[i];
    C3DBoundingSphere out;
    CHECK(p.GetBoundingSphereWorldSpace(out) == true);
    CHECK(out.Center.X == 11.0f && out.Center.Y == 22.0f && out.Center.Z == 33.0f);
    CHECK(out.Radius == 3.0f);
    float m2[12] = { 0, 1, 0,  -1, 0, 0,  0, 0, 2,  0, 0, 0 };
    for (int i = 0; i < 12; ++i) p.m_transform.m[i] = m2[i];
    p.GetBoundingSphereWorldSpace(out);
    CHECK(out.Center.X == -2.0f && out.Center.Y == 1.0f && out.Center.Z == 6.0f);
    if (fails == 0) printf("00b9e4e0_TEST PASS\n"); else printf("00b9e4e0_TEST FAIL\n");
    return fails;
}
