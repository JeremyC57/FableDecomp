#include <cstdio>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
// shared behaviour model: STLport-shaped _Rb_tree<long, pair<const long, CFileBlock>> as reconstructed for 0x00BE05D0 /
// 0x00BE0A50 / 0x00BE0B20 (the external _Rebalance / _Rebalance_for_erase are test stand-ins: no recolouring, plain BST splice)
class CCPPointerInfo { public: long RefCount; void (__fastcall* DeleteFunction)(void* data); void* Data; };
static int g_deleted = 0;
static void __fastcall CountDelete(void*) { ++g_deleted; }
template <class T> class CCountedPointer {
public:
    T* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* d, CCPPointerInfo* i) : Data(d), Info(i) { if (Info) ++Info->RefCount; }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    void DeleteData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunction(Info->Data);
                operator delete(Info);
            }
        }
    }
    void Reset() { DeleteData(); Info = 0; Data = 0; }
    ~CCountedPointer() { DeleteData(); Data = 0; Info = 0; }
};
class CBankFileAsyncData { public: int dummy; };
class CBankFileAsyncEntry { public: int dummy; };
struct CFileBlock { long RefCount; bool LowPriority; CCountedPointer<CBankFileAsyncData> LoadingData; };
struct Value { long first; CFileBlock second; };
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { Value value; };

static void Rebalance(NodeBase*, NodeBase*&) { }
static NodeBase* RebalanceForErase(NodeBase* z, NodeBase*& root, NodeBase*& leftmost, NodeBase*& rightmost)
{
    NodeBase* y = z; NodeBase* x = 0;
    if (y->left == 0) x = y->right;
    else if (y->right == 0) x = y->left;
    else { y = y->right; while (y->left) y = y->left; x = y->right; }
    if (y != z) {
        z->left->parent = y; y->left = z->left;
        if (y != z->right) { if (x) x->parent = y->parent; y->parent->left = x; y->right = z->right; z->right->parent = y; }
        if (root == z) root = y; else if (z->parent->left == z) z->parent->left = y; else z->parent->right = y;
        y->parent = z->parent; y = z;
    } else {
        if (x) x->parent = y->parent;
        if (root == z) root = x; else if (z->parent->left == z) z->parent->left = x; else z->parent->right = x;
        if (leftmost == z) { if (z->right == 0) leftmost = z->parent; else { NodeBase* m = x; while (m->left) m = m->left; leftmost = m; } }
        if (rightmost == z) { if (z->left == 0) rightmost = z->parent; else { NodeBase* m = x; while (m->right) m = m->right; rightmost = m; } }
    }
    return y;
}
static NodeBase* Increment(NodeBase* n)
{
    if (n->right != 0) { n = n->right; while (n->left != 0) n = n->left; }
    else { NodeBase* y = n->parent; while (n == y->right) { n = y; y = y->parent; } if (n->right != y) n = y; }
    return n;
}
struct Tree {
    NodeBase* header; unsigned int count;
    Tree() : count(0) { header = (NodeBase*)std::malloc(sizeof(NodeBase)); header->color = false; header->parent = 0; header->left = header->right = header; }
    Node* CreateNode(const Value& v) { Node* t = (Node*)std::malloc(sizeof(Node)); new (&t->value) Value(v); return t; }
    NodeBase* Insert(NodeBase* x, NodeBase* y, const Value& v, NodeBase* w)
    {
        Node* z;
        if (y == header || (w == 0 && (x != 0 || v.first < ((Node*)y)->value.first))) {
            z = CreateNode(v); y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            z = CreateNode(v); y->right = z;
            if (y == header->right) header->right = z;
        }
        z->parent = y; z->left = 0; z->right = 0;
        Rebalance(z, header->parent);
        ++count;
        return z;
    }
    NodeBase* InsertKey(long key, CCPPointerInfo* info)    // test driver: plain descent, then the reconstructed _M_insert
    {
        NodeBase* y = header; NodeBase* x = header->parent;
        while (x) { y = x; x = key < ((Node*)x)->value.first ? x->left : x->right; }
        Value v; v.first = key; v.second.RefCount = (long)key * 10; v.second.LowPriority = (key & 1) != 0;
        new (&v.second.LoadingData) CCountedPointer<CBankFileAsyncData>(0, info);
        NodeBase* r = Insert(0, y, v, 0);
        v.second.LoadingData.~CCountedPointer<CBankFileAsyncData>();
        return r;
    }
    void Erase(NodeBase* position)
    {
        Node* y = (Node*)RebalanceForErase(position, header->parent, header->left, header->right);
        y->value.~Value();
        if (y != 0) std::free(y);
        --count;
    }
};
static CCPPointerInfo* NewInfo() { CCPPointerInfo* i = (CCPPointerInfo*)operator new(sizeof(CCPPointerInfo)); i->RefCount = 0; i->DeleteFunction = CountDelete; i->Data = 0; return i; }
static int InOrder(Tree& t, long* out, int max)
{
    int n = 0;
    for (NodeBase* it = t.header->left; it != t.header && n < max; it = Increment(it)) out[n++] = ((Node*)it)->value.first;
    return n;
}

int main()
{
    Tree t; int ok = 1;
    long keys[7] = { 50, 20, 80, 10, 30, 70, 90 };
    CCPPointerInfo* shared = NewInfo();
    for (int i = 0; i < 7; ++i) t.InsertKey(keys[i], i == 3 ? NewInfo() : shared);
    g_deleted = 0;
    t.Erase(t.header->left);                                  // 10 (its own block: last reference -> deleted)
    if (g_deleted != 1) ok = 0;
    t.Erase(t.header->parent);                                // the root 50 (two children)
    long out[16]; int n = InOrder(t, out, 16);
    long want[5] = { 20, 30, 70, 80, 90 };
    if (n != 5 || t.count != 5) ok = 0;
    for (int i = 0; i < n && i < 5; ++i) if (out[i] != want[i]) ok = 0;
    if (((Node*)t.header->left)->value.first != 20) ok = 0;
    if (shared->RefCount != 5 || g_deleted != 1) ok = 0;
    if (!ok) { std::printf("00be0a50_TEST FAIL\n"); return 1; }
    std::printf("00be0a50_TEST PASS\n");
    return 0;
}