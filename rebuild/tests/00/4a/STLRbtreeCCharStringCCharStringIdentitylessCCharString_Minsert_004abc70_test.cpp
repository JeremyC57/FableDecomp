#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

// Behaviour model of _Rb_tree<CCharString,...>::_M_insert (0x004ABC70): link a new node left of y when y is the
// header, x is set or v < key(y) (without a hint), else right of y; fix root/leftmost/rightmost; rebalance; count.
struct CCharString {
    const char* p;
    CCharString(const char* s) : p(s) { }
    CCharString(const CCharString& o) : p(o.p) { }
    bool operator<(const CCharString& o) const
    {
        if (o.p == p) return false;
        if (p == 0) return true;
        if (o.p == 0) return false;
        return std::strcmp(p, o.p) < 0;
    }
};
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { CCharString value; };
static int g_rebalances = 0;
static NodeBase* g_lastRebalanced = 0;
static void Rebalance(NodeBase* x, NodeBase*& root) { ++g_rebalances; g_lastRebalanced = x; (void)root; }

struct Tree {
    Node* header; unsigned int count;
    NodeBase*& root() { return header->parent; }
    NodeBase*& leftmost() { return header->left; }
    NodeBase*& rightmost() { return header->right; }
    static const CCharString& key(NodeBase* x) { return ((Node*)x)->value; }
    Node* create(const CCharString& v) { Node* n = (Node*)std::malloc(sizeof(Node)); new (&n->value) CCharString(v); return n; }
    NodeBase* insert(NodeBase* x, NodeBase* y, const CCharString& v, NodeBase* w)
    {
        Node* z;
        if (y == header || (w == 0 && (x != 0 || v < key(y)))) {
            z = create(v);
            y->left = z;
            if (y == header) { root() = z; rightmost() = z; }
            else if (y == leftmost()) leftmost() = z;
        }
        else {
            z = create(v);
            y->right = z;
            if (y == rightmost()) rightmost() = z;
        }
        z->parent = y; z->left = 0; z->right = 0;
        Rebalance(z, header->parent);
        ++count;
        return z;
    }
};

static unsigned char g_hdr[sizeof(Node)];

int main()
{
    Node& hdr = *(Node*)g_hdr; hdr.color = false; hdr.parent = 0; hdr.left = &hdr; hdr.right = &hdr;
    Tree t; t.header = &hdr; t.count = 0;
    CCharString m("m"), a("a"), z("z");
    NodeBase* r = t.insert(0, &hdr, m, 0);                       // empty tree: root
    if (hdr.parent != r || hdr.left != r || hdr.right != r || r->parent != &hdr || t.count != 1) { std::printf("004abc70_TEST FAIL\n"); return 1; }
    NodeBase* l = t.insert(0, r, a, 0);                          // a < m: left child, new leftmost
    if (r->left != l || hdr.left != l || hdr.right != r || l->parent != r || t.count != 2) { std::printf("004abc70_TEST FAIL\n"); return 1; }
    NodeBase* g = t.insert(0, r, z, 0);                          // z > m: right child, new rightmost
    if (r->right != g || hdr.right != g || hdr.left != l || t.count != 3) { std::printf("004abc70_TEST FAIL\n"); return 1; }
    if (g_rebalances != 3 || g_lastRebalanced != g || ((Node*)g)->value.p != z.p || g->left != 0 || g->right != 0) { std::printf("004abc70_TEST FAIL\n"); return 1; }
    NodeBase* h = t.insert(0, g, a, g);                          // a hint forces the right link
    if (g->right != h || hdr.right != h) { std::printf("004abc70_TEST FAIL\n"); return 1; }
    std::free(l); std::free(g); std::free(h); std::free(r);
    std::printf("004abc70_TEST PASS\n");
    return 0;
}