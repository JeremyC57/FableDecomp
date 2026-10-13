#include <cstdio>
static int g_c = 0; struct Base { virtual void v00() {} unsigned char body[0x114 - 4]; Base() {} Base(const Base& o) { ++g_c; body[0] = o.body[0]; } };
struct X : Base { long e; virtual void v00() {} Base* Clone() { return new X(*this); } };
int main()
{
    X x; x.body[0] = 5; x.e = 7; X* c = (X*)x.Clone(); if (!c || g_c != 1 || c->body[0] != 5 || c->e != 7) { std::printf("008cee70_TEST FAIL\n"); return 1; }
    delete c;

    std::printf("008cee70_TEST PASS\n");
    return 0;
}
