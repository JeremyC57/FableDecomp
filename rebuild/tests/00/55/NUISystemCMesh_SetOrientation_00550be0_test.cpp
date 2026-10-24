#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CMesh {
    CRightHandedSet Orientation;
    C3DVector RotationAxis;
    float RotationSpeed;
    void SetOrientation(CRightHandedSet& o) { Orientation = o; }
};
int main()
{
    CMesh m;
    CRightHandedSet z = { { 0, 0, 0 }, { 0, 0, 0 } };
    m.Orientation = z; m.RotationSpeed = 5.0f;
    CRightHandedSet o = { { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } };
    m.SetOrientation(o);
    if (m.Orientation.Up.Z != 1.0f || m.Orientation.Forward.Y != 1.0f || m.Orientation.Up.X != 0.0f || m.RotationSpeed != 5.0f)
        { std::printf("00550be0_TEST FAIL\n"); return 1; }
    std::printf("00550be0_TEST PASS\n");
    return 0;
}