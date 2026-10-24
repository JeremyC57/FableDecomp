// Behaviour test for 0x004636CF _Rb_tree<CCharString, pair<const CCharString,long>, ...>::insert_equal: the walk goes left
// when the new key is less than the node key (CCharString order: same block -> not less, null block sorts first, then
// signed character compare) and right otherwise (equal keys go right); the new node is linked where the walk ended
#include <cstdio>
#include <cstring>
struct CCharStringData { const char* Chars; unsigned long Length; };
static long Compare(const char* a, const char* b)
{
    for (;;) { char ca = *a; if (ca == 0 && *b == 0) return 0; char cb = *b; if (ca < cb) return -1; if (ca > cb) return 1; ++a; ++b; }
}
struct CCharString {
    CCharStringData* PStringData;
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData) return false;
        if (PStringData == 0) return true;
        if (other.PStringData == 0) return false;
        return Compare(PStringData->Chars, other.PStringData->Chars) < 0;
    }
};
struct Value { CCharString first; long second; };
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { Value v; };
struct Tree {
    Node* header; unsigned long count;
    Node pool[16]; int used;
    NodeBase* Insert(NodeBase* x, NodeBase* y, const Value& v)   // minimal _M_insert: no rebalancing
    {
        Node* z = &pool[used++]; z->v = v; z->left = z->right = 0; z->parent = y;
        if (y == header) { header->parent = z; header->left = z; header->right = z; }
        else if (v.first < ((Node*)y)->v.first) y->left = z; else y->right = z;
        (void)x; ++count; return z;
    }
    NodeBase* InsertEqual(const Value& v)
    {
        Node* y = header; Node* x = (Node*)header->parent;
        while (x != 0) { y = x; x = (Node*)((v.first < x->v.first) ? x->left : x->right); }
        return Insert(x, y, v);
    }
};
int main()
{
    static Node head; head.parent = 0; head.left = head.right = &head;
    static Tree t; t.header = &head; t.count = 0; t.used = 0;
    CCharStringData m = { "m", 1 }, a = { "a", 1 }, z = { "z", 1 }, m2 = { "m", 1 };
    Value vm = { { &m }, 1 }, va = { { &a }, 2 }, vz = { { &z }, 3 }, vm2 = { { &m2 }, 4 }, vnull = { { 0 }, 5 }, vsame = { { &m }, 6 };
    NodeBase* nm = t.InsertEqual(vm);
    NodeBase* na = t.InsertEqual(va);
    NodeBase* nz = t.InsertEqual(vz);
    NodeBase* nm2 = t.InsertEqual(vm2);     // equal text: goes right of m, left of z
    NodeBase* nn = t.InsertEqual(vnull);    // null block sorts first: left of a
    NodeBase* ns = t.InsertEqual(vsame);    // same block: not less, right of m, then left of z, right of m2
    bool ok = head.parent == nm && nm->left == na && nm->right == nz && nz->left == nm2 && na->left == nn && nm2->right == ns;
    ok = ok && t.count == 6 && ((Node*)ns)->v.second == 6;
    if (!ok) { std::printf("004636cf_TEST FAIL\n"); return 1; }
    std::printf("004636cf_TEST PASS\n");
    return 0;
}