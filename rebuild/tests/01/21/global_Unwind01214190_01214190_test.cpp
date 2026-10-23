#include <cstdio>
struct B { virtual void v() {} ~B() {} }; struct X : B { void* t; long n; ~X() { if (t) { ::operator delete(t); n = 0; t = 0; } } };
static bool Check() { { X x; x.t = ::operator new(8); x.n = 1; } { X y; y.t = 0; y.n = 2; } return true; }

int main()
{
    if (!Check()) { std::printf("00c31f00_TEST FAIL\n"); return 1; }
    std::printf("01214190_TEST PASS\n");
    return 0;
}