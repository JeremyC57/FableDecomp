// Behaviour test for CTCPhysicsBase::HasSphereMeshCollidedFromTo (0x00880640): the sweep runs from/to raised by the radius (0.4 for
// the player), a hit copies the intersection normal and position into the response with Z lowered by the radius, and the mesh
// object is destroyed on both paths.
#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CPhysicsCollideResponse { C3DVector CollidePos; C3DVector CollideNormal; float Friction; };
struct CMeshIntersectionData { C3DVector IntersectionPosition; C3DVector Normal; float DistanceSquared; };
static int g_live = 0; static bool g_hit = false; static float g_radius = 0; static C3DVector g_from, g_to;
struct CPhysicsMeshObject {
    int m_id;
    CPhysicsMeshObject(int id) : m_id(id) { ++g_live; }
    CPhysicsMeshObject(const CPhysicsMeshObject& o) : m_id(o.m_id) { ++g_live; }
    ~CPhysicsMeshObject() { --g_live; }
    bool GetSphereIntersectionFromTo(float radius, const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const
    {
        g_radius = radius; g_from = from; g_to = to;
        data.IntersectionPosition = C3DVector(7, 8, 9); data.Normal = C3DVector(0, 0, 1); data.DistanceSquared = 1;
        return g_hit && m_id == 5;
    }
};
struct CThing { unsigned char Type; };
struct CTCPhysicsBase {
    CThing*   ParentThing;
    float     Radius;
    CPhysicsMeshObject GetPhysicsMeshObject() const { return CPhysicsMeshObject(5); }
    bool HasSphereMeshCollidedFromTo(const CTCPhysicsBase& other, const C3DVector& from, const C3DVector& to,
                                     CPhysicsCollideResponse& response) const;
};

bool CTCPhysicsBase::HasSphereMeshCollidedFromTo(const CTCPhysicsBase& other, const C3DVector& from, const C3DVector& to,
                                                 CPhysicsCollideResponse& response) const
{
    float radius;
    if (ParentThing->Type == 2)
        radius = 0.4f;
    else
        radius = Radius;
    CPhysicsMeshObject mesh = other.GetPhysicsMeshObject();
    C3DVector start(from.X, from.Y, from.Z + radius);
    C3DVector end(to.X, to.Y, to.Z + radius);
    CMeshIntersectionData data;
    if (mesh.GetSphereIntersectionFromTo(radius, start, end, data)) {
        response.CollideNormal = data.Normal;
        response.CollidePos = data.IntersectionPosition;
        response.CollidePos.Z -= radius;
        return true;
    }
    return false;
}

int main()
{
    int bad = 0;
    CThing villager; villager.Type = 1; CThing hero; hero.Type = 2;
    CTCPhysicsBase self; self.ParentThing = &villager; self.Radius = 1.5f;
    CTCPhysicsBase other; other.ParentThing = &villager; other.Radius = 9.0f;
    C3DVector from(1, 2, 3), to(4, 5, 6);
    CPhysicsCollideResponse r; r.CollidePos = C3DVector(0, 0, 0); r.CollideNormal = C3DVector(0, 0, 0); r.Friction = 0.25f;
    g_hit = true;
    if (!self.HasSphereMeshCollidedFromTo(other, from, to, r)) bad |= 1;
    if (g_radius != 1.5f || g_from.X != 1 || g_from.Y != 2 || g_from.Z != 4.5f || g_to.X != 4 || g_to.Z != 7.5f) bad |= 2;
    if (r.CollidePos.X != 7 || r.CollidePos.Y != 8 || r.CollidePos.Z != 7.5f || r.CollideNormal.Z != 1 || r.Friction != 0.25f) bad |= 4;
    if (g_live != 0) bad |= 8;
    self.ParentThing = &hero; g_hit = false; r.CollidePos.X = 3;
    if (self.HasSphereMeshCollidedFromTo(other, from, to, r) || g_radius != 0.4f || r.CollidePos.X != 3 || g_live != 0) bad |= 16;
    if (bad) { std::printf("00880640_TEST FAIL\n"); return 1; }
    std::printf("00880640_TEST PASS\n");
    return 0;
}
