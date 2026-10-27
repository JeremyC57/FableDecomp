#include <cstdio>
// Behaviour model: only a changed value marks the field dirty.
struct B { long f; unsigned long dirty; void Set(long v) { if (f != v) { f = v; dirty |= 2; } } };
int main()
{
    B b = { 5, 0 };
    b.Set(5);
    if (b.dirty) { std::printf("00c40880_TEST FAIL\n"); return 1; }
    b.Set(9);
    if (b.f != 9 || b.dirty != 2) { std::printf("00c40880_TEST FAIL\n"); return 1; }
    std::printf("00c40880_TEST PASS\n");
    return 0;
}