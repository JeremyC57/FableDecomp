#include <cstdio>
struct B; struct C; struct S; static int g_i = 0;
struct Base { unsigned char pad[0x30]; unsigned long f; void Init(B&, C&, long, long, const S&) { ++g_i; } };
struct D : Base { void Init(B& b, C& c, long p, long q, const S& s) { f = 0; Base::Init(b, c, p, q, s); } };
int main()
{
    D d; d.f = 9; B* b = 0; C* c = 0; S* s = 0; d.Init(*b, *c, 1, 2, *s); if (d.f || g_i != 1) { std::printf("008fd860_TEST FAIL\n"); return 1; }

    std::printf("008fd860_TEST PASS\n");
    return 0;
}