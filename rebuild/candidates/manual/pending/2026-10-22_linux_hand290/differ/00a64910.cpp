// Grade: DIFFER(756v757) -- register allocation only: retail keeps this in ebp, info in edx and the near nephew in edi (4 callee-saved pushes); ours this in edi, info in ecx (3 pushes + loop-align nop). Structure is identical under a register-masked diff.
// CMemoryAllocatorVariableSize::RemoveFromTreeFixup(CMemoryArea* area, const CDeletedNodeChildInfo* info) -- red-black rebalance of the free-area tree (root +0xc34) after a removal: while the doubly-black node (area, or the deleted null child described by info on the first pass) is not the root, recolour the sibling or rotate; finally blacken area
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A64910, length 768 bytes (+ int3 padding), `ret 8`.
// Callees: CMemoryAllocatorVariableSize::RotateLeft 0x00A646E0 (landed; the left child moves up) -- called out of line once;
//   the other five rotations are inlined.
// Layout: as the landed InsertFixup 0x00A64740 (CMemoryArea: PRightChild +0x04, PParentArea +0x0c, PLeftChild +0x10,
//   Colour +0x1c, 0 red / 1 black); Ego_r.pdb CDeletedNodeChildInfo (12 bytes): LeftChild +0, PParent +4, Colour +8, Valid +9.
// Shape: the PDB signature `?RemoveFromTreeFixup@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@1@PBVCDeletedNodeChildInfo@@@Z`;
//   the null-child description only applies to the first pass (info is cleared whenever area moves). The rotations are the
//   InsertFixup inline members; the one out-of-line call sits in an in-body `#pragma inline_depth(0)` region.
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
class CDeletedNodeChildInfo {
public:
    bool          LeftChild;                    // +0x00
    CMemoryArea*  PParent;                      // +0x04
    unsigned char Colour;                       // +0x08
    bool          Valid;                        // +0x09
};
struct CMemoryAllocatorVariableSize {
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
    bool IsNodeBlack(const CMemoryArea* node) const { return node == 0 || node->Colour == RB_BLACK; }
    void RemoveFromTreeFixup(CMemoryArea* area, const CDeletedNodeChildInfo* info);
};

void CMemoryAllocatorVariableSize::RemoveFromTreeFixup(CMemoryArea* area, const CDeletedNodeChildInfo* info)
{
    while ((area != 0 && area != PTreeRoot && area->Colour == RB_BLACK) ||
           (info != 0 && info->Valid && info->PParent != 0 && info->Colour == RB_BLACK)) {
        CMemoryArea* parent = area ? area->PParentArea : info->PParent;
        if ((area != 0 && area == parent->PLeftChild) || (info != 0 && info->LeftChild)) {
            CMemoryArea* sibling = parent->PRightChild;
            if (sibling->Colour == RB_RED) {
                sibling->Colour = RB_BLACK;
                parent->Colour = RB_RED;
                RotateRight(parent);
                sibling = parent->PRightChild;
                if (sibling == 0) {
                    area = PTreeRoot;
                    info = 0;
                    continue;
                }
            }
            CMemoryArea* nephew = sibling->PLeftChild;
            if (IsNodeBlack(nephew) && IsNodeBlack(sibling->PRightChild)) {
                sibling->Colour = RB_RED;
                area = parent;
            } else {
                if (IsNodeBlack(sibling->PRightChild)) {
                    if (nephew)
                        nephew->Colour = RB_BLACK;
                    sibling->Colour = RB_RED;
#pragma inline_depth(0)
                    RotateLeft(sibling);
#pragma inline_depth()
                    sibling = parent->PRightChild;
                }
                sibling->Colour = parent->Colour;
                parent->Colour = RB_BLACK;
                if (sibling->PRightChild)
                    sibling->PRightChild->Colour = RB_BLACK;
                RotateRight(parent);
                area = PTreeRoot;
            }
        } else {
            CMemoryArea* sibling = parent->PLeftChild;
            if (sibling->Colour == RB_RED) {
                sibling->Colour = RB_BLACK;
                parent->Colour = RB_RED;
                RotateLeft(parent);
                sibling = parent->PLeftChild;
                if (sibling == 0) {
                    area = PTreeRoot;
                    info = 0;
                    continue;
                }
            }
            CMemoryArea* nephew = sibling->PRightChild;
            if (IsNodeBlack(nephew) && IsNodeBlack(sibling->PLeftChild)) {
                sibling->Colour = RB_RED;
                area = parent;
            } else {
                if (IsNodeBlack(sibling->PLeftChild)) {
                    if (nephew)
                        nephew->Colour = RB_BLACK;
                    sibling->Colour = RB_RED;
                    RotateRight(sibling);
                    sibling = parent->PLeftChild;
                }
                sibling->Colour = parent->Colour;
                parent->Colour = RB_BLACK;
                if (sibling->PLeftChild)
                    sibling->PLeftChild->Colour = RB_BLACK;
                RotateLeft(parent);
                area = PTreeRoot;
            }
        }
        info = 0;
    }
    if (area)
        area->Colour = RB_BLACK;
}
