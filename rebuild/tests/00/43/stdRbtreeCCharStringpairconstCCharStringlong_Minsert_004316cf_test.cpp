// Behaviour test for _Rb_tree<CCharString, pair<const CCharString, long>>::_M_insert (0x004316CF): the new node hangs left of y
// when y is the header, when x is set, or when the key sorts before y's key (null strings first) unless w is set; the header's
// root / leftmost / rightmost follow; the count grows and the node gets y as parent and no children.
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CCharStringData { const char* Chars; unsigned long Length; };
static long Compare(const char* a, const char* b) { int r = std::strcmp(a, b); return r < 0 ? -1 : (r > 0 ? 1 : 0); }
class CCharString {
public:
    CCharStringData* PStringData;
    bool operator<(const CCharString& other) const
    {
        bool result;
        if (other.PStringData == PStringData) result = false;
        else if (!PStringData) result = true;
        else if (!other.PStringData) result = false;
        else result = Compare(PStringData->Chars, other.PStringData->Chars) < 0;
        return result;
    }
};
struct Value { CCharString first; long second; };
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { Value value; };
static int g_rebalances = 0;
static void Rebalance(NodeBase*, NodeBase*&) { ++g_rebalances; }
struct Tree {
    NodeBase* header; unsigned long count;
    Node* create(const Value& v) { Node* n = (Node*)std::malloc(sizeof(Node)); n->value = v; n->left = n->right = n->parent = (NodeBase*)1; return n; }
    NodeBase* insert(NodeBase* x_, NodeBase* y_, const Value& v, NodeBase* w_)
    {
        Node* w = (Node*)w_; Node* x = (Node*)x_; Node* y = (Node*)y_; Node* z;
        if (y == header || (w == 0 && (x != 0 || v.first < y->value.first))) {
            z = create(v); y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            z = create(v); y->right = z;
            if (y == header->right) header->right = z;
        }
        z->parent = y; z->left = 0; z->right = 0;
        Rebalance(z, header->parent);
        ++count;
        return z;
    }
};
int main()
{
    int bad = 0;
    NodeBase hdr; hdr.color = false; hdr.parent = 0; hdr.left = &hdr; hdr.right = &hdr;
    Tree t; t.header = &hdr; t.count = 0;
    CCharStringData dm = { "m", 1 }, da = { "a", 1 }, dz = { "z", 1 };
    Value vm = { { &dm }, 1 }, va = { { &da }, 2 }, vz = { { &dz }, 3 }, vnull = { { 0 }, 4 };
    NodeBase* m = t.insert(0, &hdr, vm, 0);                       // empty tree
    if (hdr.parent != m || hdr.left != m || hdr.right != m || m->parent != &hdr || m->left || m->right) bad = 1;
    NodeBase* a = t.insert(0, m, va, 0);                          // "a" < "m": left, new leftmost
    if (m->left != a || hdr.left != a || hdr.right != m || a->parent != m) bad = 1;
    NodeBase* z = t.insert(0, m, vz, 0);                          // "z" > "m": right, new rightmost
    if (m->right != z || hdr.right != z || hdr.left != a) bad = 1;
    NodeBase* n = t.insert(0, a, vnull, 0);                       // null key sorts first
    if (a->left != n || hdr.left != n) bad = 1;
    NodeBase* f = t.insert(0, z, vm, z);                          // w set: right regardless of order
    if (z->right != f || hdr.right != f) bad = 1;
    if (((Node*)f)->value.second != 1 || t.count != 5 || g_rebalances != 5) bad = 1;
    if (bad) { std::printf("004316cf_TEST FAIL\n"); return 1; }
    std::printf("004316cf_TEST PASS\n");
    return 0;
}