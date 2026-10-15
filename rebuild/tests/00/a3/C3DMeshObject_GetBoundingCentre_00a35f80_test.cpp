// Behaviour test for 0x00A35F80 C3DMeshObject::GetBoundingCentre
#include <cstdio>
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {} };
struct CMatrix3x4 { C3DVector XAxis, YAxis, ZAxis, Position; };
struct C3DBoundingSphere {
    C3DVector Centre; float Radius;
    C3DBoundingSphere() {}
    C3DBoundingSphere(const C3DBoundingSphere& o) : Centre(o.Centre), Radius(o.Radius) {}
};
struct C3DMeshInfo { C3DBoundingSphere BoundingSphere; };
struct C3DMeshObject {
    C3DMeshInfo* MeshInfo; CMatrix3x4 PositionMatrix;
    C3DVector GetBoundingCentre() const
    {
        C3DBoundingSphere sphere = MeshInfo->BoundingSphere;
        C3DVector result;
        result.X = PositionMatrix.XAxis.X * sphere.Centre.X + PositionMatrix.YAxis.X * sphere.Centre.Y + PositionMatrix.ZAxis.X * sphere.Centre.Z + PositionMatrix.Position.X;
        result.Y = PositionMatrix.XAxis.Y * sphere.Centre.X + PositionMatrix.YAxis.Y * sphere.Centre.Y + PositionMatrix.ZAxis.Y * sphere.Centre.Z + PositionMatrix.Position.Y;
        result.Z = PositionMatrix.XAxis.Z * sphere.Centre.X + PositionMatrix.YAxis.Z * sphere.Centre.Y + PositionMatrix.ZAxis.Z * sphere.Centre.Z + PositionMatrix.Position.Z;
        return result;
    }
};
static void Set(C3DVector& v, float x, float y, float z) { v.X = x; v.Y = y; v.Z = z; }
int main()
{
    C3DMeshInfo info; Set(info.BoundingSphere.Centre, 1, 2, 3); info.BoundingSphere.Radius = 9;
    C3DMeshObject o; o.MeshInfo = &info;
    // 90 degrees about Z (X axis -> Y, Y axis -> -X), translated by (10, 20, 30)
    Set(o.PositionMatrix.XAxis, 0, 1, 0); Set(o.PositionMatrix.YAxis, -1, 0, 0); Set(o.PositionMatrix.ZAxis, 0, 0, 1); Set(o.PositionMatrix.Position, 10, 20, 30);
    C3DVector c = o.GetBoundingCentre();
    bool ok = c.X == 8.0f && c.Y == 21.0f && c.Z == 33.0f;
    if (!ok) { std::printf("00a35f80_TEST FAIL\n"); return 1; }
    std::printf("00a35f80_TEST PASS\n");
    return 0;
}