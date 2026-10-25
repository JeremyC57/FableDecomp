// Behaviour test for std::_Rb_tree<pair<long,long>, pair<const pair<long,long>, CCountedPointer<CVertexPairContraction> >, ...>::_M_lower_bound @ 0x00AD24F0
// Returns the first node whose key is not less than k (lexicographic pair order), or the header when there is none.
#include <cstdio>
#include <cstdlib>
#include <new>

struct CNodeBase {
    bool Color;
    CNodeBase* Parent;
    CNodeBase* Left;
    CNodeBase* Right;
};

static int gRebalances = 0;
static void Rebalance(CNodeBase* x, CNodeBase*& root) { x->Color = true; (void)root; ++gRebalances; }   // stand-in for 0x0042971D (no rotations; nodes black)

template <class K, class V> struct TNode : public CNodeBase { V Value; };

// Tree with an STLport header: Parent = root, Left = leftmost, Right = rightmost.
template <class K, class V> struct TTree {
    typedef TNode<K, V> Node;
    CNodeBase* Header;
    unsigned int Count;
    TTree() : Count(0)
    {
        Header = (CNodeBase*)std::malloc(sizeof(Node));
        Header->Color = false;
        Header->Parent = 0;
        Header->Left = Header;
        Header->Right = Header;
    }
    static const K& Key(const CNodeBase* n) { return ((const Node*)n)->Value.first; }
    Node* CreateNode(const V& v)
    {
        Node* z = (Node*)std::malloc(sizeof(Node));
        new (&z->Value) V(v);
        return z;
    }
    // STLport _Rb_tree::_M_insert
    CNodeBase* Insert(CNodeBase* x, CNodeBase* y, const V& v, CNodeBase* w)
    {
        Node* z;
        if (y == Header || (w == 0 && (x != 0 || v.first < Key(y)))) {
            z = CreateNode(v);
            y->Left = z;
            if (y == Header) {
                Header->Parent = z;
                Header->Right = z;
            }
            else if (y == Header->Left)
                Header->Left = z;
        }
        else {
            z = CreateNode(v);
            y->Right = z;
            if (y == Header->Right)
                Header->Right = z;
        }
        z->Parent = y;
        z->Left = 0;
        z->Right = 0;
        Rebalance(z, Header->Parent);
        ++Count;
        return z;
    }
    static void FreeNodes(CNodeBase* n)
    {
        if (!n) return;
        FreeNodes(n->Left);
        FreeNodes(n->Right);
        ((Node*)n)->Value.~V();
        std::free(n);
    }
    ~TTree() { FreeNodes(Header->Parent); std::free(Header); }
};

struct CKey {
    int first;
    int second;
    CKey(int a, int b) : first(a), second(b) { }
};
static bool operator<(const CKey& x, const CKey& y)
{
    return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}
struct CValue {
    CKey first;
    int second;
    CValue(const CKey& k, int v) : first(k), second(v) { }
};
typedef TTree<CKey, CValue> Tree;

static CNodeBase* LowerBound(const Tree& tree, const CKey& k)
{
    CNodeBase* y = tree.Header;            // last node which is not less than k
    CNodeBase* x = tree.Header->Parent;    // current node
    while (x != 0)
        if (!(Tree::Key(x) < k))
            y = x, x = x->Left;
        else
            x = x->Right;
    return y;
}

int main()
{
    int ok = 1;
    Tree tree;
    if (LowerBound(tree, CKey(1, 1)) != tree.Header) ok = 0;
    // root (3,5); left (1,9) with right child (3,1); right (7,0) with left child (5,2)
    CNodeBase* r = tree.Insert(0, tree.Header, CValue(CKey(3, 5), 1), 0);
    CNodeBase* a = tree.Insert(0, r, CValue(CKey(1, 9), 2), 0);
    CNodeBase* b = tree.Insert(0, a, CValue(CKey(3, 1), 3), a);
    CNodeBase* c = tree.Insert(0, r, CValue(CKey(7, 0), 4), 0);
    CNodeBase* d = tree.Insert(c, c, CValue(CKey(5, 2), 5), 0);
    if (LowerBound(tree, CKey(0, 0)) != a) ok = 0;
    if (LowerBound(tree, CKey(1, 9)) != a) ok = 0;
    if (LowerBound(tree, CKey(1, 10)) != b) ok = 0;
    if (LowerBound(tree, CKey(3, 1)) != b) ok = 0;
    if (LowerBound(tree, CKey(3, 2)) != r) ok = 0;
    if (LowerBound(tree, CKey(3, 6)) != d) ok = 0;
    if (LowerBound(tree, CKey(6, -100)) != c) ok = 0;
    if (LowerBound(tree, CKey(7, 1)) != tree.Header) ok = 0;
    if (!ok) { std::printf("00ad24f0_TEST FAIL\n"); return 1; }
    std::printf("00ad24f0_TEST PASS\n");
    return 0;
}