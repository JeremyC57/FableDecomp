#include <cstdio>
// Behaviour model: a default-constructed object has its three members cleared.
struct T { void* a; unsigned long b; void* c; T() : a(0), b(0), c(0) { } };
int main()
{
    T t;
    if (t.a || t.b || t.c) { std::printf("00c34b30_TEST FAIL\n"); return 1; }
    std::printf("00c34b30_TEST PASS\n");
    return 0;
}