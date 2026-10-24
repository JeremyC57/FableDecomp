// Behaviour test for 0x004754C6 _Rb_tree<CCharString, pair<const CCharString, long>>::_M_upper_bound (a model tree in STLport's header layout)
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct StrData { const char* text; };
struct Key {                                        // CCharString: one shared-data pointer, null sorts first
    StrData* data;
    bool operator<(const Key& o) const
    {
        if (o.data == data) return false;
        if (data == 0) return true;
        if (o.data == 0) return false;
        return std::strcmp(data->text, o.data->text) < 0;
    }
};
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { Key key; int value; };
static NodeBase* Increment(NodeBase* x)
{
    if (x->right) { x = x->right; while (x->left) x = x->left; return x; }
    NodeBase* y = x->parent; while (x == y->right) { x = y; y = y->parent; } if (x->right != y) x = y; return x;
}
static NodeBase* Decrement(NodeBase* x)
{
    if (x->color == 0 && x->parent->parent == x) return x->right;           // header (red, colour 0 here)
    if (x->left) { NodeBase* y = x->left; while (y->right) y = y->right; return y; }
    NodeBase* y = x->parent; while (x == y->left) { x = y; y = y->parent; } return y;
}
struct Tree {
    NodeBase* header; unsigned int count; int created;
    void Init() { header = (NodeBase*)std::malloc(sizeof(NodeBase)); header->color = 0; header->parent = 0; header->left = header->right = header; count = 0; created = 0; }
    static const Key& K(NodeBase* x) { return ((Node*)x)->key; }
    NodeBase* LowerBound(const Key& k) const
    {
        NodeBase* y = header; NodeBase* x = header->parent;
        while (x) if (!(K(x) < k)) { y = x; x = x->left; } else x = x->right;
        return y;
    }
    NodeBase* UpperBound(const Key& k) const
    {
        NodeBase* y = header; NodeBase* x = header->parent;
        while (x) if (k < K(x)) { y = x; x = x->left; } else x = x->right;
        return y;
    }
    NodeBase* Find(const Key& k) const
    {
        NodeBase* y = LowerBound(k);
        if (y == header || k < K(y)) y = header;
        return y;
    }
    Node* CreateNode(const Key& k, int v) { ++created; Node* n = (Node*)std::malloc(sizeof(Node)); n->color = 1; n->key = k; n->value = v; return n; }
    NodeBase* InsertAt(NodeBase* x, NodeBase* y, const Key& k, int v, NodeBase* w)
    {
        Node* z;
        if (y == header || (w == 0 && (x != 0 || k < K(y)))) {
            z = CreateNode(k, v); y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            z = CreateNode(k, v); y->right = z;
            if (y == header->right) header->right = z;
        }
        z->parent = y; z->left = 0; z->right = 0;                           // (no rebalance: still a valid search tree)
        ++count;
        return z;
    }
    NodeBase* InsertUnique(const Key& k, int v, bool* inserted)
    {
        NodeBase* y = header; NodeBase* x = header->parent; bool comp = true;
        while (x) { y = x; comp = k < K(x); x = comp ? x->left : x->right; }
        NodeBase* j = y;
        if (comp) { if (j == header->left) { *inserted = true; return InsertAt(y, y, k, v, 0); } else j = Decrement(j); }
        if (K(j) < k) { *inserted = true; return InsertAt(x, y, k, v, 0); }
        *inserted = false; return j;
    }
};
static StrData S_apple = { "apple" }, S_kiwi = { "kiwi" }, S_mango = { "mango" }, S_pear = { "pear" }, S_fig = { "fig" };
static Key MakeKey(StrData* d) { Key k; k.data = d; return k; }
static bool InOrder(Tree& t, const char* expect)
{
    char buf[128]; int len = 0; buf[0] = 0;
    for (NodeBase* n = t.header->left; n != t.header; n = Increment(n))
        len += std::sprintf(buf + len, "%s,", ((Node*)n)->key.data ? ((Node*)n)->key.data->text : "<null>");
    return std::strcmp(buf, expect) == 0;
}
static void Fill(Tree& t)
{
    bool ins;
    t.Init();
    t.InsertUnique(MakeKey(&S_mango), 1, &ins); t.InsertUnique(MakeKey(&S_apple), 2, &ins);
    t.InsertUnique(MakeKey(&S_pear), 3, &ins);  t.InsertUnique(MakeKey(&S_fig), 4, &ins);
}
int main()
{
    Tree t; Fill(t);
    Key kiwi = MakeKey(&S_kiwi), fig = MakeKey(&S_fig), pear = MakeKey(&S_pear), zed = MakeKey(0);
    bool ok = ((Node*)t.UpperBound(kiwi))->key.data == &S_mango && ((Node*)t.UpperBound(fig))->key.data == &S_mango
        && t.UpperBound(pear) == t.header && ((Node*)t.UpperBound(zed))->key.data == &S_apple;
    if (!ok) { std::printf("004754c6_TEST FAIL\n"); return 1; }
    std::printf("004754c6_TEST PASS\n");
    return 0;
}