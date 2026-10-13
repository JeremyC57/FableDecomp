#include <cstdio>
struct X { virtual void v00() {} unsigned char p[8]; void* f; X(void* a, void*) : f(a) {} };
int main()
{
    int v; X x(&v, 0); if (x.f != &v) { std::printf("00c36d80_TEST FAIL\n"); return 1; }

    std::printf("00c36d80_TEST PASS\n");
    return 0;
}