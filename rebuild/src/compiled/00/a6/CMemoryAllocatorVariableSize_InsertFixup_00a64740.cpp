// CMemoryAllocatorVariableSize::InsertFixup(CMemoryArea* area) -- red-black rebalance of the free-area tree (root +0xc34) after inserting `area`: recolour while the parent and uncle are red, otherwise rotate (inner case first) and recolour; the root ends black
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A64740, length 464 bytes (`ret 4`).
// Callees: none -- both rotations are inlined (their out-of-line copies are 0x00A64680 and the landed 0x00A646E0).
// Layout (Ego_r.pdb CMemoryArea, as in the landed CMemoryAreaChunk::InsertAreaIntoFreeList 0x00A650F0): PRightChild +0x04,
//   PParentArea +0x0c, PLeftChild +0x10, Colour +0x1c (0 red, 1 black); CMemoryAllocatorVariableSize::PTreeRoot +0xc34.
//   No engine header for these classes.
// Shape: the two rotations are inline members on the allocator (root update on a parentless node); the whole guard sits in
//   the while test (`x != root && parent red && parent != root`): VC7.1 rotates it and duplicates the exits as retail does.
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
class CMemoryAllocatorVariableSize { public:
    unsigned char _base[0xc34];
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