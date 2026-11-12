// Behaviour test for 0x00A11F20 CPhysicsMeshObject::IsSpherePartContainedInBoundingBox, through a copy of the model:
// the radius is scaled by OO_Scale, the point goes into mesh space through the inverse PositionMatrix, and the mesh's
// HasBoundingBoxIntersectedFromTo(scaled radius, local, local) decides (here a recording axis-aligned box test).
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
    void TransformPoint(const C3DVector& pSource, C3DVector& destination) const {
        destination.X = E11 * pSource.X + E21 * pSource.Y + E31 * pSource.Z + E41;
        destination.Y = E12 * pSource.X + E22 * pSource.Y + E32 * pSource.Z + E42;
        destination.Z = E13 * pSource.X + E23 * pSource.Y + E33 * pSource.Z + E43;
    }
};

static int g_calls;
static float g_radius;
static C3DVector g_from, g_to;
static bool g_same_object;

class CPhysicsMesh { public:
    C3DVector Min, Max;
    // Recording model of the callee: a sphere at `from` (== `to` for this caller) against the axis-aligned box.
    bool HasBoundingBoxIntersectedFromTo(float sphere_radius, const C3DVector& from, const C3DVector& to) const {
        ++g_calls; g_radius = sphere_radius; g_from = from; g_to = to; g_same_object = (&from == &to);
        return from.X >= Min.X - sphere_radius && from.X <= Max.X + sphere_radius &&
               from.Y >= Min.Y - sphere_radius && from.Y <= Max.Y + sphere_radius &&
               from.Z >= Min.Z - sphere_radius && from.Z <= Max.Z + sphere_radius;
    }
};

class CPhysicsMeshObject { public:
    const CPhysicsMesh* Mesh;
    CMatrix3x4 PositionMatrix;
    float Scale;
    float OO_Scale;
    bool IsSpherePartContainedInBoundingBox(const C3DVector& pos, float radius) const {
        float scaled_radius = radius * OO_Scale;
        CMatrix3x4 inverse;
        inverse.SetInverse(PositionMatrix);
        C3DVector local;
        inverse.TransformPoint(pos, local);
        return Mesh->HasBoundingBoxIntersectedFromTo(scaled_radius, local, local);
    }
};

static bool Near(float a, float b) { return std::fabs(a - b) < 1.0e-3f; }

int main()
{
    CPhysicsMesh mesh;
    mesh.Min.X = -1.0f; mesh.Min.Y = -2.0f; mesh.Min.Z = -3.0f;
    mesh.Max.X = 1.0f; mesh.Max.Y = 2.0f; mesh.Max.Z = 3.0f;

    // Position matrix: scale 2, rotation of 90 degrees about Z (row vectors), translation (10, 20, 30).
    CPhysicsMeshObject object;
    object.Mesh = &mesh;
    object.Scale = 2.0f;
    object.OO_Scale = 0.5f;
    CMatrix3x4& m = object.PositionMatrix;
    m.E11 = 0.0f;  m.E12 = 2.0f; m.E13 = 0.0f;
    m.E21 = -2.0f; m.E22 = 0.0f; m.E23 = 0.0f;
    m.E31 = 0.0f;  m.E32 = 0.0f; m.E33 = 2.0f;
    m.E41 = 10.0f; m.E42 = 20.0f; m.E43 = 30.0f;

    bool ok = true;

    // 1. The inverse really is the inverse: local -> world -> local round trip on an arbitrary point.
    C3DVector mesh_point; mesh_point.X = 0.5f; mesh_point.Y = -1.5f; mesh_point.Z = 2.5f;
    C3DVector world;
    m.TransformPoint(mesh_point, world);                    // (10 + 3, 20 + 1, 30 + 5)
    if (!Near(world.X, 13.0f) || !Near(world.Y, 21.0f) || !Near(world.Z, 35.0f)) ok = false;

    // 2. A world point whose mesh-space image is inside the box: contained, callee sees the mesh-space point twice
    //    (the same object for from and to) and the radius scaled by OO_Scale.
    g_calls = 0;
    if (!object.IsSpherePartContainedInBoundingBox(world, 3.0f)) ok = false;
    if (g_calls != 1 || !g_same_object) ok = false;
    if (!Near(g_radius, 1.5f)) ok = false;
    if (!Near(g_from.X, 0.5f) || !Near(g_from.Y, -1.5f) || !Near(g_from.Z, 2.5f)) ok = false;
    if (!Near(g_to.X, 0.5f) || !Near(g_to.Y, -1.5f) || !Near(g_to.Z, 2.5f)) ok = false;

    // 3. A point outside the box in mesh space (mesh X = 2.0): out with a zero radius, in once the scaled radius reaches it.
    C3DVector outside_mesh; outside_mesh.X = 2.0f; outside_mesh.Y = 0.0f; outside_mesh.Z = 0.0f;
    C3DVector outside_world;
    m.TransformPoint(outside_mesh, outside_world);
    if (object.IsSpherePartContainedInBoundingBox(outside_world, 0.0f)) ok = false;
    if (!Near(g_radius, 0.0f) || !Near(g_from.X, 2.0f)) ok = false;
    if (object.IsSpherePartContainedInBoundingBox(outside_world, 1.0f)) ok = false;      // scaled 0.5: still 0.5 short
    if (!object.IsSpherePartContainedInBoundingBox(outside_world, 2.5f)) ok = false;     // scaled 1.25: reaches the box
    if (g_calls != 4) ok = false;

    // 4. A non-orthogonal matrix (shear) still round-trips: the full cofactor inverse, not a transpose.
    m.E11 = 1.0f; m.E12 = 0.5f; m.E13 = 0.0f;
    m.E21 = 0.0f; m.E22 = 2.0f; m.E23 = 0.25f;
    m.E31 = 0.5f; m.E32 = 0.0f; m.E33 = 4.0f;
    m.E41 = -3.0f; m.E42 = 7.0f; m.E43 = 1.0f;
    object.OO_Scale = 0.25f;
    m.TransformPoint(mesh_point, world);
    if (!object.IsSpherePartContainedInBoundingBox(world, 8.0f)) ok = false;
    if (!Near(g_radius, 2.0f)) ok = false;
    if (!Near(g_from.X, 0.5f) || !Near(g_from.Y, -1.5f) || !Near(g_from.Z, 2.5f)) ok = false;

    if (!ok) { std::printf("00a11f20_TEST FAIL\n"); return 1; }
    std::printf("00a11f20_TEST PASS\n");
    return 0;
}