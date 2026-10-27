// behaviour test for 0x004AD690: the recursive erase frees every node once and releases each value
#include <cstdio>
#include <cstdlib>
static int g_freed = 0, g_released = 0;
struct N { N* l; N* r; };
static void Erase(N* x) { while (x) { Erase(x->r); N* y = x->l; ++g_released; std::free(x); ++g_freed; x = y; } }
static N* Make(N* l, N* r) { N* n = (N*)std::malloc(sizeof(N)); n->l = l; n->r = r; return n; }
int main()
{
    N* t = Make(Make(Make(0, 0), 0), Make(0, Make(0, 0)));
    Erase(t); Erase(0);
    if (g_freed != 5 || g_released != 5) { std::printf("004ad690_TEST FAIL\n"); return 1; }
    std::printf("004ad690_TEST PASS\n");
    return 0;
}