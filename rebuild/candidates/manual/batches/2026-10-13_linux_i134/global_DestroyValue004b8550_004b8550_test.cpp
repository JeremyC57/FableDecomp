#include <cstdio>
static int g_d = 0; static void __thiscall D(void*) { ++g_d; }
struct B { long r; void (__thiscall* f)(void*); void* o; }; struct V { unsigned char k[8]; void* o; B* c; };
static void __fastcall F(V* v) { if (v->c) { --v->c->r; if (v->c->r == 0) { v->c->f(v->c->o); delete v->c; } } v->o = 0; v->c = 0; }
int main()
{
    B* b = new B; b->r = 1; b->f = D; b->o = 0; V v; v.o = (void*)1; v.c = b; F(&v); if (g_d != 1 || v.o != 0 || v.c != 0) { std::printf("004b8550_TEST FAIL\n"); return 1; }

    std::printf("004b8550_TEST PASS\n");
    return 0;
}
