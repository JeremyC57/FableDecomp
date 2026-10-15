#include <cstdio>
#include <cstring>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CRightHandedSet {
    C3DVector Up; C3DVector Forward;
    void SetDefault() { Up = C3DVector(0.0f, 0.0f, 1.0f); Forward = C3DVector(0.0f, 1.0f, 0.0f); }
};
int main()
{
    int ok = 1;
    CRightHandedSet s; s.Up = C3DVector(7.0f, 7.0f, 7.0f); s.Forward = C3DVector(8.0f, 8.0f, 8.0f);
    s.SetDefault();
    if (s.Up.X != 0.0f || s.Up.Y != 0.0f || s.Up.Z != 1.0f || s.Forward.X != 0.0f || s.Forward.Y != 1.0f || s.Forward.Z != 0.0f) ok = 0;
    if (!ok) { std::printf("00af07a0_TEST FAIL\n"); return 1; }
    std::printf("00af07a0_TEST PASS\n");
    return 0;
}