// Behaviour test for _STL::_Rb_tree<float, pair<const float, CCountedPointer<CVertexPairContraction> >, ...>::_M_insert (the cost queue multimap) @ 0x00AD4540
// Links a copy of v as the left or right child of y (left when y is the header, or w == 0 and (x != 0 or v < key(y))),
// keeps the header's root/leftmost/rightmost, rebalances and counts the node.
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

struct CInfoModel { int RefCount; };
static CInfoModel gInfo;
struct CPointerModel {
    void* Data;
    CInfoModel* Info;
    CPointerModel(CInfoModel* info) : Data(info), Info(info) { ++Info->RefCount; }
    CPointerModel(const CPointerModel& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CPointerModel() { if (Info) --Info->RefCount; }
};
struct CValue {
    float first;
    CPointerModel second;
    CValue(float k, const CPointerModel& p) : first(k), second(p) { }
    CValue(const CValue& o) : first(o.first), second(o.second) { }
};
typedef TTree<float, CValue> Tree;
static CValue MAKE(unsigned int k) { return CValue((float)k * 0.25f, CPointerModel(&gInfo)); }

int main()
{
    int ok = 1;
    {
        Tree tree;
        // empty tree: y == header -> the new node is root, leftmost and rightmost
        CNodeBase* a = tree.Insert(0, tree.Header, MAKE(50), 0);
        if (tree.Header->Parent != a || tree.Header->Left != a || tree.Header->Right != a || a->Parent != tree.Header
            || a->Left != 0 || a->Right != 0 || tree.Count != 1) ok = 0;
        // smaller key under the leftmost node, x == 0, w == 0 -> left child, new leftmost
        CNodeBase* b = tree.Insert(0, a, MAKE(20), 0);
        if (a->Left != b || b->Parent != a || tree.Header->Left != b || tree.Header->Right != a) ok = 0;
        // larger key under the rightmost node -> right child, new rightmost
        CNodeBase* c = tree.Insert(0, a, MAKE(80), 0);
        if (a->Right != c || c->Parent != a || tree.Header->Right != c || tree.Header->Left != b) ok = 0;
        // x != 0 forces the left side even for a larger key
        CNodeBase* d = tree.Insert(c, c, MAKE(90), 0);
        if (c->Left != d || tree.Header->Right != c) ok = 0;
        // w != 0 forces the right side even for a smaller key; rightmost is not touched for an inner node
        CNodeBase* e = tree.Insert(0, b, MAKE(10), b);
        if (b->Right != e || tree.Header->Left != b) ok = 0;
        if (tree.Count != 5 || gRebalances != 5) ok = 0;
                if (gInfo.RefCount != 5) ok = 0;
        float diff = ((Tree::Node*)e)->Value.first - 2.5f;
        if (diff > 0.0001f || diff < -0.0001f) ok = 0;
    }
        if (gInfo.RefCount != 0) ok = 0;
    if (!ok) { std::printf("00ad4540_TEST FAIL\n"); return 1; }
    std::printf("00ad4540_TEST PASS\n");
    return 0;
}