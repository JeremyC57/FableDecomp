#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static int g_copies = 0; struct Str { char* p; Str() : p(0) {} Str(const Str& o) : p(o.p) { ++g_copies; } }; struct V { Str name; struct { long a, b, c, d, e; } r; };
struct Node { unsigned char color; unsigned char pad[3]; Node* parent; Node* left; Node* right; V value; };
struct Tree { Node* BuyNode(const V& v) { Node* n = (Node*)std::malloc(sizeof(Node)); new (&n->value) V(v); return n; } };
int main()
{
    V v; std::memset(&v, 0, sizeof v); static char s[] = "x"; v.name.p = s; v.r.e = 9;
    Tree t; Node* n = t.BuyNode(v); bool ok = n && (g_copies == 1 && n->value.name.p == s && n->value.r.e == 9); std::free(n);
    if (!ok) { std::printf("009aa9e0_TEST FAIL\n"); return 1; }
    std::printf("009aa9e0_TEST PASS\n");
    return 0;
}
