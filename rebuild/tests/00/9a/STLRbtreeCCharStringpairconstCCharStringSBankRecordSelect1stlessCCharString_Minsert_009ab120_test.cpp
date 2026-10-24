// Behaviour test for 0x009AB120 _Rb_tree<CCharString, pair<const CCharString, bank record> >::_M_insert: nodes are linked in name order and the value (name, four wide strings, word) is copied into the node; the tree stays valid.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
// STLport 4.6 red-black tree core (as Fable.exe ships it): header node {color, parent=root, left=leftmost, right=rightmost}.
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
static const bool RED = false, BLACK = true;
static void rotate_left(NodeBase* x, NodeBase*& root)
{
    NodeBase* y = x->right; x->right = y->left; if (y->left) y->left->parent = x; y->parent = x->parent;
    if (x == root) root = y; else if (x == x->parent->left) x->parent->left = y; else x->parent->right = y;
    y->left = x; x->parent = y;
}
static void rotate_right(NodeBase* x, NodeBase*& root)
{
    NodeBase* y = x->left; x->left = y->right; if (y->right) y->right->parent = x; y->parent = x->parent;
    if (x == root) root = y; else if (x == x->parent->right) x->parent->right = y; else x->parent->left = y;
    y->right = x; x->parent = y;
}
static void Rebalance(NodeBase* x, NodeBase*& root)                       // _Rb_global<bool>::_Rebalance
{
    x->color = RED;
    while (x != root && x->parent->color == RED) {
        if (x->parent == x->parent->parent->left) {
            NodeBase* y = x->parent->parent->right;
            if (y && y->color == RED) { x->parent->color = BLACK; y->color = BLACK; x->parent->parent->color = RED; x = x->parent->parent; }
            else { if (x == x->parent->right) { x = x->parent; rotate_left(x, root); }
                   x->parent->color = BLACK; x->parent->parent->color = RED; rotate_right(x->parent->parent, root); }
        } else {
            NodeBase* y = x->parent->parent->left;
            if (y && y->color == RED) { x->parent->color = BLACK; y->color = BLACK; x->parent->parent->color = RED; x = x->parent->parent; }
            else { if (x == x->parent->left) { x = x->parent; rotate_right(x, root); }
                   x->parent->color = BLACK; x->parent->parent->color = RED; rotate_left(x->parent->parent, root); }
        }
    }
    root->color = BLACK;
}
static NodeBase* Decrement(NodeBase* p)                                     // iterator::operator-- @ 0x009AA3E0
{
    if (p->color == RED && p->parent->parent == p) return p->right;
    if (p->left) { NodeBase* q = p->left; while (q->right) q = q->right; return q; }
    NodeBase* q = p->parent; while (p == q->left) { p = q; q = q->parent; } return q;
}
static NodeBase* Increment(NodeBase* p)
{
    if (p->right) { p = p->right; while (p->left) p = p->left; return p; }
    NodeBase* q = p->parent; while (p == q->right) { p = q; q = q->parent; } if (p->right != q) p = q; return p;
}
template <class V> struct Node : NodeBase { V value; };
template <class K, class V, class KeyOf, class Less> struct Tree {
    NodeBase* header; unsigned int count;
    Tree() : count(0) { header = (NodeBase*)std::malloc(sizeof(Node<V>)); header->color = RED; header->parent = 0; header->left = header->right = header; }
    static const K& key(NodeBase* n) { return KeyOf()(((Node<V>*)n)->value); }
    Node<V>* create(const V& v) { Node<V>* z = (Node<V>*)std::malloc(sizeof(Node<V>)); new (&z->value) V(v); return z; }
    NodeBase* _M_insert(NodeBase* x, NodeBase* y, const V& v, NodeBase* w = 0)
    {
        NodeBase* z;
        if (y == header || (w == 0 && (x != 0 || Less()(KeyOf()(v), key(y))))) {
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
    NodeBase* insert_unique(const V& v, bool& inserted)
    {
        NodeBase* y = header; NodeBase* x = header->parent; bool comp = true;
        while (x) { y = x; comp = Less()(KeyOf()(v), key(x)); x = comp ? x->left : x->right; }
        NodeBase* j = y;
        if (comp) { if (j == header->left) { inserted = true; return _M_insert(y, y, v); } j = Decrement(j); }
        if (Less()(key(j), KeyOf()(v))) { inserted = true; return _M_insert(x, y, v); }
        inserted = false; return j;
    }
    NodeBase* _M_find(const K& k) const
    {
        NodeBase* y = header; NodeBase* x = header->parent;
        while (x) if (!Less()(key(x), k)) y = x, x = x->left; else x = x->right;
        if (y == header || Less()(k, key(y))) y = header;
        return y;
    }
    // invariants: in-order keys strictly increasing, count matches, leftmost/rightmost right, root black, no red-red
    int check_rb(NodeBase* n, bool& ok) const
    {
        if (!n) return 1;
        if (n->color == RED && ((n->left && n->left->color == RED) || (n->right && n->right->color == RED))) ok = false;
        if (n->left && n->left->parent != n) ok = false;
        if (n->right && n->right->parent != n) ok = false;
        int a = check_rb(n->left, ok), b = check_rb(n->right, ok);
        if (a != b) ok = false;
        return a + (n->color == BLACK ? 1 : 0);
    }
    bool valid() const
    {
        bool ok = true;
        if (count == 0) return header->parent == 0 && header->left == header && header->right == header;
        if (header->parent->color != BLACK || header->parent->parent != header) return false;
        check_rb(header->parent, ok);
        unsigned int n = 0; NodeBase* prev = 0;
        for (NodeBase* it = header->left; it != header; it = Increment(it)) {
            if (prev && !Less()(key(prev), key(it))) ok = false;
            prev = it; ++n;
        }
        NodeBase* lm = header->parent; while (lm->left) lm = lm->left;
        NodeBase* rm = header->parent; while (rm->right) rm = rm->right;
        return ok && n == count && lm == header->left && rm == header->right;
    }
};
template <class T> struct less { bool operator()(const T& a, const T& b) const { return a < b; } };
template <class T> struct Identity { const T& operator()(const T& x) const { return x; } };
template <class P> struct Select1st { const typename P::first_type& operator()(const P& x) const { return x.first; } };
template <class T1, class T2> struct pair {
    typedef T1 first_type;
    T1 first; T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
#define FAIL(a) { std::printf(a "_TEST FAIL\n"); return 1; }
class CCharString {                                 // null block = empty string, sorts first
public:
    const char* PStringData;
    CCharString(const char* t) : PStringData(t && *t ? t : 0) {}
    CCharString(const CCharString& o) : PStringData(o.PStringData) {}
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData) return false;
        if (PStringData == 0) return true;
        if (other.PStringData == 0) return false;
        return std::strcmp(PStringData, other.PStringData) < 0;
    }
};
static const char* N[8] = { "music", "", "speech", "fx", "ambient", "ui", "dialogue", "zz" };
#include <cwchar>
struct SBankRecord { const wchar_t* Strings[4]; int Value; };
int main()
{
    typedef pair<const CCharString, SBankRecord> V;
    typedef Tree<CCharString, V, Select1st<V>, less<CCharString> > T;
    T t; bool ins;
    for (int i = 0; i < 8; ++i) { SBankRecord r; r.Strings[0] = L"a"; r.Strings[1] = L"b"; r.Strings[2] = L"c"; r.Strings[3] = L"d"; r.Value = 100 + i;
        t.insert_unique(V(CCharString(N[i]), r), ins); if (!ins || !t.valid()) FAIL("009ab120") }
    for (int i = 0; i < 8; ++i) { NodeBase* n = t._M_find(CCharString(N[i])); if (n == t.header || ((Node<V>*)n)->value.second.Value != 100 + i || std::wcscmp(((Node<V>*)n)->value.second.Strings[3], L"d") != 0) FAIL("009ab120") }
    SBankRecord r2; r2.Value = 7; t.insert_unique(V(CCharString("music"), r2), ins); if (ins || t.count != 8) FAIL("009ab120")
    std::printf("009ab120_TEST PASS\n");
    return 0;
}