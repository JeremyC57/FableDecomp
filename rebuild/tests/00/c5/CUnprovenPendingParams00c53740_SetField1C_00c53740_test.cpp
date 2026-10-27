#include <cstdio>
// Behaviour model: the stored value is clamped and the dirty bit always raised.
struct B { unsigned long dirty; float f;
    void Set(float v) { f = v < 20.0f ? 20.0f : (v > 2000.0f ? 2000.0f : v); dirty |= 8; } };
int main()
{
    B b = { 0, 0.0f };
    b.Set(-1.0e9f);
    if (b.f != 20.0f || b.dirty != 8) { std::printf("00c53740_TEST FAIL\n"); return 1; }
    b.Set(1.0e9f);
    if (b.f != 2000.0f) { std::printf("00c53740_TEST FAIL\n"); return 1; }
    b.Set((20.0f + 2000.0f) / 2);
    if (b.f != (20.0f + 2000.0f) / 2) { std::printf("00c53740_TEST FAIL\n"); return 1; }
    std::printf("00c53740_TEST PASS\n");
    return 0;
}