#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CPlane {
    C3DVector Normal; float Distance;
    bool Initialise(const C3DVector& normal, float distance) { Normal = normal; Distance = distance; return true; }
};
int main()
{
    CPlane p; p.Normal.X = p.Normal.Y = p.Normal.Z = 9.0f; p.Distance = 9.0f;
    C3DVector n; n.X = 0.0f; n.Y = 0.0f; n.Z = 1.0f;
    if (!p.Initialise(n, -2.5f)) { std::printf("00a42140_TEST FAIL\n"); return 1; }
    if (p.Normal.X != 0.0f || p.Normal.Y != 0.0f || p.Normal.Z != 1.0f || p.Distance != -2.5f) { std::printf("00a42140_TEST FAIL\n"); return 1; }
    std::printf("00a42140_TEST PASS\n");
    return 0;
}