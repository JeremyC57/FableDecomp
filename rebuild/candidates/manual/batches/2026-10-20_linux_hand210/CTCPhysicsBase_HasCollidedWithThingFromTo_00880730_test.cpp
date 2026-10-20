// Behaviour test for CTCPhysicsBase::HasCollidedWithThingFromTo (0x00880730): mesh bodies use the swept mesh test and flip the
// normal when the global switch is set; cylinders forward the cylinder test at `to`; spheres test at `to` and report `from`.
#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
};
struct CPhysicsCollideResponse { C3DVector CollidePos; C3DVector CollideNormal; float Friction; };
bool g_flipMeshCollideNormals_013b86ec = false;
static int g_calls[3] = { 0, 0, 0 }; static const C3DVector* g_at = 0;
struct CTCPhysicsBase {
    int m_type; bool m_hit;
    bool IsPhysicsMeshTypeMesh() const { return m_type == 1; }
    bool IsPhysicsMeshTypeCylinder() const { return m_type == 2; }
    bool HasSphereMeshCollidedFromTo(const CTCPhysicsBase& other, const C3DVector& from, const C3DVector& to, CPhysicsCollideResponse& response) const
    { ++g_calls[0]; g_at = &to; if (other.m_hit) { response.CollideNormal.X = 0; response.CollideNormal.Y = 0.5f; response.CollideNormal.Z = 1; } return other.m_hit; }
    bool HasSphereCylinderCollidedAt(const CTCPhysicsBase& other, const C3DVector& pos, CPhysicsCollideResponse& response) const
    { ++g_calls[1]; g_at = &pos; return other.m_hit; }
    bool HasSphereSphereCollidedAt(const CTCPhysicsBase& other, const C3DVector& pos, CPhysicsCollideResponse& response) const
    { ++g_calls[2]; g_at = &pos; return other.m_hit; }
    bool HasCollidedWithThingFromTo(const CTCPhysicsBase& other, const C3DVector& from, const C3DVector& to,
                                    CPhysicsCollideResponse& response) const;
};

bool CTCPhysicsBase::HasCollidedWithThingFromTo(const CTCPhysicsBase& other, const C3DVector& from, const C3DVector& to,
                                                CPhysicsCollideResponse& response) const
{
    if (other.IsPhysicsMeshTypeMesh()) {
        if (HasSphereMeshCollidedFromTo(other, from, to, response)) {
            float scale = g_flipMeshCollideNormals_013b86ec ? -1.0f : 1.0f;
            response.CollideNormal *= scale;
            return true;
        }
        return false;
    }
    if (other.IsPhysicsMeshTypeCylinder())
        return HasSphereCylinderCollidedAt(other, to, response);
    if (HasSphereSphereCollidedAt(other, to, response)) {
        response.CollidePos = from;
        return true;
    }
    return false;
}

int main()
{
    int bad = 0;
    CTCPhysicsBase self; self.m_type = 0; self.m_hit = false;
    CTCPhysicsBase mesh; mesh.m_type = 1; mesh.m_hit = true;
    C3DVector from = { 1, 2, 3 }, to = { 4, 5, 6 };
    CPhysicsCollideResponse r = { { 0, 0, 0 }, { 0, 0, 0 }, 0 };
    if (!self.HasCollidedWithThingFromTo(mesh, from, to, r) || g_calls[0] != 1 || g_at != &to || r.CollideNormal.Z != 1.0f) bad |= 1;
    g_flipMeshCollideNormals_013b86ec = true;
    if (!self.HasCollidedWithThingFromTo(mesh, from, to, r) || r.CollideNormal.Z != -1.0f || r.CollideNormal.Y != -0.5f) bad |= 2;
    mesh.m_hit = false;
    if (self.HasCollidedWithThingFromTo(mesh, from, to, r) || g_calls[0] != 3) bad |= 4;
    CTCPhysicsBase cyl; cyl.m_type = 2; cyl.m_hit = true;
    if (!self.HasCollidedWithThingFromTo(cyl, from, to, r) || g_calls[1] != 1 || g_at != &to || r.CollidePos.X != 0.0f) bad |= 8;
    CTCPhysicsBase sphere; sphere.m_type = 0; sphere.m_hit = true;
    if (!self.HasCollidedWithThingFromTo(sphere, from, to, r) || g_calls[2] != 1 || r.CollidePos.X != 1 || r.CollidePos.Z != 3) bad |= 16;
    sphere.m_hit = false; r.CollidePos.X = 9;
    if (self.HasCollidedWithThingFromTo(sphere, from, to, r) || r.CollidePos.X != 9) bad |= 32;
    if (bad) { std::printf("00880730_TEST FAIL\n"); return 1; }
    std::printf("00880730_TEST PASS\n");
    return 0;
}
