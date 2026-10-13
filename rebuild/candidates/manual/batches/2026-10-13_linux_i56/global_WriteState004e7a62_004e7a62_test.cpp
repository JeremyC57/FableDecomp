#include <cstdio>
static long g_s = 0; struct N { unsigned char p[0x10]; long a; long b; N* next; };
struct Hd { unsigned char p[8]; N* lm; }; struct T { N* h; long c; };
struct O { void W(long v) { g_s += v; } void WF(float v) { g_s += (long)v; } };
static N* __cdecl NX(N* n) { return n->next; }
static void __fastcall WS(O* o, T* t) { o->W(t->c); for (N* n = ((Hd*)t->h)->lm; n != (N*)t->h; n = NX(n)) { o->W(n->a); o->W(n->b); } }
int main()
{
    Hd hd; N n1; n1.a = 1; n1.b = 2; n1.next = (N*)&hd; hd.lm = &n1; T t; t.h = (N*)&hd; t.c = 10; O o; WS(&o, &t); if (g_s != 13) { std::printf("004e7a62_TEST FAIL\n"); return 1; }

    std::printf("004e7a62_TEST PASS\n");
    return 0;
}
