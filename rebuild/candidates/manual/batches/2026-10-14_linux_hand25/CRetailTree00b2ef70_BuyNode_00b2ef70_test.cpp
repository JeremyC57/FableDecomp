#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static int g_copies = 0; struct Vec { void* f; void* l; void* e; Vec() : f(0), l(0), e(0) {} Vec(const Vec& o) : f(o.f), l(o.l), e(o.e) { ++g_copies; } }; struct V { long a, b, c, d; Vec items; };
struct Node { unsigned char color; unsigned char pad[3]; Node* parent; Node* left; Node* right; V value; };
struct Tree { Node* BuyNode(const V& v) { Node* n = (Node*)std::malloc(sizeof(Node)); new (&n->value) V(v); return n; } };
int main()
{
    V v; std::memset(&v, 0, sizeof v); int x; v.d = 4; v.items.f = &x;
    Tree t; Node* n = t.BuyNode(v); bool ok = n && (g_copies == 1 && n->value.d == 4 && n->value.items.f == &x); std::free(n);
    if (!ok) { std::printf("00b2ef70_TEST FAIL\n"); return 1; }
    std::printf("00b2ef70_TEST PASS\n");
    return 0;
}
