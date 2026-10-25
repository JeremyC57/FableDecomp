// Behaviour test for 00c402e0: CPKVector3::Normalise -- scales a non-zero vector to unit length (left alone when already unit) and returns the original length; 0 for the zero vector
#include <cstdio>
#include <math.h>

struct CPKVector3 {
    float x, y, z;
    float Normalise()
    {
        if (x == 0.0f && y == 0.0f && z == 0.0f)
            return 0.0f;
        float length = (float)sqrt(z * z + y * y + x * x);
        if (1.0f != length) {
            float scale = 1.0f / length;
            x *= scale;
            y *= scale;
            z *= scale;
        }
        return length;
    }
};

static bool near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }
static int fail() { std::printf("00c402e0_TEST FAIL\n"); return 1; }

int main()
{
    CPKVector3 v = { 3.0f, 0.0f, 4.0f };
    if (!near(v.Normalise(), 5.0f) || !near(v.x, 0.6f) || !near(v.z, 0.8f) || v.y != 0.0f) return fail();
    CPKVector3 z = { 0.0f, 0.0f, 0.0f };
    if (z.Normalise() != 0.0f || z.x != 0.0f) return fail();
    CPKVector3 u = { 0.0f, 1.0f, 0.0f };
    if (u.Normalise() != 1.0f || u.y != 1.0f) return fail();
    std::printf("00c402e0_TEST PASS\n");
    return 0;
}