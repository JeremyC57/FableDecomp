#include <cstdio>
struct H { void* p; H(void* q) : p(q) { } }; struct X { void* f; H G() { return H(f); } };
int main()
{
    X x; int i; x.f = &i; if (x.G().p != &i) { std::printf("008000b0_TEST FAIL\n"); return 1; }

    std::printf("008000b0_TEST PASS\n");
    return 0;
}