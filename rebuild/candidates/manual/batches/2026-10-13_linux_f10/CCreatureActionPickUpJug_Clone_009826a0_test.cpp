#include <cstdio>
struct Base { virtual void v00() {} }; static int g_c = 0;
struct X : Base { unsigned char body[0xb0 - 4]; X() {} X(const X& o) { ++g_c; body[0] = o.body[0]; } Base* Clone() { return new X(*this); } };
int main()
{
    X x; x.body[0] = 7; Base* c = x.Clone(); if (!c || g_c != 1 || static_cast<X*>(c)->body[0] != 7) { std::printf("009826a0_TEST FAIL\n"); return 1; }
    delete c;

    std::printf("009826a0_TEST PASS\n");
    return 0;
}
