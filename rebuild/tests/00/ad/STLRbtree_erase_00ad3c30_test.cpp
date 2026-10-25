// Behaviour test for std::_Rb_tree<float, pair<const float, CCountedPointer<CVertexPairContraction> >, ...>::erase(iterator) @ 0x00AD3C30
// Unlinks the node through _Rb_tree_rebalance_for_erase(node, root, leftmost, rightmost), destroys the value (the counted
// pointer drops one reference and deletes the contraction on the last one), frees the node and decrements the count.
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

static int gDeletes = 0;
static int gRebalanceForErase = 0;
struct CInfoModel { int RefCount; void (*DeleteFunc)(void*); void* Data; };
struct CContractionModel { float Cost; };
static void DeleteContraction(void* data) { delete (CContractionModel*)data; ++gDeletes; }

struct CPointerModel {
    CContractionModel* Data;
    CInfoModel* Info;
    CPointerModel(CContractionModel* data, CInfoModel* info) : Data(data), Info(info) { if (Info) ++Info->RefCount; }
    CPointerModel(const CPointerModel& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CPointerModel()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; }
        }
        Data = 0;
        Info = 0;
    }
};
struct CValue {
    float first;
    CPointerModel second;
    CValue(float k, const CPointerModel& p) : first(k), second(p) { }
    CValue(const CValue& o) : first(o.first), second(o.second) { }
};
typedef TTree<float, CValue> Tree;

// Stand-in for _Rb_tree_rebalance_for_erase (0x0042A34E), enough for leaves: unlink z, fix leftmost/rightmost.
static CNodeBase* RebalanceForErase(CNodeBase* z, CNodeBase*& root, CNodeBase*& leftmost, CNodeBase*& rightmost)
{
    ++gRebalanceForErase;
    CNodeBase* p = z->Parent;
    if (root == z) { root = 0; leftmost = p; rightmost = p; return z; }
    if (p->Left == z) p->Left = 0; else p->Right = 0;
    if (leftmost == z) leftmost = p;
    if (rightmost == z) rightmost = p;
    return z;
}

static void Erase(Tree& tree, CNodeBase* position)
{
    Tree::Node* y = (Tree::Node*)RebalanceForErase(position, tree.Header->Parent, tree.Header->Left, tree.Header->Right);
    y->Value.~CValue();
    std::free(y);
    --tree.Count;
}

int main()
{
    int ok = 1;
    {
        Tree queue;
        CContractionModel* shared = new CContractionModel;
        CInfoModel* sharedInfo = new CInfoModel;
        sharedInfo->RefCount = 0; sharedInfo->DeleteFunc = DeleteContraction; sharedInfo->Data = shared;
        CContractionModel* lone = new CContractionModel;
        CInfoModel* loneInfo = new CInfoModel;
        loneInfo->RefCount = 0; loneInfo->DeleteFunc = DeleteContraction; loneInfo->Data = lone;
        CPointerModel keep(shared, sharedInfo);                        // an outside owner keeps `shared` alive
        CNodeBase* root = queue.Insert(0, queue.Header, CValue(1.0f, keep), 0);
        CNodeBase* low = queue.Insert(0, root, CValue(0.5f, keep), 0);
        CNodeBase* high = queue.Insert(0, root, CValue(2.0f, CPointerModel(lone, loneInfo)), 0);
        if (queue.Count != 3 || sharedInfo->RefCount != 3 || loneInfo->RefCount != 1) ok = 0;
        Erase(queue, high);                                            // last reference to `lone`: deleted
        if (queue.Count != 2 || gDeletes != 1 || queue.Header->Right != root || root->Right != 0) ok = 0;
        Erase(queue, low);                                             // `shared` still owned elsewhere
        if (queue.Count != 1 || gDeletes != 1 || sharedInfo->RefCount != 2 || queue.Header->Left != root) ok = 0;
        Erase(queue, root);
        if (queue.Count != 0 || queue.Header->Parent != 0 || sharedInfo->RefCount != 1 || gRebalanceForErase != 3) ok = 0;
    }
    if (gDeletes != 2) ok = 0;                                         // `keep` released the shared contraction
    if (!ok) { std::printf("00ad3c30_TEST FAIL\n"); return 1; }
    std::printf("00ad3c30_TEST PASS\n");
    return 0;
}