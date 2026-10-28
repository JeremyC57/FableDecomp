#include <cstdio>
#include <cmath>
// Behaviour test for the sqrt normalise at 0x00a14480.
struct V { float c[3];
    float Norm(int n) { float s = 0; for (int i = 0; i < n; ++i) s += c[i] * c[i]; float len = (float)std::sqrt(s);
        float inv = 1.0f / len; for (int i = 0; i < n; ++i) c[i] *= inv; return len; } };
int main()
{
    V v; v.c[0] = 3.0f; v.c[1] = 4.0f; v.c[2] = (3 == 3) ? 12.0f : 0.0f;
    float expect = (3 == 3) ? 13.0f : 5.0f;
    float len = v.Norm(3);
    float s = 0; for (int i = 0; i < 3; ++i) s += v.c[i] * v.c[i];
    bool ok = std::fabs(len - expect) < 1e-4f && std::fabs(s - 1.0f) < 1e-5f && std::fabs(v.c[0] - 3.0f / expect) < 1e-6f;
    if (!ok) { std::printf("00a14480_TEST FAIL\n"); return 1; }
    std::printf("00a14480_TEST PASS\n");
    return 0;
}