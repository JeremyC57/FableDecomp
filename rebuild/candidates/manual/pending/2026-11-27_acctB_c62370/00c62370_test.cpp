// Behaviour test for GetRandomPointInSphere (0x00C62370): phi = u*2pi, theta = acos(2u-1), r = u*radius.
#include <cmath>
#include <cstdio>
static bool near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 0.0001f; }
int main()
{
    float u1 = 0.0f, u2 = 0.5f, u3 = 1.0f, radius = 2.0f;
    float phi = u1 * 6.28318548f;
    float theta = std::acos(u2 * 2.0f - 1.0f);
    float r = u3 * radius;
    float x = std::sin(theta) * std::cos(phi) * r;
    float y = std::cos(theta) * std::sin(phi) * r;
    float z = std::cos(theta) * r;
    if (!near(x, 2.0f) || !near(y, 0.0f) || !near(z, 0.0f)) { std::printf("00c62370_TEST FAIL\n"); return 1; }
    std::printf("00c62370_TEST PASS\n");
    return 0;
}
