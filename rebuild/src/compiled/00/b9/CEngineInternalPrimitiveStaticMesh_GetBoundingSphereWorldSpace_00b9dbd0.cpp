// CEngineInternalPrimitiveStaticMesh::GetBoundingSphereWorldSpace(C3DBoundingSphere&) -- the mesh's local bounding sphere (record at +0x50, sphere at +0x28) moved into world space: centre through the object transform at +0x70, radius times the scale at +0x58; always true
// Retry 10b: the radius is widened into a double local inside the inline GetRadius (`double radius = ...; *r = (float)(radius * m_scale);`),
// which loads the radius first (`fld [edx+34h] ; fmul [ecx+58h]`) as retail; a float product loads the scale first.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B9DBD0, length 130 bytes; byte-identical to 0x00B9E4E0 (CEngineInternalPrimitive2DStaticMesh). No callees.
// Shape: the centre is read through a reference (`add eax,28h`) into three float locals kept on the x87 stack; each row is
// `x*m[i] + y*m[i+3] + z*m[i+6] + m[i+9]`. Retail evaluates the X row z,y,x but the Y and Z rows z,x,y: the parenthesised
// `y*m + (x*m + z*m) + t` form reproduces that (plain sums in any term order are canonicalised to z,y,x). The radius tail
// needs the double local (float products in either order, float locals and a float-returning helper load the scale first).
class C3DVector { public: float X, Y, Z; };
class C3DBoundingSphere { public: C3DVector Center; float Radius; };
class CMatrix3x4 { public: float m[12]; };
struct C3DMeshRecord {
    unsigned char _pad[0x28];
    C3DBoundingSphere m_sphere;   // +0x28
};
class CEngineInternalPrimitiveStaticMesh { public:
    unsigned char _pad[0x50];
    C3DMeshRecord* m_pMesh;       // +0x50
    unsigned char _pad54[4];
    float m_scale;                // +0x58
    unsigned char _pad5c[0x14];
    CMatrix3x4 m_transform;       // +0x70
    void GetRadius(float* r) { double radius = m_pMesh->m_sphere.Radius; *r = (float)(radius * m_scale); }
    bool GetBoundingSphereWorldSpace(C3DBoundingSphere& out);
};
bool CEngineInternalPrimitiveStaticMesh::GetBoundingSphereWorldSpace(C3DBoundingSphere& out)
{
    const C3DVector& c = m_pMesh->m_sphere.Center;
    float x = c.X, y = c.Y, z = c.Z;
    out.Center.X = x * m_transform.m[0] + y * m_transform.m[3] + z * m_transform.m[6] + m_transform.m[9];
    out.Center.Y = y * m_transform.m[4] + (x * m_transform.m[1] + z * m_transform.m[7]) + m_transform.m[10];
    out.Center.Z = y * m_transform.m[5] + (x * m_transform.m[2] + z * m_transform.m[8]) + m_transform.m[11];
    GetRadius(&out.Radius);
    return true;
}