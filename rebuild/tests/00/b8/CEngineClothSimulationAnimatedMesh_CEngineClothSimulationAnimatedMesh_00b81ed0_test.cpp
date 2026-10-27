#include <cstdio>
// Behaviour model: the derived constructor passes its arguments through unchanged.
struct Base { int a; unsigned long b; int c; Base(int x, unsigned long y, int z) : a(x), b(y), c(z) { } };
struct Derived : Base { Derived(int x, unsigned long y, int z) : Base(x, y, z) { } };
int main()
{
    Derived d(1, 2, 3);
    if (d.a != 1 || d.b != 2 || d.c != 3) { std::printf("00b81ed0_TEST FAIL\n"); return 1; }
    std::printf("00b81ed0_TEST PASS\n");
    return 0;
}