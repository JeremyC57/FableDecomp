// Behaviour test for std::_Rb_tree<pair<long,long>, pair<const pair<long,long>, CCountedPointer<CVertexPairContraction> >, ...>::insert_unique(const value_type&) @ 0x00AD4720
// Walks down to the insertion point, steps back one node (operator--) when the walk ended on a left turn, and inserts
// only when that predecessor's key is less than v's key; returns (node, true) or (existing node, false).
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
struct CInfoModel { int RefCount; };
static CInfoModel gInfo;
struct CValue {
    CKey first;
    CInfoModel* second;
    CValue(const CKey& k) : first(k), second(&gInfo) { ++gInfo.RefCount; }
    CValue(const CValue& o) : first(o.first), second(o.second) { ++gInfo.RefCount; }
    ~CValue() { --gInfo.RefCount; }
};
typedef TTree<CKey, CValue> Tree;

static CNodeBase* Decrement(CNodeBase* node)       // STLport _Rb_tree_base_iterator::_M_decrement
{
    if (node->Color == false && node->Parent->Parent == node)
        node = node->Right;
    else if (node->Left != 0) {
        CNodeBase* y = node->Left;
        while (y->Right != 0) y = y->Right;
        node = y;
    }
    else {
        CNodeBase* y = node->Parent;
        while (node == y->Left) { node = y; y = y->Parent; }
        node = y;
    }
    return node;
}

struct CInsertResult { CNodeBase* first; bool second; };

static CInsertResult InsertUnique(Tree& tree, const CValue& v)
{
    CNodeBase* y = tree.Header;
    CNodeBase* x = tree.Header->Parent;
    bool comp = true;
    while (x != 0) {
        y = x;
        comp = v.first < Tree::Key(x);
        x = comp ? x->Left : x->Right;
    }
    CNodeBase* j = y;
    CInsertResult result;
    if (comp) {
        if (j == tree.Header->Left) {
            result.first = tree.Insert(y, y, v, 0);
            result.second = true;
            return result;
        }
        j = Decrement(j);
    }
    if (Tree::Key(j) < v.first) {
        result.first = tree.Insert(x, y, v, 0);
        result.second = true;
        return result;
    }
    result.first = j;
    result.second = false;
    return result;
}

static int InOrderSorted(const CNodeBase* n, const CNodeBase** prev)
{
    if (!n) return 1;
    if (!InOrderSorted(n->Left, prev)) return 0;
    if (*prev && !(Tree::Key(*prev) < Tree::Key(n))) return 0;
    *prev = n;
    return InOrderSorted(n->Right, prev);
}

int main()
{
    int ok = 1;
    {
        Tree tree;
        static const int keys[9][2] = { { 5, 5 }, { 2, 0 }, { 8, 1 }, { 5, 4 }, { 5, 6 }, { 1, 1 }, { 9, 9 }, { 2, 1 }, { 7, 7 } };
        for (int i = 0; i < 9; ++i) {
            CInsertResult r = InsertUnique(tree, CValue(CKey(keys[i][0], keys[i][1])));
            if (!r.second || Tree::Key(r.first).first != keys[i][0] || Tree::Key(r.first).second != keys[i][1]) ok = 0;
        }
        if (tree.Count != 9 || gInfo.RefCount != 9) ok = 0;
        for (int i = 0; i < 9; ++i) {                                  // duplicates: found, not inserted
            CInsertResult r = InsertUnique(tree, CValue(CKey(keys[i][0], keys[i][1])));
            if (r.second || Tree::Key(r.first).first != keys[i][0] || Tree::Key(r.first).second != keys[i][1]) ok = 0;
        }
        if (tree.Count != 9 || gInfo.RefCount != 9) ok = 0;
        const CNodeBase* prev = 0;
        if (!InOrderSorted(tree.Header->Parent, &prev)) ok = 0;
        if (Tree::Key(tree.Header->Left).first != 1 || Tree::Key(tree.Header->Right).first != 9) ok = 0;
    }
    if (gInfo.RefCount != 0) ok = 0;
    if (!ok) { std::printf("00ad4720_TEST FAIL\n"); return 1; }
    std::printf("00ad4720_TEST PASS\n");
    return 0;
}