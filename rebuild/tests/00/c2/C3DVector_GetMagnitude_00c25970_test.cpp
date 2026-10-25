// behaviour test for 0x00C25970 C3DVector::GetMagnitude: the Euclidean length
#include <cstdio>
#include <math.h>

inline float Magnitude(float x, float y, float z)
{
    return (float)sqrt(x * x + y * y + z * z);
}

struct C3DVector {
    float X, Y, Z;
    float GetMagnitude() const;
};

float C3DVector::GetMagnitude() const
{
    return Magnitude(X, Y, Z);
}

static bool Near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }

int main()
{
    C3DVector a = { 2.0f, 3.0f, 6.0f };
    C3DVector b = { 0.0f, 0.0f, 0.0f };
    C3DVector c = { -1.0f, 0.0f, 0.0f };
    if (!Near(a.GetMagnitude(), 7.0f) || !Near(b.GetMagnitude(), 0.0f) || !Near(c.GetMagnitude(), 1.0f)) { std::printf("00c25970_TEST FAIL\n"); return 1; }
    std::printf("00c25970_TEST PASS\n");
    return 0;
}