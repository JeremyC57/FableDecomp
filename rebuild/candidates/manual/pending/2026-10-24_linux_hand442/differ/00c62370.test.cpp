// behaviour test for 0x00C62370 RandomVectorInSphere: theta = 2*pi*u1, phi = acos(2*u2 - 1), r = u3*radius
#include <cstdio>
#include <cmath>
struct CPKVector3 { float x, y, z; };
static int g_seq[3]; static int g_pos = 0;
static float RandomUnit() { return (float)g_seq[g_pos++ % 3] * (1.0f / 32767.0f); }
static CPKVector3 RandomVectorInSphere(float radius)
{
    float theta = RandomUnit() * 6.2831855f;
    double phi = std::acos(RandomUnit() * 2.0f - 1.0f);
    float r = RandomUnit() * radius;
    CPKVector3 v;
    v.x = (float)(std::sin(phi) * std::cos(theta) * r);
    v.y = (float)(std::cos(phi) * std::sin(theta) * r);
    v.z = (float)(std::cos(phi) * r);
    return v;
}
int main()
{
    // u1 = 0 (theta 0), u2 = 1/2 (phi = pi/2), u3 = 1 (r = radius): the +x axis
    g_seq[0] = 0; g_seq[1] = 16384; g_seq[2] = 32767;
    CPKVector3 v = RandomVectorInSphere(2.0f);
    if (std::fabs(v.x - 2.0f) > 1e-3f || std::fabs(v.y) > 1e-3f || std::fabs(v.z) > 1e-3f) { std::printf("00c62370_TEST FAIL\n"); return 1; }
    // u2 = 1 (phi = 0): straight up z = r; y = sin(theta)*cos(0)*r follows retail's cos(phi) term
    g_seq[0] = 8192; g_seq[1] = 32767; g_seq[2] = 32767; g_pos = 0;
    v = RandomVectorInSphere(1.0f);
    if (std::fabs(v.z - 1.0f) > 1e-3f || std::fabs(v.x) > 1e-3f || std::fabs(v.y - 1.0f) > 1e-3f) { std::printf("00c62370_TEST FAIL\n"); return 1; }
    std::printf("00c62370_TEST PASS\n");
    return 0;
}
