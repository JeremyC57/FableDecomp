#include <cstdio>
static void* g_v = 0; struct P { virtual void v00() {} void* v; P(void* x) : v(x) {} ~P() {} };
struct T { virtual void v00() {} virtual void R(P* p) { g_v = p->v; } }; struct X { unsigned char pad[4]; T* t; void Call(void* v) { P p(v); t->R(&p); } };
int main()
{
    int q; T t; X x; x.t = &t; x.Call(&q); if (g_v != &q) { std::printf("00c2fe20_TEST FAIL\n"); return 1; }

    std::printf("00c2fe20_TEST PASS\n");
    return 0;
}