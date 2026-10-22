#include <cstdio>
// Behaviour test for 00a0d280: the empty-vector constructor nulls all three pointers.
struct Vec { void* s; void* f; void* e; Vec() : s(0), f(0), e(0) { } };
int main()
{
    Vec v;
    if (v.s != 0 || v.f != 0 || v.e != 0) { std::printf("00a0d280_TEST FAIL\n"); return 1; }
    std::printf("00a0d280_TEST PASS\n");
    return 0;
}
