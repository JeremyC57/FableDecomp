#include <cstdio>
// Behaviour model: the stored value is clamped and the dirty bit always raised.
struct B { unsigned long dirty; float f;
    void Set(float v) { f = v < 100.0f ? 100.0f : (v > 20000.0f ? 20000.0f : v); dirty |= 0x10; } };
int main()
{
    B b = { 0, 0.0f };
    b.Set(-1.0e9f);
    if (b.f != 100.0f || b.dirty != 0x10) { std::printf("00c537a0_TEST FAIL\n"); return 1; }
    b.Set(1.0e9f);
    if (b.f != 20000.0f) { std::printf("00c537a0_TEST FAIL\n"); return 1; }
    b.Set((100.0f + 20000.0f) / 2);
    if (b.f != (100.0f + 20000.0f) / 2) { std::printf("00c537a0_TEST FAIL\n"); return 1; }
    std::printf("00c537a0_TEST PASS\n");
    return 0;
}