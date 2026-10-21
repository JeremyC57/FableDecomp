// CEngineTreeFilerBase::ConvertTreeToList(CNodeHeader*) -- flatten a key tree into a singly linked list of its leaves (in depth-first order), freeing every interior node on the way; a leaf root is returned as a one-element list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF2AD0, length 115 bytes (+ int3 padding).
// Callees: operator delete @ 0x00BFE9BC (interior nodes are PODs: no destructor call).
// Node layout as the landed sibling CEngineTreeFilerBase::DeleteTree @ 0x00BF29E0: leaf flag byte +0x00, +0x08 link
// word, next sibling / next list entry +0x0c, first child +0x10, parent +0x18.
// Shape: the walk cursor is the `root` parameter itself (kept in eax); a `__fastcall` member that never reads `this` (`ret 4`); the head/tail locals are zeroed (ebp/esi) before the
// leaf test; the walk is iterative: pop the first child, append a leaf (the tail store, then the +0x08 word, which retail
// sets to the leaf itself when the list was non-empty and to null for the first leaf), descend into an interior child,
// or climb to the parent and free the exhausted node; the `next = 0 ; parent = 0` tail is duplicated into both append
// arms by VC7.1. Registers: `first = last = 0;` zeroes the tail first (esi) and the head second (ebp), and `last = child`
// written inside EACH append arm gives the tail esi and the climbing parent edi (one shared `last = child` after the
// if/else swaps esi/edi; separate `= 0` initialisers in first/last order swap the two zeroing instructions).
void __cdecl operator delete(void* block);         // 0x00BFE9BC

struct CNodeHeader {
    unsigned char m_isLeaf;                        // +0x00
    unsigned char _pad01[7];
    CNodeHeader*  m_pLink;                         // +0x08
    CNodeHeader*  m_pNext;                         // +0x0c
    CNodeHeader*  m_pFirstChild;                   // +0x10
    unsigned char _pad14[4];
    CNodeHeader*  m_pParent;                       // +0x18
};

struct CEngineTreeFilerBase {
    CNodeHeader* ConvertTreeToList(CNodeHeader* root);
};

CNodeHeader* CEngineTreeFilerBase::ConvertTreeToList(CNodeHeader* root)
{
    CNodeHeader* parent;
    CNodeHeader* first;
    CNodeHeader* last;
    first = last = 0;
    if (root->m_isLeaf) {
        root->m_pNext = 0;
        return root;
    }
    root->m_pParent = 0;
    do {
        CNodeHeader* child = root->m_pFirstChild;
        if (child) {
            root->m_pFirstChild = child->m_pNext;
            if (child->m_isLeaf) {
                if (last) {
                    last->m_pNext = child;
                    child->m_pLink = last->m_pNext;
                    last = child;
                } else {
                    first = child;
                    child->m_pLink = 0;
                    last = child;
                }
                child->m_pNext = 0;
                child->m_pParent = 0;
            } else {
                root = child;
            }
        } else {
            parent = root->m_pParent;
            delete root;
            root = parent;
        }
    } while (root);
    return first;
}
// ---- harness ----
#include <cstdio>
#include <cstdlib>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static CNodeHeader* Node(bool leaf)
{
    CNodeHeader* n = new CNodeHeader;
    memset(n, 0, sizeof(*n));
    n->m_isLeaf = leaf ? 1 : 0;
    return n;
}
static void AddChild(CNodeHeader* parent, CNodeHeader* child)
{
    child->m_pParent = parent;
    child->m_pNext = 0;
    if (!parent->m_pFirstChild) { parent->m_pFirstChild = child; return; }
    CNodeHeader* c = parent->m_pFirstChild;
    while (c->m_pNext) c = c->m_pNext;
    c->m_pNext = child;
}
int main()
{
    CEngineTreeFilerBase filer;
    CNodeHeader* single = Node(true);
    single->m_pNext = (CNodeHeader*)0x1;
    CHECK(filer.ConvertTreeToList(single) == single && single->m_pNext == 0);
    // root(interior) -> a(leaf), b(interior) -> {c(leaf), d(leaf)}, e(leaf)
    CNodeHeader* root = Node(false);
    CNodeHeader* a = Node(true); CNodeHeader* b = Node(false); CNodeHeader* e = Node(true);
    CNodeHeader* c = Node(true); CNodeHeader* d = Node(true);
    AddChild(root, a); AddChild(root, b); AddChild(root, e); AddChild(b, c); AddChild(b, d);
    CNodeHeader* list = filer.ConvertTreeToList(root);
    CHECK(list == a);
    CHECK(a->m_pNext == c && c->m_pNext == d && d->m_pNext == e && e->m_pNext == 0);
    CHECK(a->m_pLink == 0 && c->m_pLink == c && d->m_pLink == d && e->m_pLink == e);
    CHECK(a->m_pParent == 0 && c->m_pParent == 0 && e->m_pParent == 0);
    if (fails == 0) printf("00bf2ad0_TEST PASS\n");
    return fails;
}