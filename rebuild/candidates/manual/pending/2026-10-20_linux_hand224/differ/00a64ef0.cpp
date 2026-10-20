// CMemoryAllocatorVariableSize::RemoveFromTree(CMemoryArea*) -- red-black delete of a free area from the size tree
// Grade: DIFFER(ours v retail) -- register assignment: the deleted area lives in ebp and the removed node's parent in edi (retail edi / ebp with `push ebp` mid-body), and the child-colour store is duplicated into both arms instead of joined
struct CMemoryAreaChunk;
struct CMemoryArea {                               // 0x20 bytes
    CMemoryArea*      PNextArea;                   // +0x00
    CMemoryArea*      PRightChild;                 // +0x04
    CMemoryArea*      PPreviousArea;               // +0x08
    CMemoryArea*      PParentArea;                 // +0x0c
    CMemoryArea*      PLeftChild;                  // +0x10
    CMemoryAreaChunk* PParentChunk;                // +0x14
    long              DataSize;                    // +0x18
    unsigned char     Colour;                      // +0x1c (1 = black)
    unsigned char     ID;                          // +0x1d
};
struct CDeletedNodeChildInfo {                     // 12 bytes
    bool          LeftChild;                       // +0x00
    CMemoryArea*  PParent;                         // +0x04
    unsigned char Colour;                          // +0x08
    bool          Valid;                           // +0x09
};
class CMemoryAllocatorVariableSize {
public:
    unsigned char _pad000[0xc34];
    CMemoryArea*  PTreeRoot;                       // +0xc34
    void RemoveFromTreeFixup(CMemoryArea* area, CDeletedNodeChildInfo* info);   // 0x00A64910
    void RemoveFromTree(CMemoryArea* area);
};

void CMemoryAllocatorVariableSize::RemoveFromTree(CMemoryArea* area)
{
    CMemoryArea* removed;
    if (area->PLeftChild == 0 || area->PRightChild == 0)
        removed = area;
    else {
        removed = area->PRightChild;
        while (removed->PLeftChild)
            removed = removed->PLeftChild;
    }
    CMemoryArea* removedParent = removed->PParentArea;
    CDeletedNodeChildInfo info;
    info.Valid = true;
    info.PParent = removedParent;
    if (info.PParent == area)
        info.PParent = removed;
    if (info.PParent)
        info.LeftChild = info.PParent->PLeftChild == removed;
    CMemoryArea* child = removed->PLeftChild;
    if (child)
        info.LeftChild = true;
    else {
        child = removed->PRightChild;
        if (child)
            info.LeftChild = false;
    }
    info.Colour = child ? child->Colour : removed->Colour;
    if (child) {
        info.Valid = false;
        child->PParentArea = removedParent;
    }
    CMemoryArea* parent = removed->PParentArea;
    if (parent) {
        if (removed == parent->PLeftChild)
            parent->PLeftChild = child;
        else
            parent->PRightChild = child;
    } else {
        PTreeRoot = child;
        if (child)
            child->Colour = 1;
    }
    unsigned char removedColour = removed->Colour;
    if (removed != area) {
        removed->Colour = area->Colour;
        removed->PParentArea = area->PParentArea;
        removed->PLeftChild = area->PLeftChild;
        removed->PRightChild = area->PRightChild;
        parent = removed->PParentArea;
        if (parent) {
            if (area == parent->PLeftChild)
                parent->PLeftChild = removed;
            else
                parent->PRightChild = removed;
        }
        if (removed->PLeftChild)
            removed->PLeftChild->PParentArea = removed;
        if (removed->PRightChild)
            removed->PRightChild->PParentArea = removed;
        if (PTreeRoot == area) {
            PTreeRoot = removed;
            removed->Colour = 1;
        }
    }
    if (removedColour == 1)
        RemoveFromTreeFixup(child, &info);
    area->PParentArea = 0;
    area->PLeftChild = 0;
    area->PRightChild = 0;
}
