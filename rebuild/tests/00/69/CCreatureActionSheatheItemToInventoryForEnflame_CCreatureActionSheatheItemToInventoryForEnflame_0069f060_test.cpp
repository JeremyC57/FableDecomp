#include <cstdio>
// Behaviour model: the derived constructor forwards its four arguments unchanged to the base and returns this.
struct Base { int a; const void* b; const void* c; long d; Base(int a_, const void* b_, const void* c_, long d_) : a(a_), b(b_), c(c_), d(d_) { } };
struct Derived : Base { Derived(int a_, const void* b_, const void* c_, long d_) : Base(a_, b_, c_, d_) { } };
int main()
{
    int x, y;
    Derived o(7, &x, &y, -3);
    if (o.a != 7 || o.b != &x || o.c != &y || o.d != -3) { std::printf("0069f060_TEST FAIL\n"); return 1; }
    std::printf("0069f060_TEST PASS\n");
    return 0;
}