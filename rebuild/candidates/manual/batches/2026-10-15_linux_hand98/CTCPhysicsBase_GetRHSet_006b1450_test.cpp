#include <cstdio>
struct C3DVector { float X, Y, Z; C3DVector() { } C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; CRightHandedSet() { } };
static CRightHandedSet g_DefaultRHSet;
struct CTCPhysicsBase {
    CRightHandedSet GetRHSet(float interpolation) const { return g_DefaultRHSet; }
};
int main()
{
    g_DefaultRHSet.Up.X = 0.0f; g_DefaultRHSet.Up.Y = 0.0f; g_DefaultRHSet.Up.Z = 1.0f;
    g_DefaultRHSet.Forward.X = 0.0f; g_DefaultRHSet.Forward.Y = 1.0f; g_DefaultRHSet.Forward.Z = 0.0f;
    CTCPhysicsBase b;
    CRightHandedSet r = b.GetRHSet(0.5f);
    if (r.Up.Z != 1.0f || r.Up.X != 0.0f || r.Forward.Y != 1.0f || r.Forward.Z != 0.0f || sizeof(CRightHandedSet) != 24) { std::printf("006b1450_TEST FAIL\n"); return 1; }
    std::printf("006b1450_TEST PASS\n");
    return 0;
}
