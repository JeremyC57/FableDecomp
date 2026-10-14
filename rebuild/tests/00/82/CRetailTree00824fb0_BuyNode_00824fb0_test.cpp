#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
struct V { long a, b, c, d, e; struct { long x, y, z; } vec; };
struct Node { unsigned char color; unsigned char pad[3]; Node* parent; Node* left; Node* right; V value; };
struct Tree { Node* BuyNode(const V& v) { Node* n = (Node*)std::malloc(sizeof(Node)); new (&n->value) V(v); return n; } };
int main()
{
    V v; std::memset(&v, 0, sizeof v); v.e = 5; v.vec.z = 9;
    Tree t; Node* n = t.BuyNode(v); bool ok = n && (n->value.e == 5 && n->value.vec.z == 9); std::free(n);
    if (!ok) { std::printf("00824fb0_TEST FAIL\n"); return 1; }
    std::printf("00824fb0_TEST PASS\n");
    return 0;
}