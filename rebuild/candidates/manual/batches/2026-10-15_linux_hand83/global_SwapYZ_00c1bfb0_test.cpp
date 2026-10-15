// Behaviour test for 00c1bfb0 (_global::SwapYZ): the result keeps X and exchanges Y and Z.
#include <cstdio>

struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

C3DVector SwapYZ(const C3DVector& v)
{
    return C3DVector(v.X, v.Z, v.Y);
}

int main()
{
    C3DVector v(1.0f, 2.0f, 3.0f);
    C3DVector r = SwapYZ(v);
    if (r.X != 1.0f || r.Y != 3.0f || r.Z != 2.0f || v.Y != 2.0f) { std::printf("00c1bfb0_TEST FAIL\n"); return 1; }
    std::printf("00c1bfb0_TEST PASS\n");
    return 0;
}
