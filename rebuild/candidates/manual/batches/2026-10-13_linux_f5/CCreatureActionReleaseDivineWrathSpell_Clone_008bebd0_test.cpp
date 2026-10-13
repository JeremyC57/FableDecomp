#include <cstdio>
static int g_c = 0; struct Base { virtual void v00() {} unsigned char body[0x114 - 4]; Base() {} Base(const Base& o) { ++g_c; body[0] = o.body[0]; } };
struct X : Base { virtual void v00() {} Base* Clone() { return new X(*this); } };
int main()
{
    X x; x.body[0] = 5; Base* c = x.Clone(); if (!c || g_c != 1 || c->body[0] != 5) { std::printf("008bebd0_TEST FAIL\n"); return 1; }
    delete c;

    std::printf("008bebd0_TEST PASS\n");
    return 0;
}
