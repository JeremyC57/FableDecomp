#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static void* g_set = 0; struct Bip { virtual ~Bip() {} void* item; Bip() : item(0) {} void* GetPItem() const { return item; } void SetPItem(void* p) { item = p; g_set = p; } }; struct Ip : Bip { Ip() {} Ip(const Ip& o) : Bip() { SetPItem(o.GetPItem()); } }; struct V { long a, b; Ip target; };
struct Node { unsigned char color; unsigned char pad[3]; Node* parent; Node* left; Node* right; V value; };
struct Tree { Node* BuyNode(const V& v) { Node* n = (Node*)std::malloc(sizeof(Node)); new (&n->value) V(v); return n; } };
int main()
{
    V v; std::memset(&v, 0, sizeof v); int x; v.b = 4; v.target.SetPItem(&x); g_set = 0;
    Tree t; Node* n = t.BuyNode(v); bool ok = n && (g_set == &x && n->value.b == 4 && n->value.target.item == &x); std::free(n);
    if (!ok) { std::printf("00783dd0_TEST FAIL\n"); return 1; }
    std::printf("00783dd0_TEST PASS\n");
    return 0;
}
