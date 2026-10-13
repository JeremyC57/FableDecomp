#include <cstdio>
struct T {}; struct M { bool Has(T* t) { return t != 0; } }; struct O { void* vt; M* m; };
static O g_o; static O* g_p = &g_o; static M g_m;
static bool __fastcall Has(T* t) { return g_p->m->Has(t); }
int main()
{
    g_o.m = &g_m; T t; if (!Has(&t) || Has(0)) { std::printf("004c7e70_TEST FAIL\n"); return 1; }

    std::printf("004c7e70_TEST PASS\n");
    return 0;
}
