// CMemoryAllocatorVariableSize::RemoveFromTreeFixup(CMemoryArea* area, const CDeletedNodeChildInfo* info) -- red-black rebalance of the free-area tree (root +0xc34) after a removal: while the doubly-black node (area, or the deleted null child described by info on the first pass) is not the root, recolour the sibling or rotate; finally blacken area
// Leaf: ?RemoveFromTreeFixup@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@1@PBVCDeletedNodeChildInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A64910, 757 bytes (+ int3 padding), `ret 8`.
// Callees: CMemoryAllocatorVariableSize::RotateLeft 0x00A646E0 (called out of line once; RotateRight 0x00A64680 and the other
//   four rotations are inlined).
// Names: Ego_r.pdb `?RemoveFromTreeFixup@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@1@PBVCDeletedNodeChildInfo@@@Z`,
//   `?RotateLeft@...@@IAEXPAVCMemoryArea@1@@Z`, `?RotateRight@...` (protected members; CMemoryArea is the nested class, key V).
// Layout: CMemoryAllocatorBase.h (0xc1c bytes, flattened vptr) + PTreeRoot +0xc34. CMemoryArea (Ego_r.pdb, 32 bytes, as the landed
//   InsertFixup 0x00A64740): PRightChild +0x04, PParentArea +0x0c, PLeftChild +0x10, Colour +0x1c (0 red, 1 black).
//   CDeletedNodeChildInfo (Ego_r.pdb, 12 bytes): LeftChild +0, PParent +4, Colour +8, Valid +9.
// Shape: the two rotations are ordinary (non-inline) members defined in this TU before the caller -- their out-of-line copies
//   are retail's 0x00A64680 / 0x00A646E0 -- and VC7.1's /Ob2 auto-inliner inlines five of the six calls and keeps the inner
//   left-case RotateLeft(sibling) as a call. That inlining decision is what gives retail's register assignment (this in ebp,
//   info in edx, 4 callee-saved pushes). The parked attempt forced the out-of-line call with `#pragma inline_depth(0)` around
//   inline members and got this in edi / info in ecx (756v757).
#include "engine/CMemoryAllocatorBase.h"
enum { RB_RED = 0, RB_BLACK = 1 };
class CDeletedNodeChildInfo;
// Layout: CMemoryAllocatorBase.h (0xc1c, flattened vptr) + the variable-size allocator's chunk list and free-area tree root.
class CMemoryAllocatorVariableSize : public CMemoryAllocatorBase {
public:
    class CMemoryAreaChunk;
    class CMemoryArea {
    public:
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
private:
    unsigned char     _chunks[0xc34 - sizeof(CMemoryAllocatorBase)];
    CMemoryArea*      PTreeRoot;                    // +0xc34
protected:
    void RotateRight(CMemoryArea* node);           // 0x00A64680
    void RotateLeft(CMemoryArea* node);            // 0x00A646E0
    void RemoveFromTreeFixup(CMemoryArea* area, const CDeletedNodeChildInfo* info);
};
// Descriptive inline predicate (no PDB name; a null leaf counts as black). Written as one inline helper, not open-coded:
// the open-coded `a == 0 || a->Colour == RB_BLACK` gives 761 bytes, the helper gives retail's branch layout.
static inline bool IsBlackArea(const CMemoryAllocatorVariableSize::CMemoryArea* area)
{
    return area == 0 || area->Colour == RB_BLACK;
}
// Ego_r.pdb CDeletedNodeChildInfo (class, 12 bytes): the null child a removal left behind.
class CDeletedNodeChildInfo {
public:
    bool          LeftChild;                    // +0x00
    CMemoryAllocatorVariableSize::CMemoryArea* PParent;   // +0x04
    unsigned char Colour;                       // +0x08
    bool          Valid;                        // +0x09
};

void CMemoryAllocatorVariableSize::RotateRight(CMemoryArea* node)
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
void CMemoryAllocatorVariableSize::RotateLeft(CMemoryArea* node)
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
            if (IsBlackArea(nephew) && IsBlackArea(sibling->PRightChild)) {
                sibling->Colour = RB_RED;
                area = parent;
            } else {
                if (IsBlackArea(sibling->PRightChild)) {
                    if (nephew)
                        nephew->Colour = RB_BLACK;
                    sibling->Colour = RB_RED;
                    RotateLeft(sibling);
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
            if (IsBlackArea(nephew) && IsBlackArea(sibling->PLeftChild)) {
                sibling->Colour = RB_RED;
                area = parent;
            } else {
                if (IsBlackArea(sibling->PLeftChild)) {
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