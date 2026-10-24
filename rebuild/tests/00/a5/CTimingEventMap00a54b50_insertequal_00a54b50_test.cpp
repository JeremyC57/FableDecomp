// Behaviour test for 00a54b50: STLport _Rb_tree::insert_equal of a multimap<CCharString, float> -- descend to the leaf where v sorts (equal keys to the right) and hand (x = 0, y = leaf parent, v) to _M_insert
#include <cstdio>

template <class T> class CBasicString {
public:
    T* m_pData;
    long Compare(const CBasicString& other) const
    {
        const T* b = other.m_pData;
        const T* a = m_pData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    bool operator<(const CBasicString& other) const { return Compare(other) < 0; }
};
class CCharString {
public:
    CBasicString<char>* PData;
    bool operator<(const CCharString& other) const
    {
        if (other.PData == PData)
            return false;
        if (PData == 0)
            return true;
        if (other.PData == 0)
            return false;
        return *PData < *other.PData;
    }
};
struct NodeBase {
    bool      Color;
    NodeBase* Parent;
    NodeBase* Left;
    NodeBase* Right;
};
template <class V> struct Node : NodeBase {
    V Value;
};
template <class T1, class T2> struct Pair {
    T1 first;
    T2 second;
};
// plain BST insertion used to build the fixture (equal keys go right, as insert_equal does)
static void BstInsert(NodeBase* header, NodeBase* n, bool (*less)(NodeBase*, NodeBase*))
{
    n->Left = n->Right = 0;
    NodeBase* y = header;
    NodeBase* x = header->Parent;
    bool goLeft = true;
    while (x) { y = x; goLeft = less(n, x); x = goLeft ? x->Left : x->Right; }
    n->Parent = y;
    if (y == header) header->Parent = n;
    else if (goLeft) y->Left = n;
    else y->Right = n;
}

typedef Pair<CCharString, float> ValueType;
typedef Node<ValueType> MapNode;

static NodeBase* g_insX = (NodeBase*)1;
static NodeBase* g_insY = 0;
static const ValueType* g_insV = 0;

class CMultimap {
public:
    struct iterator {
        NodeBase* Node_;
        iterator(NodeBase* x) : Node_(x) { }
        iterator(const iterator& it) : Node_(it.Node_) { }
    };
    MapNode* Header;
    unsigned int NodeCount;
    iterator Insert(NodeBase* x, NodeBase* y, const ValueType& v, NodeBase* w = 0)   // retail _M_insert (stub: record the arguments)
    {
        (void)w;
        g_insX = x; g_insY = y; g_insV = &v;
        return iterator(y);
    }
    iterator InsertEqual(const ValueType& v)
    {
        NodeBase* y = Header;
        NodeBase* x = Header->Parent;
        while (x != 0) {
            y = x;
            x = (v.first < ((MapNode*)x)->Value.first) ? x->Left : x->Right;
        }
        return Insert(x, y, v);
    }
};

static bool NodeLess(NodeBase* a, NodeBase* b) { return ((MapNode*)a)->Value.first < ((MapNode*)b)->Value.first; }

int main()
{
    static char s0[] = "b", s1[] = "d", s2[] = "f", sa[] = "a", sc[] = "c", sz[] = "z";
    CBasicString<char> b[3] = { { s0 }, { s1 }, { s2 } };
    CBasicString<char> ba = { sa }, bc = { sc }, bz = { sz };
    MapNode header;
    header.Parent = 0; header.Left = header.Right = &header;
    CMultimap m;
    m.Header = &header; m.NodeCount = 0;
    ValueType v;
    v.first.PData = &ba; v.second = (float)1;
    CMultimap::iterator it = m.InsertEqual(v);                 // empty tree: parent is the header
    bool ok = g_insX == 0 && g_insY == &header && g_insV == &v && it.Node_ == &header;
    MapNode nodes[3];
    int order[3] = { 1, 0, 2 };
    for (int i = 0; i < 3; ++i) { nodes[order[i]].Value.first.PData = &b[order[i]]; BstInsert(&header, &nodes[order[i]], NodeLess); }
    v.first.PData = &bc; m.InsertEqual(v); ok = ok && g_insX == 0 && g_insY == &nodes[0];   // "c" goes right of "b"
    v.first.PData = &ba; m.InsertEqual(v); ok = ok && g_insY == &nodes[0];                  // "a" goes left of "b"
    v.first.PData = &b[1]; m.InsertEqual(v); ok = ok && g_insY == &nodes[2];                // equal "d" goes right, under "f"
    v.first.PData = &bz; m.InsertEqual(v); ok = ok && g_insY == &nodes[2];
    if (!ok) { std::printf("00a54b50_TEST FAIL\n"); return 1; }
    std::printf("00a54b50_TEST PASS\n");
    return 0;
}