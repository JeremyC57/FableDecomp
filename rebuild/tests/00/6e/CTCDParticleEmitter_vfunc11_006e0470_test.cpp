#include <cstdio>
struct P { void* p; void* G() { return p; } }; struct X { unsigned char pad[4]; P q; void* F(long) { return q.G(); } };
int main()
{
    X x; x.q.p = &x; if (x.F(0) != &x) { std::printf("006e0470_TEST FAIL\n"); return 1; }

    std::printf("006e0470_TEST PASS\n");
    return 0;
}