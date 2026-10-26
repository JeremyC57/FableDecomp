#include <cstdio>
class C3DVector { public: float X, Y, Z; };
struct CSnapToPoint {
    C3DVector Pos;
    float     FOV;
    void UpdateDesiredPos(C3DVector& desired) { desired = Pos; }
};
int main()
{
    CSnapToPoint s; s.Pos.X = 1.0f; s.Pos.Y = 2.5f; s.Pos.Z = -4.0f; s.FOV = 0.5f;
    C3DVector d; d.X = d.Y = d.Z = 9.0f;
    s.UpdateDesiredPos(d);
    if (d.X != 1.0f || d.Y != 2.5f || d.Z != -4.0f) { std::printf("008506e0_TEST FAIL\n"); return 1; }
    std::printf("008506e0_TEST PASS\n");
    return 0;
}