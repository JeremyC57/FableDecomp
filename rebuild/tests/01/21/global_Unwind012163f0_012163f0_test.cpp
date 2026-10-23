#include <cstdio>
static int g_r = 0; struct X; struct R { void Rm(X*) { ++g_r; } }; struct B { virtual void v() {} ~B() {} }; struct X : B { unsigned char p[16]; R* r; ~X() { r->Rm(this); } };
int main()
{
    R r; { X x; x.r = &r; } if (g_r != 1) { std::printf("00c60b90_TEST FAIL\n"); return 1; }
    std::printf("012163f0_TEST PASS\n");
    return 0;
}