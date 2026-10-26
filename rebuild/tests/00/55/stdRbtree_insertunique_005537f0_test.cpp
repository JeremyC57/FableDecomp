// behaviour test for 0x005537F0 _Rb_tree<unsigned long, pair<unsigned long const, unsigned long >, _Select1st<...>, less<unsigned long > >::insert_unique(iterator position, const value_type& v): a hint that brackets v inserts next to it (begin / end / interior), an equal key at the hint returns the hint without inserting, a hint that does not bracket v falls back on the unhinted insert, and the tree stays ordered
#include <cstdio>
#include <cstdlib>

typedef unsigned long Key;
static char gBuf[128];
#define KEY(n) ((Key)(n))
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { Key first; int second; };
struct Iter {
    NodeBase* node;
    explicit Iter(NodeBase* n) : node(n) { }
    Iter& operator++()
    {
        if (node->right != 0) { node = node->right; while (node->left != 0) node = node->left; }
        else {
            NodeBase* y = node->parent;
            while (node == y->right) { node = y; y = y->parent; }
            if (node->right != y) node = y;
        }
        return *this;
    }
    Iter& operator--()
    {
        if (node->color == false && node->parent->parent == node) node = node->right;
        else if (node->left != 0) { NodeBase* y = node->left; while (y->right != 0) y = y->right; node = y; }
        else { NodeBase* y = node->parent; while (node == y->left) { node = y; y = y->parent; } node = y; }
        return *this;
    }
    bool operator==(const Iter& o) const { return node == o.node; }
    bool operator!=(const Iter& o) const { return node != o.node; }
};
struct Tree {
    NodeBase* header; unsigned int count;
    static const Key& KeyOf(NodeBase* x) { return ((Node*)x)->first; }
    Iter begin() { return Iter(header->left); }
    Iter end() { return Iter(header); }
    Iter Insert(NodeBase* x, NodeBase* y, Key k, int v, NodeBase* w)        // _M_insert (no rebalance: stays a search tree)
    {
        Node* z = (Node*)std::malloc(sizeof(Node)); z->first = k; z->second = v; z->color = true;
        if (y == header || (w == 0 && (x != 0 || k < KeyOf(y)))) {
            y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            y->right = z;
            if (y == header->right) header->right = z;
        }
        z->parent = y; z->left = 0; z->right = 0;
        ++count;
        return Iter(z);
    }
    Iter InsertUnique(Key k, int v)                                        // unhinted insert_unique(v).first
    {
        NodeBase* y = header; NodeBase* x = header->parent; bool comp = true;
        while (x != 0) { y = x; comp = k < KeyOf(x); x = comp ? x->left : x->right; }
        Iter j(y);
        if (comp) { if (j == begin()) return Insert(y, y, k, v, 0); --j; }
        if (KeyOf(j.node) < k) return Insert(x, y, k, v, 0);
        return j;
    }
    Iter InsertHint(Iter pos, Key k, int v)                               // the function under test (STLport 4.6 shape)
    {
        if (pos.node == header->left) {
            if (count <= 0) return InsertUnique(k, v);
            if (k < KeyOf(pos.node)) return Insert(pos.node, pos.node, k, v, 0);
            if (!(KeyOf(pos.node) < k)) return pos;
            Iter after = pos; ++after;
            if (after.node == header) return Insert(0, pos.node, k, v, pos.node);
            if (k < KeyOf(after.node)) {
                if (pos.node->right == 0) return Insert(0, pos.node, k, v, pos.node);
                return Insert(after.node, after.node, k, v, 0);
            }
            return InsertUnique(k, v);
        }
        if (pos.node == header) {
            if (KeyOf(header->right) < k) return Insert(0, header->right, k, v, pos.node);
            return InsertUnique(k, v);
        }
        Iter before = pos; --before;
        bool compVPos = k < KeyOf(pos.node);
        if (compVPos && KeyOf(before.node) < k) {
            if (before.node->right == 0) return Insert(0, before.node, k, v, before.node);
            return Insert(pos.node, pos.node, k, v, 0);
        }
        Iter after = pos; ++after;
        bool compPosV = !compVPos;
        if (!compVPos) compPosV = KeyOf(pos.node) < k;
        if (!compVPos && compPosV && (after.node == header || k < KeyOf(after.node))) {
            if (pos.node->right == 0) return Insert(0, pos.node, k, v, pos.node);
            return Insert(after.node, after.node, k, v, 0);
        }
        if (compVPos == compPosV) return pos;
        return InsertUnique(k, v);
    }
};
int main()
{
    int fail = 0;
    Tree t;
    NodeBase* h = (NodeBase*)std::malloc(sizeof(Node));
    h->color = false; h->parent = 0; h->left = h; h->right = h;
    t.header = h; t.count = 0;
    Iter r = t.InsertHint(t.begin(), KEY(50), 5);                 // empty tree: falls back
    if (Tree::KeyOf(r.node) != KEY(50) || t.count != 1) fail |= 1;
    r = t.InsertHint(t.begin(), KEY(10), 1);                      // begin hint, smaller key
    if (Tree::KeyOf(r.node) != KEY(10) || t.count != 2) fail |= 2;
    r = t.InsertHint(t.end(), KEY(90), 9);                        // end hint, larger key
    if (Tree::KeyOf(r.node) != KEY(90) || t.count != 3) fail |= 4;
    Iter p = t.begin(); ++p;                                      // 50
    r = t.InsertHint(p, KEY(30), 3);                              // interior hint bracketing 30
    if (Tree::KeyOf(r.node) != KEY(30) || t.count != 4) fail |= 8;
    p = t.begin(); ++p;                                           // 30
    r = t.InsertHint(p, KEY(30), 7);                              // equal key: the hint comes back
    if (r != p || ((Node*)r.node)->second != 3 || t.count != 4) fail |= 16;
    r = t.InsertHint(t.begin(), KEY(70), 7);                      // wrong hint: falls back
    if (Tree::KeyOf(r.node) != KEY(70) || t.count != 5) fail |= 32;
    p = t.begin(); ++p; ++p; ++p;                                 // 70
    r = t.InsertHint(p, KEY(80), 8);                              // hint just before v: inserted after it
    if (Tree::KeyOf(r.node) != KEY(80) || t.count != 6) fail |= 64;
    static const int want[6] = { 10, 30, 50, 70, 80, 90 };
    int n = 0;
    for (Iter it = t.begin(); it != t.end(); ++it, ++n)
        if (n >= 6 || Tree::KeyOf(it.node) != KEY(want[n]) || ((Node*)it.node)->second != want[n] / 10) fail |= 128;
    if (n != 6) fail |= 256;
    if (fail) { std::printf("005537f0_TEST FAIL %d\n", fail); return 1; }
    std::printf("005537f0_TEST PASS\n");
    return 0;
}