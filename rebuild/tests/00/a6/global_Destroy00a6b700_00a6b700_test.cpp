#include <cstdio>
static int g_d = 0; struct O { virtual void a() {} virtual void X() { ++g_d; } long r; void Rel() { if (--r == 0) X(); } };
struct P { O* p; ~P() { if (p) { p->Rel(); p = 0; } } };
static void __fastcall F(P* f, P* l, long) { for (; f != l; ++f) f->~P(); }
int main()
{
    O o; o.r = 2; P buf[2]; buf[0].p = &o; buf[1].p = &o; F(buf, buf + 2, 0); if (g_d != 1 || buf[1].p != 0) { std::printf("00a6b700_TEST FAIL\n"); return 1; }

    std::printf("00a6b700_TEST PASS\n");
    return 0;
}