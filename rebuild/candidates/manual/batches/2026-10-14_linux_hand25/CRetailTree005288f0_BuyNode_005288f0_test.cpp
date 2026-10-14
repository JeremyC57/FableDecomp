#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
struct V { long key, kind; struct { long a, b, c, d; } data; };
struct Node { unsigned char color; unsigned char pad[3]; Node* parent; Node* left; Node* right; V value; };
struct Tree { Node* BuyNode(const V& v) { Node* n = (Node*)std::malloc(sizeof(Node)); new (&n->value) V(v); return n; } };
int main()
{
    V v; std::memset(&v, 0, sizeof v); v.key = 5; v.kind = 6; v.data.d = 9;
    Tree t; Node* n = t.BuyNode(v); bool ok = n && (n->value.key == 5 && n->value.kind == 6 && n->value.data.d == 9); std::free(n);
    if (!ok) { std::printf("005288f0_TEST FAIL\n"); return 1; }
    std::printf("005288f0_TEST PASS\n");
    return 0;
}
