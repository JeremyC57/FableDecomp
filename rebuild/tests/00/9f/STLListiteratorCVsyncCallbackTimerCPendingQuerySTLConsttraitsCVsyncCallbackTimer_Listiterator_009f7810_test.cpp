// Behaviour test for 009f7810: _List_iterator node constructor -- the iterator refers to exactly the node it was built from, through the base constructor
#include <cstdio>
static int g_base = 0;
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Query { void* q; long frame; double stamp; };
struct Node : NodeBase { Query data; };
struct IterBase { NodeBase* node; IterBase(NodeBase* x) : node(x) { ++g_base; } };
struct Iter : IterBase { Iter(Node* x) : IterBase(x) { } Query& operator*() const { return static_cast<Node*>(node)->data; } };
int main()
{
    Node n; n.next = n.prev = &n; n.data.frame = 42;
    Iter it(&n);
    Iter e((Node*)0);
    int ok = it.node == &n && (*it).frame == 42 && e.node == 0 && g_base == 2;
    std::printf(ok ? "009f7810_TEST PASS\n" : "009f7810_TEST FAIL\n");
    return ok ? 0 : 1;
}
