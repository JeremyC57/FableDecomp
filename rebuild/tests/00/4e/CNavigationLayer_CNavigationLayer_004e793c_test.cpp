#include <cstdio>
struct X { void* a; void* b; void* c; unsigned char f; X() : a(0), b(0), c(0), f(0) { } };
int main()
{
    X x; if (x.a || x.b || x.c || x.f) { std::printf("004e793c_TEST FAIL\n"); return 1; }

    std::printf("004e793c_TEST PASS\n");
    return 0;
}