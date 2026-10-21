#include <cstdio>
// Behaviour test for CMemoryAllocatorVariableSize::InsertFixup (0x00A64740): insert keys BST-style, fix up, check red-black invariants.
enum { RB_RED = 0, RB_BLACK = 1 };
struct CMemoryAreaChunk;
struct CMemoryArea {
    CMemoryArea*      PNextArea;                // +0x00
    CMemoryArea*      PRightChild;              // +0x04
    CMemoryArea*      PPreviousArea;            // +0x08
    CMemoryArea*      PParentArea;              // +0x0c
    CMemoryArea*      PLeftChild;               // +0x10
    CMemoryAreaChunk* PParentChunk;             // +0x14
    long              DataSize;                 // +0x18
    unsigned char     Colour;                   // +0x1c
    unsigned char     ID;                       // +0x1d
};
struct CMemoryAllocatorVariableSize {
    CMemoryArea* _unused;
    CMemoryArea*  PTreeRoot;                    // +0xc34

    void RotateRight(CMemoryArea* node)         // out-of-line copy 0x00A64680: the right child moves up
    {
        CMemoryArea* child = node->PRightChild;
        node->PRightChild = child->PLeftChild;
        if (child->PLeftChild)
            child->PLeftChild->PParentArea = node;
        child->PParentArea = node->PParentArea;
        if (!node->PParentArea)
            PTreeRoot = child;
        else if (node == node->PParentArea->PLeftChild)
            node->PParentArea->PLeftChild = child;
        else
            node->PParentArea->PRightChild = child;
        child->PLeftChild = node;
        node->PParentArea = child;
    }
    void RotateLeft(CMemoryArea* node)          // out-of-line copy 0x00A646E0: the left child moves up
    {
        CMemoryArea* child = node->PLeftChild;
        node->PLeftChild = child->PRightChild;
        if (child->PRightChild)
            child->PRightChild->PParentArea = node;
        child->PParentArea = node->PParentArea;
        if (!node->PParentArea)
            PTreeRoot = child;
        else if (node == node->PParentArea->PLeftChild)
            node->PParentArea->PLeftChild = child;
        else
            node->PParentArea->PRightChild = child;
        child->PRightChild = node;
        node->PParentArea = child;
    }
    void InsertFixup(CMemoryArea* area);
};

void CMemoryAllocatorVariableSize::InsertFixup(CMemoryArea* area)
{
    while (area != PTreeRoot && area->PParentArea->Colour == RB_RED && area->PParentArea != PTreeRoot) {
        CMemoryArea* grandParent = area->PParentArea->PParentArea;
        if (area->PParentArea == grandParent->PLeftChild) {
            CMemoryArea* uncle = grandParent->PRightChild;
            if (uncle && uncle->Colour == RB_RED) {
                area->PParentArea->Colour = RB_BLACK;
                uncle->Colour = RB_BLACK;
                area = area->PParentArea->PParentArea;
                area->Colour = RB_RED;
            } else {
                if (area == area->PParentArea->PRightChild) {
                    area = area->PParentArea;
                    RotateRight(area);
                }
                CMemoryArea* parent = area->PParentArea;
                parent->Colour = RB_BLACK;
                CMemoryArea* top = parent->PParentArea;
                top->Colour = RB_RED;
                RotateLeft(top);
            }
        } else {
            CMemoryArea* uncle = grandParent->PLeftChild;
            if (uncle && uncle->Colour == RB_RED) {
                area->PParentArea->Colour = RB_BLACK;
                uncle->Colour = RB_BLACK;
                area = area->PParentArea->PParentArea;
                area->Colour = RB_RED;
            } else {
                if (area == area->PParentArea->PLeftChild) {
                    area = area->PParentArea;
                    RotateLeft(area);
                }
                CMemoryArea* parent = area->PParentArea;
                parent->Colour = RB_BLACK;
                CMemoryArea* top = parent->PParentArea;
                top->Colour = RB_RED;
                RotateRight(top);
            }
        }
    }
    PTreeRoot->Colour = RB_BLACK;
}

static CMemoryArea g_nodes[64];
static int Check(CMemoryArea* n, int* blackHeight)
{
    if (!n) { *blackHeight = 1; return 1; }
    if (n->Colour == RB_RED) {
        if (n->PLeftChild && n->PLeftChild->Colour == RB_RED) return 0;
        if (n->PRightChild && n->PRightChild->Colour == RB_RED) return 0;
    }
    if (n->PLeftChild && (n->PLeftChild->DataSize > n->DataSize || n->PLeftChild->PParentArea != n)) return 0;
    if (n->PRightChild && (n->PRightChild->DataSize < n->DataSize || n->PRightChild->PParentArea != n)) return 0;
    int l, r;
    if (!Check(n->PLeftChild, &l) || !Check(n->PRightChild, &r) || l != r) return 0;
    *blackHeight = l + (n->Colour == RB_BLACK ? 1 : 0);
    return 1;
}
static int Count(CMemoryArea* n) { return n ? 1 + Count(n->PLeftChild) + Count(n->PRightChild) : 0; }
int main()
{
    CMemoryAllocatorVariableSize alloc;
    alloc.PTreeRoot = 0;
    int i;
    for (i = 0; i < 64; i++) {
        CMemoryArea* n = &g_nodes[i];
        n->DataSize = (i * 37) % 101;
        n->PLeftChild = n->PRightChild = n->PParentArea = 0;
        n->Colour = RB_RED;
        if (!alloc.PTreeRoot) {
            alloc.PTreeRoot = n;
        } else {
            CMemoryArea* p = alloc.PTreeRoot;
            for (;;) {
                if (n->DataSize < p->DataSize) { if (!p->PLeftChild) { p->PLeftChild = n; break; } p = p->PLeftChild; }
                else { if (!p->PRightChild) { p->PRightChild = n; break; } p = p->PRightChild; }
            }
            n->PParentArea = p;
        }
        alloc.InsertFixup(n);
        int bh;
        if (alloc.PTreeRoot->Colour != RB_BLACK || alloc.PTreeRoot->PParentArea != 0 || !Check(alloc.PTreeRoot, &bh) || Count(alloc.PTreeRoot) != i + 1) {
            std::printf("00a64740_TEST FAIL\n");
            return 1;
        }
    }
    std::printf("00a64740_TEST PASS\n");
    return 0;
}
