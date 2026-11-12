// Behaviour test for CPhysicsMeshObject::IsInContainmentVolume (retail 0x00A128E0): a copy of the model (affine inverse, point
// transform, radius scaled by OO_Scale, counted mesh copy held for the query and released afterwards).
#include <cstdio>
#include <cmath>
class C3DVector { public: float X, Y, Z; };
class CMatrix3x4 { public:
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    void SetInverse(const CMatrix3x4& m) {
        float det = m.E11 * (m.E22 * m.E33 - m.E23 * m.E32) + m.E21 * (m.E32 * m.E13 - m.E33 * m.E12) + m.E31 * (m.E23 * m.E12 - m.E22 * m.E13);
        float inv = 1.0f / det;
        E11 = (m.E22 * m.E33 - m.E23 * m.E32) * inv;
        E21 = (m.E23 * m.E31 - m.E21 * m.E33) * inv;
        E31 = (m.E21 * m.E32 - m.E22 * m.E31) * inv;
        E12 = (m.E32 * m.E13 - m.E33 * m.E12) * inv;
        E22 = (m.E33 * m.E11 - m.E13 * m.E31) * inv;
        E32 = (m.E31 * m.E12 - m.E32 * m.E11) * inv;
        E13 = (m.E23 * m.E12 - m.E22 * m.E13) * inv;
        E23 = (m.E21 * m.E13 - m.E23 * m.E11) * inv;
        E33 = (m.E22 * m.E11 - m.E21 * m.E12) * inv;
        E41 = -(E11 * m.E41 + E21 * m.E42 + E31 * m.E43);
        E42 = -(E12 * m.E41 + E22 * m.E42 + E32 * m.E43);
        E43 = -(E13 * m.E41 + E23 * m.E42 + E33 * m.E43);
    }
    void TransformPoint(const C3DVector& s, C3DVector& d) const {
        d.X = E11 * s.X + E21 * s.Y + E31 * s.Z + E41;
        d.Y = E12 * s.X + E22 * s.Y + E32 * s.Z + E42;
        d.Z = E13 * s.X + E23 * s.Y + E33 * s.Z + E43;
    }
};
class CCharString { public: const char* Text; };
static int g_deleted = 0;
static int g_refAtCall = 0;
static const CCharString* g_name = 0;
static C3DVector g_local;
static float g_radius;
class CPhysicsMesh { public:
    long IVRefCount;
    C3DVector VolumeCentre;     // a spherical containment volume in mesh space
    float VolumeRadius;
    void Delete() const { ++g_deleted; }
    bool IsInContainmentVolume(const CCharString& name, const C3DVector& position, float radius) const {
        g_refAtCall = IVRefCount; g_name = &name; g_local = position; g_radius = radius;
        float dx = position.X - VolumeCentre.X, dy = position.Y - VolumeCentre.Y, dz = position.Z - VolumeCentre.Z;
        return std::sqrt(dx * dx + dy * dy + dz * dz) + radius <= VolumeRadius;
    }
};
struct CCountedMesh {
    const CPhysicsMesh* Object;
    CCountedMesh(const CCountedMesh& o) : Object(o.Object) { if (Object) ++const_cast<CPhysicsMesh*>(Object)->IVRefCount; }
    explicit CCountedMesh(const CPhysicsMesh* p) : Object(p) {}
    ~CCountedMesh() { if (Object && --const_cast<CPhysicsMesh*>(Object)->IVRefCount == 0) Object->Delete(); }
    const CPhysicsMesh* operator->() const { return Object; }
};
class CPhysicsMeshObject { public:
    CCountedMesh Mesh;
    CMatrix3x4 PositionMatrix;
    float Scale;
    float OO_Scale;
    CPhysicsMeshObject(const CPhysicsMesh* m) : Mesh(m) {}
    bool IsInContainmentVolume(const CCharString& name, const C3DVector& pos, float radius) const {
        CMatrix3x4 inverse;
        inverse.SetInverse(PositionMatrix);
        C3DVector local;
        inverse.TransformPoint(pos, local);
        float scaled_radius = radius * OO_Scale;
        CCountedMesh mesh(Mesh);
        return mesh->IsInContainmentVolume(name, local, scaled_radius);
    }
};
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    bool ok = true;
    CPhysicsMesh mesh;
    mesh.IVRefCount = 1;
    mesh.VolumeCentre.X = 1.0f; mesh.VolumeCentre.Y = 2.0f; mesh.VolumeCentre.Z = 3.0f;
    mesh.VolumeRadius = 2.0f;
    {
        CPhysicsMeshObject object(&mesh);
        // Rotation of 90 degrees about Z, uniform scale 2, translation (10, 20, 30): world = local * M.
        CMatrix3x4& m = object.PositionMatrix;
        m.E11 = 0.0f;  m.E12 = 2.0f; m.E13 = 0.0f;
        m.E21 = -2.0f; m.E22 = 0.0f; m.E23 = 0.0f;
        m.E31 = 0.0f;  m.E32 = 0.0f; m.E33 = 2.0f;
        m.E41 = 10.0f; m.E42 = 20.0f; m.E43 = 30.0f;
        object.Scale = 2.0f;
        object.OO_Scale = 0.5f;
        CCharString name; name.Text = "VOLUME";
        // The world image of the volume's centre: (1,2,3) * M = (-4+10, 2+20, 6+30).
        C3DVector world; world.X = 6.0f; world.Y = 22.0f; world.Z = 36.0f;
        bool inside = object.IsInContainmentVolume(name, world, 2.0f);
        if (!inside) ok = false;                                   // radius 2 -> 1 in mesh space, centre on the volume centre
        if (!Near(g_local.X, 1.0f) || !Near(g_local.Y, 2.0f) || !Near(g_local.Z, 3.0f)) ok = false;
        if (!Near(g_radius, 1.0f)) ok = false;                     // radius * OO_Scale
        if (g_name != &name) ok = false;
        if (g_refAtCall != 2) ok = false;                          // the counted copy is held across the query
        if (mesh.IVRefCount != 1 || g_deleted != 0) ok = false;    // and released afterwards, without deleting
        if (object.IsInContainmentVolume(name, world, 6.0f)) ok = false;   // 3 in mesh space > volume radius 2
        C3DVector away; away.X = 6.0f; away.Y = 22.0f; away.Z = 40.0f;      // mesh-space (1,2,5): 2 from the centre
        if (object.IsInContainmentVolume(name, away, 2.0f)) ok = false;    // 2 + 1 > 2
        if (!Near(g_local.Z, 5.0f)) ok = false;
        if (!object.IsInContainmentVolume(name, away, 0.0f)) ok = false;   // a point on the volume's surface counts as inside
        if (mesh.IVRefCount != 1 || g_deleted != 0) ok = false;
        // When the object's own reference is the last one and has already been dropped to zero, the copy's release deletes.
        mesh.IVRefCount = 0;
        object.IsInContainmentVolume(name, world, 2.0f);
        if (g_deleted != 1 || mesh.IVRefCount != 0) ok = false;
        mesh.IVRefCount = 2;                                       // keep the object's own destructor from deleting again
    }
    if (mesh.IVRefCount != 1 || g_deleted != 1) ok = false;
    if (!ok) { std::printf("00a128e0_TEST FAIL\n"); return 1; }
    std::printf("00a128e0_TEST PASS\n");
    return 0;
}