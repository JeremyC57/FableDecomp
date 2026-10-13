#include <cstdio>
struct PB { virtual void v00() {} void* p; void* Get() { return p; } };
struct X { virtual void v00() {} unsigned char pad[0x34]; PB m; virtual void* G() { return m.Get(); } };
int main()
{
    int v; X x; x.m.p = &v; if (x.G() != &v) { std::printf("00787c1b_TEST FAIL\n"); return 1; }

    std::printf("00787c1b_TEST PASS\n");
    return 0;
}