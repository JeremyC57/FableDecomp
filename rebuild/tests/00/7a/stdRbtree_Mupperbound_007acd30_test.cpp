// Behaviour test for 007acd30: STLport _Rb_tree::_M_upper_bound of a map keyed by CCharString -- the first node whose key sorts after k, else the header
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

struct CMappedValue { int Id; };
typedef Pair<CCharString, CMappedValue*> ValueType;
typedef Node<ValueType> MapNode;

class CCharStringMap {
public:
    MapNode* Header;
    unsigned int NodeCount;
    MapNode* UpperBound(const CCharString& k) const
    {
        MapNode* y = Header;
        MapNode* x = (MapNode*)Header->Parent;
        while (x != 0)
            if (k < x->Value.first)
                y = x, x = (MapNode*)x->Left;
            else
                x = (MapNode*)x->Right;
        return y;
    }
};

static bool NodeLess(NodeBase* a, NodeBase* b) { return ((MapNode*)a)->Value.first < ((MapNode*)b)->Value.first; }

int main()
{
    static char s0[] = "apple", s1[] = "banana", s2[] = "cherry", s3[] = "damson", s4[] = "elder", sq[] = "blue", sz[] = "zzz";
    CBasicString<char> b[5] = { { s0 }, { s1 }, { s2 }, { s3 }, { s4 } };
    CBasicString<char> bq = { sq }, bz = { sz };
    MapNode header;
    header.Parent = 0; header.Left = header.Right = &header;
    MapNode nodes[5];
    int order[5] = { 2, 0, 4, 1, 3 };
    for (int i = 0; i < 5; ++i) { nodes[order[i]].Value.first.PData = &b[order[i]]; BstInsert(&header, &nodes[order[i]], NodeLess); }
    CCharStringMap map;
    map.Header = &header; map.NodeCount = 5;
    CCharString k;
    bool ok = true;
    k.PData = &b[1]; ok = ok && map.UpperBound(k) == &nodes[2];      // "banana" -> "cherry"
    k.PData = &bq;   ok = ok && map.UpperBound(k) == &nodes[2];      // "blue" -> "cherry"
    k.PData = &b[4]; ok = ok && map.UpperBound(k) == &header;        // "elder" -> end
    k.PData = &bz;   ok = ok && map.UpperBound(k) == &header;
    k.PData = 0;     ok = ok && map.UpperBound(k) == &nodes[0];      // a null string sorts first
    header.Parent = 0;
    k.PData = &b[0]; ok = ok && map.UpperBound(k) == &header;        // empty tree
    if (!ok) { std::printf("007acd30_TEST FAIL\n"); return 1; }
    std::printf("007acd30_TEST PASS\n");
    return 0;
}