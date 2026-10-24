// Behaviour test for 0x009F0FA0 _Rb_tree::_M_insert(x, y, v, w): left/right placement, header leftmost/rightmost/root
// maintenance, the new node's links, the rebalance call and the count.
#include <cstdio>
#include <cstdlib>

struct NodeBase { bool Color; NodeBase* Parent; NodeBase* Left; NodeBase* Right; };
struct Node : NodeBase { unsigned long Key; };

static int gRebalances = 0;
static NodeBase** gRootSeen = 0;
static void Rebalance(NodeBase*, NodeBase*& root) { ++gRebalances; gRootSeen = &root; }

struct Tree
{
    NodeBase* Header;
    unsigned int Count;
    Node* CreateNode(const unsigned long& v) { Node* n = (Node*)std::malloc(sizeof(Node)); n->Key = v; return n; }
    Node* Insert(NodeBase* x_, NodeBase* y_, const unsigned long& v, NodeBase* w_)
    {
        Node* w = (Node*)w_; Node* x = (Node*)x_; Node* y = (Node*)y_; Node* z;
        if (y == Header || (w == 0 && (x != 0 || v < y->Key))) {
            z = CreateNode(v);
            y->Left = z;
            if (y == Header) { Header->Parent = z; Header->Right = z; }
            else if (y == Header->Left) Header->Left = z;
        } else {
            z = CreateNode(v);
            y->Right = z;
            if (y == Header->Right) Header->Right = z;
        }
        z->Parent = y; z->Left = 0; z->Right = 0;
        Rebalance(z, Header->Parent);
        ++Count;
        return z;
    }
};

int main()
{
    NodeBase header; header.Color = false; header.Parent = 0; header.Left = &header; header.Right = &header;
    Tree t; t.Header = &header; t.Count = 0;
    unsigned long k10 = 10, k5 = 5, k20 = 20;
    Node* a = t.Insert(0, &header, k10, 0);                 // first node: root, leftmost, rightmost
    bool ok = header.Parent == a && header.Left == a && header.Right == a && a->Parent == &header && a->Left == 0 && a->Right == 0;
    Node* b = t.Insert(0, a, k5, 0);                        // 5 < 10: left child, new leftmost
    ok = ok && a->Left == b && header.Left == b && header.Right == a && b->Parent == a;
    Node* c = t.Insert(0, a, k20, 0);                       // 20 > 10: right child, new rightmost
    ok = ok && a->Right == c && header.Right == c && header.Left == b && c->Parent == a;
    ok = ok && t.Count == 3 && gRebalances == 3 && gRootSeen == &header.Parent;
    std::free(a); std::free(b); std::free(c);
    if (!ok) { std::printf("009f0fa0_TEST FAIL\n"); return 1; }
    std::printf("009f0fa0_TEST PASS\n");
    return 0;
}