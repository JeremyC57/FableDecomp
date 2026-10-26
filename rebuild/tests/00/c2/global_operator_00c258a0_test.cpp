// behaviour test for 0x00C258A0 operator*(const C3DVector&, float): component-wise scale
#include <cstdio>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

C3DVector operator*(const C3DVector& v, float s)
{
    return C3DVector(v.X * s, v.Y * s, v.Z * s);
}

static bool Near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }

int main()
{
    C3DVector v(1.5f, -2.0f, 4.0f);
    C3DVector r = v * 2.0f;
    if (!Near(r.X, 3.0f) || !Near(r.Y, -4.0f) || !Near(r.Z, 8.0f)) { std::printf("00c258a0_TEST FAIL\n"); return 1; }
    r = v * 0.0f;
    if (!Near(r.X, 0.0f) || !Near(r.Y, 0.0f) || !Near(r.Z, 0.0f)) { std::printf("00c258a0_TEST FAIL\n"); return 1; }
    std::printf("00c258a0_TEST PASS\n");
    return 0;
}