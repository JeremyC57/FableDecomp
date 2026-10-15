#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct C3DBoundingBox { C3DVector Min; C3DVector Max; };
struct CMatrix3x4 {
    C3DVector Row[3]; C3DVector Position;
    C3DVector Rotate(const C3DVector& v) const
    {
        C3DVector r;
        r.X = v.X * Row[0].X + v.Y * Row[1].X + v.Z * Row[2].X;
        r.Y = v.X * Row[0].Y + v.Y * Row[1].Y + v.Z * Row[2].Y;
        r.Z = v.X * Row[0].Z + v.Y * Row[1].Z + v.Z * Row[2].Z;
        return r;
    }
};
struct CPhysicsMesh { C3DBoundingBox BoundingBox; };
struct CPhysicsMeshObject {
    const CPhysicsMesh* Mesh; CMatrix3x4 PositionMatrix;
    C3DVector GetBoundingBoxYEdge() const
    { const C3DBoundingBox& box = Mesh->BoundingBox; return PositionMatrix.Rotate(C3DVector(0.0f, (box.Max.Y - box.Min.Y) * 0.5f, 0.0f)); }
    C3DVector GetBoundingBoxZEdge() const
    { const C3DBoundingBox& box = Mesh->BoundingBox; return PositionMatrix.Rotate(C3DVector(0.0f, 0.0f, (box.Max.Z - box.Min.Z) * 0.5f)); }
};
static void Setup(CPhysicsMesh& m, CPhysicsMeshObject& o)
{
    m.BoundingBox.Min = C3DVector(-1.0f, -2.0f, -3.0f); m.BoundingBox.Max = C3DVector(1.0f, 4.0f, 5.0f);
    o.Mesh = &m;
    o.PositionMatrix.Row[0] = C3DVector(0.0f, 1.0f, 0.0f);     // a 90-degree turn: x -> y, y -> -x, z -> z
    o.PositionMatrix.Row[1] = C3DVector(-1.0f, 0.0f, 0.0f);
    o.PositionMatrix.Row[2] = C3DVector(0.0f, 0.0f, 1.0f);
    o.PositionMatrix.Position = C3DVector(100.0f, 100.0f, 100.0f);
}
int main()
{
    CPhysicsMesh m; CPhysicsMeshObject o; Setup(m, o);
    C3DVector e = o.GetBoundingBoxYEdge();         // half height 3 along row 1 = (-3, 0, 0)
    if (e.X != -3.0f || e.Y != 0.0f || e.Z != 0.0f) { std::printf("00a11c90_TEST FAIL\n"); return 1; }
    std::printf("00a11c90_TEST PASS\n");
    return 0;
}
