#include <cstdio>
#include <new>
#include <cstdlib>
static int g_c = 0; struct V { unsigned char b[8]; V() {} V(const V& o) { ++g_c; b[0] = o.b[0]; } };
struct L { struct node { node* n; node* p; unsigned char g[8]; V v; }; node* h; node* Buy(const V& v) { node* x = (node*)std::malloc(sizeof(node)); new (&x->v) V(v); return x; } };
int main()
{
    V v; v.b[0] = 7; L l; L::node* n = l.Buy(v); if (!n || g_c != 1 || n->v.b[0] != 7) { std::printf("0065f359_TEST FAIL\n"); return 1; }
    std::free(n);

    std::printf("0065f359_TEST PASS\n");
    return 0;
}