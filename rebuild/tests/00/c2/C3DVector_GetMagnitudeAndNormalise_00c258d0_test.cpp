// behaviour test for 0x00C258D0 C3DVector::GetMagnitudeAndNormalise: normalise in place, return the old length; zero stays zero
#include <cstdio>
#include <math.h>

class C3DVector { public:
    float X, Y, Z;
    float GetMagnitudeAndNormalise();
};

float C3DVector::GetMagnitudeAndNormalise()
{
    float x = X, y = Y, z = Z;
    if (x == 0.0f && y == 0.0f && z == 0.0f)
        return 0.0f;
    float length = (float)sqrt(x * x + y * y + z * z);
    float inv = 1.0f / length;
    X = x * inv;
    Y = y * inv;
    Z = z * inv;
    return length;
}

static bool Near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }

int main()
{
    C3DVector v = { 3.0f, 0.0f, 4.0f };
    float len = v.GetMagnitudeAndNormalise();
    if (!Near(len, 5.0f) || !Near(v.X, 0.6f) || !Near(v.Y, 0.0f) || !Near(v.Z, 0.8f)) { std::printf("00c258d0_TEST FAIL\n"); return 1; }
    C3DVector z = { 0.0f, 0.0f, 0.0f };
    if (z.GetMagnitudeAndNormalise() != 0.0f || z.X != 0.0f || z.Y != 0.0f || z.Z != 0.0f) { std::printf("00c258d0_TEST FAIL\n"); return 1; }
    C3DVector w = { 0.0f, 0.0f, -2.0f };
    if (!Near(w.GetMagnitudeAndNormalise(), 2.0f) || !Near(w.Z, -1.0f)) { std::printf("00c258d0_TEST FAIL\n"); return 1; }
    std::printf("00c258d0_TEST PASS\n");
    return 0;
}