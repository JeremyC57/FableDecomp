// Behaviour test for 0x008AE1E0 _Rb_tree::erase(iterator): the node's value is destroyed, the node freed, the count dropped.
#include <cstdio>
#include <new>
static int g_strings = 0, g_frees = 0;
struct Str { int id; Str() : id(0) { ++g_strings; } ~Str() { --g_strings; } };
struct Vec { Str* start; Str* finish; Vec() : start(0), finish(0) {} ~Vec() { for (Str* p = start; p != finish; ++p) p->~Str(); if (start) { ::operator delete(start); ++g_frees; } } };
struct Node { Node* left; Node* right; Str key; Vec value; };
struct Tree {
    unsigned int count;
    Node* rebalance_for_erase(Node* z) { return z; }   // stands in for _Rebalance_for_erase (returns the node to free)
    void erase(Node* position)
    {
        Node* y = rebalance_for_erase(position);
        y->~Node();
        if (y) { ::operator delete(y); ++g_frees; }
        --count;
    }
};
int main()
{
    Tree t; t.count = 2;
    Node* n = (Node*)::operator new(sizeof(Node));
    new (&n->key) Str(); new (&n->value) Vec();
    n->value.start = (Str*)::operator new(2 * sizeof(Str)); new (n->value.start) Str(); new (n->value.start + 1) Str(); n->value.finish = n->value.start + 2;
    t.erase(n);
    if (t.count != 1 || g_strings != 0 || g_frees != 2) { std::printf("008ae1e0_TEST FAIL\n"); return 1; }
    std::printf("008ae1e0_TEST PASS\n");
    return 0;
}