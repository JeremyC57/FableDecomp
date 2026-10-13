#include <cstdio>
struct B { virtual void v00() {} long c; B() : c(1) {} }; struct X : B { virtual void v00() {} unsigned char t[4]; X() : B() {} };
static X* Alloc() { return new X; }
int main()
{
    X* x = Alloc(); if (!x || x->c != 1) { std::printf("005df650_TEST FAIL\n"); return 1; }
    delete x;

    std::printf("005df650_TEST PASS\n");
    return 0;
}
