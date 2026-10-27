#include <cstdio>
// Behaviour model: the stored value is clamped and the dirty bit always raised.
struct B { unsigned long dirty; float f;
    void Set(float v) { f = v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v); dirty |= 2; } };
int main()
{
    B b = { 0, 0.0f };
    b.Set(-1.0e9f);
    if (b.f != 0.0f || b.dirty != 2) { std::printf("00c53980_TEST FAIL\n"); return 1; }
    b.Set(1.0e9f);
    if (b.f != 1.0f) { std::printf("00c53980_TEST FAIL\n"); return 1; }
    b.Set((0.0f + 1.0f) / 2);
    if (b.f != (0.0f + 1.0f) / 2) { std::printf("00c53980_TEST FAIL\n"); return 1; }
    std::printf("00c53980_TEST PASS\n");
    return 0;
}