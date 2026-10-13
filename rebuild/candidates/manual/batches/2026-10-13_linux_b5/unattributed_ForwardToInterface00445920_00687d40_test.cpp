#include <cstdio>
struct I { void* M(void* a) { return a; } };
struct O { unsigned char pad[0x24]; I* i; void* F(void* a) { return i->M(a); } };
int main()
{
    I i; O o; o.i = &i; int x; if (o.F(&x) != &x) { std::printf("00687d40_TEST FAIL\n"); return 1; }

    std::printf("00687d40_TEST PASS\n");
    return 0;
}
