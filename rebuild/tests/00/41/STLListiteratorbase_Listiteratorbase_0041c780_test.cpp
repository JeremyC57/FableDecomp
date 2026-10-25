// Behaviour test for 0041c780: _List_iterator_base(_List_node_base*) -- stores the node and leaves the node untouched
#include <cstdio>
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct IterBase { NodeBase* node; IterBase(NodeBase* x) : node(x) { } };
int main()
{
    NodeBase a, b; a.next = &b; a.prev = &b; b.next = &a; b.prev = &a;
    IterBase i(&a); IterBase j(0); IterBase k(a.next);
    int ok = sizeof(IterBase) == 4 && i.node == &a && j.node == 0 && k.node == &b && a.next == &b;
    std::printf(ok ? "0041c780_TEST PASS\n" : "0041c780_TEST FAIL\n");
    return ok ? 0 : 1;
}
