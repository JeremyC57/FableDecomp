// Behaviour test for BuildViewMatrix (0x00C1BF00): right = up x forward, columns right/up/forward, translation = -dot(position).
#include <cstdio>
struct V { float x, y, z; };
static V cross(V a, V b)
{
    V r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}
static float dot(V a, V b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static bool near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 0.0001f; }
int main()
{
    V up = { 0.0f, 1.0f, 0.0f };
    V forward = { 0.0f, 0.0f, 1.0f };
    V position = { 1.0f, 2.0f, 3.0f };
    V right = cross(up, forward);
    float m[12];
    m[0] = right.x; m[1] = up.x; m[2] = forward.x;
    m[3] = right.y; m[4] = up.y; m[5] = forward.y;
    m[6] = right.z; m[7] = up.z; m[8] = forward.z;
    m[9] = -dot(right, position);
    m[10] = -dot(up, position);
    m[11] = -dot(forward, position);
    float want[12] = { 1, 0, 0, 0, 1, 0, 0, 0, 1, -1, -2, -3 };
    for (int i = 0; i < 12; ++i) {
        if (!near(m[i], want[i])) { std::printf("00c1bf00_TEST FAIL\n"); return 1; }
    }
    std::printf("00c1bf00_TEST PASS\n");
    return 0;
}