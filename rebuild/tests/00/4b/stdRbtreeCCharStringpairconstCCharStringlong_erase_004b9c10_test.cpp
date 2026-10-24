// Behaviour test for 0x004B9C10 _Rb_tree<CCharString,...>::erase(first, last): the [begin, end) range clears the
// tree through _M_erase(root) and resets the header; another range erases node by node in order (successor taken
// before the unlink), destroying each key, freeing the node and decrementing the count.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_dtors = 0;
struct CCharString { int m_v; ~CCharString() { ++g_dtors; } };
struct Value { CCharString first; long second; };

struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { Value value; };

static NodeBase* g_order[8]; static int g_n = 0;
static NodeBase* Rebalance_for_erase(NodeBase* z, NodeBase*&, NodeBase*&, NodeBase*&) { g_order[g_n++] = z; return z; }
static NodeBase* g_erasedRoot = 0;

struct Tree {
    NodeBase* header; unsigned int count;
    void _M_erase(Node* x) { g_erasedRoot = x; }
    static NodeBase* Next(NodeBase* n)
    {
        if (n->right != 0) { n = n->right; while (n->left != 0) n = n->left; }
        else { NodeBase* y = n->parent; while (n == y->right) { n = y; y = y->parent; } if (n->right != y) n = y; }
        return n;
    }
    void erase(NodeBase* first, NodeBase* last)
    {
        if (first == header->left && last == header) {
            if (count != 0) {
                _M_erase(static_cast<Node*>(header->parent));
                header->left = header; header->parent = 0; header->right = header; count = 0;
            }
        }
        else
            while (first != last) {
                NodeBase* victim = first; first = Next(first);
                Node* y = static_cast<Node*>(Rebalance_for_erase(victim, header->parent, header->left, header->right));
                y->value.~Value();
                if (y) std::free(y);
                --count;
            }
    }
};

static Node* MakeNode() { void* p = std::malloc(sizeof(Node)); Node* n = new (p) Node(); return n; }

int main()
{
    NodeBase header = { false, 0, 0, 0 };
    Node* a = MakeNode(); Node* b = MakeNode(); Node* c = MakeNode();
    header.parent = b; header.left = a; header.right = c;
    b->parent = &header; b->left = a; b->right = c;
    a->parent = b; a->left = a->right = 0; c->parent = b; c->left = c->right = 0;
    Tree t = { &header, 3 };
    t.erase(a, c);                                   // erases a then b
    if (g_n != 2 || g_order[0] != a || g_order[1] != b || t.count != 1 || g_dtors != 2) { std::printf("004b9c10_TEST FAIL\n"); return 1; }
    // full range: header->left must be begin
    NodeBase h2 = { false, 0, 0, 0 }; Node d; d.parent = &h2; d.left = d.right = 0;
    h2.parent = &d; h2.left = &d; h2.right = &d;
    Tree t2 = { &h2, 1 };
    t2.erase(&d, &h2);
    if (g_erasedRoot != &d || h2.left != &h2 || h2.right != &h2 || h2.parent != 0 || t2.count != 0 || g_n != 2) { std::printf("004b9c10_TEST FAIL\n"); return 1; }
    std::free(c);
    std::printf("004b9c10_TEST PASS\n");
    return 0;
}