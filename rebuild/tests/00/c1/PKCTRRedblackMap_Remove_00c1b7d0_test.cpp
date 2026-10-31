// Behaviour test for PKCTRRedblackMap::Remove (0x00C1B7D0): the same model with textbook (Niemann) rotations, insert
// fixup and delete fixup; inserts keys, removes them through the cache and the search path, checks the result codes,
// the allocator frees, the cache reset and the red-black / ordering invariants after every removal.
#include <cstdio>

struct PKCTRRedblackMapNode {
    PKCTRRedblackMapNode* Left;
    PKCTRRedblackMapNode* Right;
    PKCTRRedblackMapNode* Parent;
    long                  Colour;
    unsigned long         Key;
    void*                 Value;
};
enum { PKCTR_BLACK = 0, PKCTR_RED = 1 };
struct PKCTRRedblackNode;

static int g_frees = 0;
class CMapNodeAllocator {
public:
    virtual void Free(void* node) { ++g_frees; delete (PKCTRRedblackMapNode*)node; }
    virtual void* Allocate() { return new PKCTRRedblackMapNode; }
    void Release(PKCTRRedblackNode** node) { Free(*node); *node = 0; }
};

class PKCTRRedblackMap {
public:
    CMapNodeAllocator*    PAllocator;
    PKCTRRedblackMapNode  Sentinel;
    PKCTRRedblackMapNode* Root;
    PKCTRRedblackMapNode* LastFound;

    void RotateLeft(PKCTRRedblackMapNode* x)
    {
        PKCTRRedblackMapNode* y = x->Right;
        x->Right = y->Left;
        if (y->Left != &Sentinel) y->Left->Parent = x;
        if (y != &Sentinel) y->Parent = x->Parent;
        if (x->Parent) { if (x == x->Parent->Left) x->Parent->Left = y; else x->Parent->Right = y; }
        else Root = y;
        y->Left = x;
        if (x != &Sentinel) x->Parent = y;
    }
    void RotateRight(PKCTRRedblackMapNode* x)
    {
        PKCTRRedblackMapNode* y = x->Left;
        x->Left = y->Right;
        if (y->Right != &Sentinel) y->Right->Parent = x;
        if (y != &Sentinel) y->Parent = x->Parent;
        if (x->Parent) { if (x == x->Parent->Right) x->Parent->Right = y; else x->Parent->Left = y; }
        else Root = y;
        y->Right = x;
        if (x != &Sentinel) x->Parent = y;
    }
    void InsertFixup(PKCTRRedblackMapNode* x)
    {
        while (x != Root && x->Parent->Colour == PKCTR_RED) {
            if (x->Parent == x->Parent->Parent->Left) {
                PKCTRRedblackMapNode* y = x->Parent->Parent->Right;
                if (y->Colour == PKCTR_RED) {
                    x->Parent->Colour = PKCTR_BLACK; y->Colour = PKCTR_BLACK; x->Parent->Parent->Colour = PKCTR_RED; x = x->Parent->Parent;
                } else {
                    if (x == x->Parent->Right) { x = x->Parent; RotateLeft(x); }
                    x->Parent->Colour = PKCTR_BLACK; x->Parent->Parent->Colour = PKCTR_RED; RotateRight(x->Parent->Parent);
                }
            } else {
                PKCTRRedblackMapNode* y = x->Parent->Parent->Left;
                if (y->Colour == PKCTR_RED) {
                    x->Parent->Colour = PKCTR_BLACK; y->Colour = PKCTR_BLACK; x->Parent->Parent->Colour = PKCTR_RED; x = x->Parent->Parent;
                } else {
                    if (x == x->Parent->Left) { x = x->Parent; RotateRight(x); }
                    x->Parent->Colour = PKCTR_BLACK; x->Parent->Parent->Colour = PKCTR_RED; RotateLeft(x->Parent->Parent);
                }
            }
        }
        Root->Colour = PKCTR_BLACK;
    }
    void DeleteFixup(PKCTRRedblackMapNode* x)
    {
        while (x != Root && x->Colour == PKCTR_BLACK) {
            if (x == x->Parent->Left) {
                PKCTRRedblackMapNode* w = x->Parent->Right;
                if (w->Colour == PKCTR_RED) { w->Colour = PKCTR_BLACK; x->Parent->Colour = PKCTR_RED; RotateLeft(x->Parent); w = x->Parent->Right; }
                if (w->Left->Colour == PKCTR_BLACK && w->Right->Colour == PKCTR_BLACK) { w->Colour = PKCTR_RED; x = x->Parent; }
                else {
                    if (w->Right->Colour == PKCTR_BLACK) { w->Left->Colour = PKCTR_BLACK; w->Colour = PKCTR_RED; RotateRight(w); w = x->Parent->Right; }
                    w->Colour = x->Parent->Colour; x->Parent->Colour = PKCTR_BLACK; w->Right->Colour = PKCTR_BLACK; RotateLeft(x->Parent); x = Root;
                }
            } else {
                PKCTRRedblackMapNode* w = x->Parent->Left;
                if (w->Colour == PKCTR_RED) { w->Colour = PKCTR_BLACK; x->Parent->Colour = PKCTR_RED; RotateRight(x->Parent); w = x->Parent->Left; }
                if (w->Right->Colour == PKCTR_BLACK && w->Left->Colour == PKCTR_BLACK) { w->Colour = PKCTR_RED; x = x->Parent; }
                else {
                    if (w->Left->Colour == PKCTR_BLACK) { w->Right->Colour = PKCTR_BLACK; w->Colour = PKCTR_RED; RotateLeft(w); w = x->Parent->Left; }
                    w->Colour = x->Parent->Colour; x->Parent->Colour = PKCTR_BLACK; w->Left->Colour = PKCTR_BLACK; RotateRight(x->Parent); x = Root;
                }
            }
        }
        x->Colour = PKCTR_BLACK;
    }
    void Insert(unsigned long key)
    {
        PKCTRRedblackMapNode* cur = Root; PKCTRRedblackMapNode* parent = 0;
        while (cur != &Sentinel) { parent = cur; cur = key < cur->Key ? cur->Left : cur->Right; }
        PKCTRRedblackMapNode* x = (PKCTRRedblackMapNode*)PAllocator->Allocate();
        x->Key = key; x->Value = (void*)(key * 2 + 1); x->Parent = parent; x->Left = &Sentinel; x->Right = &Sentinel; x->Colour = PKCTR_RED;
        if (parent) { if (key < parent->Key) parent->Left = x; else parent->Right = x; } else Root = x;
        InsertFixup(x);
    }

    long Remove(const unsigned long* key)
    {
        PKCTRRedblackMapNode* z;
        if (LastFound != 0 && LastFound->Key == *key) {
            z = LastFound;
        } else {
            z = Root;
            while (z != &Sentinel) {
                if (*key == z->Key)
                    break;
                if (*key < z->Key)
                    z = z->Left;
                else
                    z = z->Right;
            }
            if (z == &Sentinel)
                return 3;
        }
        PKCTRRedblackMapNode* y;
        if (z->Left == &Sentinel || z->Right == &Sentinel) {
            y = z;
        } else {
            y = z->Right;
            while (y->Left != &Sentinel)
                y = y->Left;
        }
        PKCTRRedblackMapNode* x;
        if (y->Left != &Sentinel)
            x = y->Left;
        else
            x = y->Right;
        x->Parent = y->Parent;
        if (y->Parent) {
            if (y == y->Parent->Left)
                y->Parent->Left = x;
            else
                y->Parent->Right = x;
        } else {
            Root = x;
        }
        if (y != z) {
            z->Key = y->Key;
            z->Value = y->Value;
        }
        if (y->Colour == PKCTR_BLACK)
            DeleteFixup(x);
        PAllocator->Release((PKCTRRedblackNode**)&y);
        LastFound = 0;
        return 0;
    }
    PKCTRRedblackMapNode* FindNode(unsigned long key)
    {
        PKCTRRedblackMapNode* n = Root;
        while (n != &Sentinel && n->Key != key) n = key < n->Key ? n->Left : n->Right;
        return n == &Sentinel ? 0 : n;
    }
};

static int g_bad = 0;
static int Check(PKCTRRedblackMap& m, PKCTRRedblackMapNode* n, unsigned long lo, unsigned long hi, int* count)
{
    if (n == &m.Sentinel) return 1;
    ++*count;
    if (n->Key < lo || n->Key > hi) g_bad = 1;
    if (n->Value != (void*)(n->Key * 2 + 1)) g_bad = 1;
    if (n->Colour == PKCTR_RED && (n->Left->Colour == PKCTR_RED || n->Right->Colour == PKCTR_RED)) g_bad = 1;
    if (n->Left != &m.Sentinel && n->Left->Parent != n) g_bad = 1;
    if (n->Right != &m.Sentinel && n->Right->Parent != n) g_bad = 1;
    int l = Check(m, n->Left, lo, n->Key, count), r = Check(m, n->Right, n->Key, hi, count);
    if (l != r) g_bad = 1;
    return l + (n->Colour == PKCTR_BLACK ? 1 : 0);
}

int main()
{
    CMapNodeAllocator alloc;
    PKCTRRedblackMap m;
    m.PAllocator = &alloc;
    m.Sentinel.Left = m.Sentinel.Right = &m.Sentinel; m.Sentinel.Parent = 0; m.Sentinel.Colour = PKCTR_BLACK; m.Sentinel.Key = 0; m.Sentinel.Value = 0;
    m.Root = &m.Sentinel; m.LastFound = 0;
    const int N = 64;
    for (int i = 0; i < N; ++i) m.Insert((unsigned long)((i * 37) % N) + 1);
    int live = N;
    unsigned long absent = 1000;
    if (m.Remove(&absent) != 3) { std::printf("00c1b7d0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < N; ++i) {
        unsigned long k = (unsigned long)((i * 11) % N) + 1;
        if (i % 3 == 0) m.LastFound = m.FindNode(k);           // cache hit path
        else if (i % 3 == 1) m.LastFound = m.FindNode(((k % N) + 1)); // stale cache (other key) -> search path
        int frees = g_frees;
        if (m.Remove(&k) != 0 || g_frees != frees + 1 || m.LastFound != 0) { std::printf("00c1b7d0_TEST FAIL\n"); return 1; }
        --live;
        if (m.FindNode(k) != 0) { std::printf("00c1b7d0_TEST FAIL\n"); return 1; }
        if (m.Remove(&k) != 3) { std::printf("00c1b7d0_TEST FAIL\n"); return 1; }
        int count = 0;
        if (m.Root != &m.Sentinel && m.Root->Colour != PKCTR_BLACK) g_bad = 1;
        Check(m, m.Root, 0, 0xffffffffUL, &count);
        if (g_bad || count != live) { std::printf("00c1b7d0_TEST FAIL\n"); return 1; }
    }
    if (m.Root != &m.Sentinel || g_frees != N) { std::printf("00c1b7d0_TEST FAIL\n"); return 1; }
    std::printf("00c1b7d0_TEST PASS\n");
    return 0;
}