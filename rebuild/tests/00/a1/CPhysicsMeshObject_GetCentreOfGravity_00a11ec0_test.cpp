#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
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
struct CPhysicsMesh { C3DVector CentreOfGravity; };
struct CPhysicsMeshObject {
    const CPhysicsMesh* Mesh; CMatrix3x4 PositionMatrix; float Scale; float OO_Scale;
    C3DVector GetCentreOfGravity() const { return PositionMatrix.Rotate(Mesh->CentreOfGravity); }
};
static void Set(C3DVector& v, float x, float y, float z) { v.X = x; v.Y = y; v.Z = z; }
int main()
{
    CPhysicsMesh mesh; Set(mesh.CentreOfGravity, 1.0f, 2.0f, 3.0f);
    CPhysicsMeshObject o; o.Mesh = &mesh; o.Scale = 1.0f; o.OO_Scale = 1.0f;
    // a 90-degree turn about Z: X -> Y, Y -> -X, plus a translation that must be ignored
    Set(o.PositionMatrix.Row[0], 0.0f, 1.0f, 0.0f); Set(o.PositionMatrix.Row[1], -1.0f, 0.0f, 0.0f);
    Set(o.PositionMatrix.Row[2], 0.0f, 0.0f, 1.0f); Set(o.PositionMatrix.Position, 100.0f, 100.0f, 100.0f);
    C3DVector r = o.GetCentreOfGravity();
    if (r.X != -2.0f || r.Y != 1.0f || r.Z != 3.0f) { std::printf("00a11ec0_TEST FAIL\n"); return 1; }
    Set(o.PositionMatrix.Row[0], 2.0f, 0.0f, 0.0f); Set(o.PositionMatrix.Row[1], 0.0f, 2.0f, 0.0f); Set(o.PositionMatrix.Row[2], 0.0f, 0.0f, 2.0f);
    r = o.GetCentreOfGravity();
    if (r.X != 2.0f || r.Y != 4.0f || r.Z != 6.0f) { std::printf("00a11ec0_TEST FAIL\n"); return 1; }
    std::printf("00a11ec0_TEST PASS\n");
    return 0;
}