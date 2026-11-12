#include <cstdio>
// Behaviour model: the point test forwards to the sphere test with a zero radius (the mesh object unchanged).
class C3DVector { public: float X, Y, Z; };
class CPhysicsMeshObject {
public:
    float MinX, MaxX;
    mutable const CPhysicsMeshObject* LastThis;
    mutable float LastRadius;
    bool IsSpherePartContainedInBoundingBox(const C3DVector& pos, float radius) const
    {
        LastThis = this;
        LastRadius = radius;
        return pos.X + radius >= MinX && pos.X - radius <= MaxX;
    }
    bool IsPointContainedInBoundingBox(const C3DVector& pos) const;
};
bool CPhysicsMeshObject::IsPointContainedInBoundingBox(const C3DVector& pos) const
{
    return IsSpherePartContainedInBoundingBox(pos, 0.0f);
}
int main()
{
    CPhysicsMeshObject m = { 0.0f, 10.0f, 0, -1.0f };
    C3DVector in = { 5.0f, 0.0f, 0.0f }, out = { 10.5f, 0.0f, 0.0f };
    if (!m.IsPointContainedInBoundingBox(in) || m.LastThis != &m || m.LastRadius != 0.0f) { std::printf("00a12890_TEST FAIL\n"); return 1; }
    m.LastRadius = -1.0f;
    if (m.IsPointContainedInBoundingBox(out) || m.LastRadius != 0.0f) { std::printf("00a12890_TEST FAIL\n"); return 1; }
    std::printf("00a12890_TEST PASS\n");
    return 0;
}
