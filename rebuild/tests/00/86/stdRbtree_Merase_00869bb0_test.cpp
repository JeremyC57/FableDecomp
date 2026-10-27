// behaviour test for 0x00869BB0: the recursive erase drops one reference per node and destroys a pointee on its last one
#include <cstdio>
#include <cstdlib>
static int g_destroyed = 0, g_freed = 0;
struct Obj { long refs; };
struct N { N* l; N* r; Obj* p; };
static void Erase(N* x) { while (x) { Erase(x->r); N* y = x->l; if (x->p) { if (--x->p->refs == 0) ++g_destroyed; x->p = 0; } std::free(x); ++g_freed; x = y; } }
static N* Make(N* l, N* r, Obj* p) { N* n = (N*)std::malloc(sizeof(N)); n->l = l; n->r = r; n->p = p; return n; }
int main()
{
    Obj a = { 1 }, b = { 2 };
    N* t = Make(Make(0, 0, &a), Make(0, 0, &b), &b);
    Erase(t);
    if (g_freed != 3 || g_destroyed != 2 || a.refs != 0 || b.refs != 0) { std::printf("00869bb0_TEST FAIL\n"); return 1; }
    std::printf("00869bb0_TEST PASS\n");
    return 0;
}