#include <cstdio>
struct Tg { Tg() {} }; struct X { void* H(void* f, const Tg&, long z) { return (char*)f + z; } void* F(void* f, void*) { return H(f, Tg(), 0); } };
int main()
{
    X x; int v; if (x.F(&v, 0) != &v) { std::printf("0053cc90_TEST FAIL\n"); return 1; }

    std::printf("0053cc90_TEST PASS\n");
    return 0;
}