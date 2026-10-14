#include <cstdio>
static int g_c = 0; struct P { virtual void v00() {} P() {} ~P() {} };
struct R { virtual void v00() {} virtual void Recv(P*) { ++g_c; } }; struct X { unsigned char pad[4]; R r; void Act() { P p; r.Recv(&p); } };
int main()
{
    X x; x.Act(); if (g_c != 1) { std::printf("00c36390_TEST FAIL\n"); return 1; }

    std::printf("00c36390_TEST PASS\n");
    return 0;
}