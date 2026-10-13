#include <cstdio>
static int g_r = 0, g_a = 0, g_n = 2; struct It;
struct Src { virtual void a() {} virtual long GetCount() { return 3; } virtual It* First() { return g_n-- > 0 ? (It*)1 : 0; } virtual void Remove(It*) { ++g_r; } };
struct Tgt { virtual long GetCount() { return 4; } virtual void Reserve(long n) { if (n == 7) g_a += 100; } };
struct X { virtual void Add(It*) { ++g_a; } Tgt* t; void T(Src* s) { t->Reserve(s->GetCount() + t->GetCount()); for (It* i = s->First(); i; i = s->First()) { s->Remove(i); Add(i); } } };
int main()
{
    Src s; Tgt t; X x; x.t = &t; x.T(&s); if (g_r != 2 || g_a != 102) { std::printf("00c0a1b0_TEST FAIL\n"); return 1; }

    std::printf("00c0a1b0_TEST PASS\n");
    return 0;
}