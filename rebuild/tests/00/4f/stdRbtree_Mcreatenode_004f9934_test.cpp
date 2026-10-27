// behaviour test for 0x004F9934: the node holds the copied key and value after the four-word node header
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_copies = 0;
struct V { int v; V(int x) : v(x) { } V(const V& o) : v(o.v) { ++g_copies; } };
struct P { long k; V val; P(long k_, int v) : k(k_), val(v) { } };
struct N { int color; N* parent; N* left; N* right; P value; };
static N* Create(const P& x) { N* n = (N*)std::malloc(sizeof(N)); new (&n->value) P(x); return n; }
int main()
{
    P p(5, 42); N* n = Create(p);
    if (n->value.k != 5 || n->value.val.v != 42 || g_copies != 1) { std::printf("004f9934_TEST FAIL\n"); return 1; }
    std::free(n);
    std::printf("004f9934_TEST PASS\n");
    return 0;
}