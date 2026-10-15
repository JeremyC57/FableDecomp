#include <cstdio>

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

struct CRightHandedSet {
    C3DVector Up; C3DVector Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }
};

int main()
{
    C3DVector up(0.0f, 0.0f, 1.0f), fwd(0.25f, -2.0f, 3.5f);
    CRightHandedSet s(up, fwd);
    if (s.Up.X != 0.0f || s.Up.Y != 0.0f || s.Up.Z != 1.0f || s.Forward.X != 0.25f || s.Forward.Y != -2.0f || s.Forward.Z != 3.5f)
    { std::printf("00487050_TEST FAIL\n"); return 1; }
    std::printf("00487050_TEST PASS\n");
    return 0;
}