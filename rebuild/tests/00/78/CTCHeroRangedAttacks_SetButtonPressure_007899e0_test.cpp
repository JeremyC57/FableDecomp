#include <cstdio>

struct R { unsigned char pad[0x34]; float ButtonPressure;
    void Set(float p) { ButtonPressure = p; float m = 1.0f - 0.0001f; if (p > m) ButtonPressure = m; } };
int main()
{
    R r; r.Set(0.5f); if (r.ButtonPressure != 0.5f) { std::printf("007899e0_TEST FAIL\n"); return 1; }
    r.Set(1.0f); if (!(r.ButtonPressure < 1.0f) || r.ButtonPressure < 0.999f) { std::printf("007899e0_TEST FAIL\n"); return 1; }
    std::printf("007899e0_TEST PASS\n");
    return 0;
}