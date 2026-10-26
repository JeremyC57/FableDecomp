// CMemoryAllocatorVariableSize::InsertIntoTree(CMemoryArea*, CMemoryArea*) -- red-black insert of a free area keyed by (DataSize, address): walk down from the given root, link as a red leaf, rebalance
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A64E70, 128 bytes (padding included).
// Shape: descend from `root` remembering the parent and which side was taken (a bool cleared or set on
// every step: `xor dl,dl` / `mov dl,1`); smaller DataSize (signed) or, on equal sizes, a lower address
// goes left (PLeftChild +0x10), otherwise right (PRightChild +4). The area's PParentArea (+0xc) is the
// last node; with no parent it becomes PTreeRoot (+0xc34), else the remembered child slot. The area is
// coloured red (Colour +0x1c = 0) and InsertFixup (0x00A64740) rebalances; VC duplicates that tail into
// each of the three link arms.
struct CMemoryAreaChunk;
class CMemoryAllocatorVariableSize;

// Process-wide copies of the active allocator's alignment, refreshed on entry to the allocator's
// public methods (0x013D28D4 alignment, 0x013D28D8 alignment shift).
extern unsigned long g_CurrentAlignment;        // 0x013D28D4
extern unsigned long g_CurrentAlignmentShift;   // 0x013D28D8

// Round a header size up to the current alignment: ((size + align - 1) >> shift) << shift.
#define ALIGNED_SIZE(size) ((((size) + g_CurrentAlignment - 1) >> g_CurrentAlignmentShift) << g_CurrentAlignmentShift)

struct CMemoryArea {                            // PDB: 32 bytes
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

struct CMemoryAreaChunk {                       // PDB: 52 bytes
    long              TotalDataSize;            // +0x00
    long              NoFreeAreas;              // +0x04
    long              NoAllocatedAreas;         // +0x08
    long              FreeDataSize;             // +0x0c
    long              LargestFreeAreaSize;      // +0x10
    long              SmallestFreeAreaSize;     // +0x14
    CMemoryAllocatorVariableSize* PParentAllocator;   // +0x18
    bool              Permanent;                // +0x1c
    bool              DemandPaged;              // +0x1d
    CMemoryAreaChunk* PNextChunk;               // +0x20
    CMemoryAreaChunk* PPreviousChunk;           // +0x24
    CMemoryArea*      PFirstFreeArea;           // +0x28
    CMemoryArea*      PLastFreeArea;            // +0x2c
    CMemoryArea*      PFirstAllocatedArea;      // +0x30
};

struct CMemoryAllocatorBase {                   // PDB: 3100 bytes
    virtual void v00();
    unsigned char     PerformanceStats[0xc10];  // +0x04 CPerformanceStats
    unsigned long     Alignment;                // +0xc14
    unsigned long     AlignmentShift;           // +0xc18
};

class CMemoryAllocatorVariableSize : public CMemoryAllocatorBase { public:   // PDB: 3128 bytes, vtable 0x0129DB1C
    CMemoryAreaChunk* PFirstChunk;              // +0xc1c
    long              MinChunkSize;             // +0xc20
    long              NoFreesSinceConsolidation;// +0xc24
    unsigned long     Flags;                    // +0xc28
    unsigned long     InternalFlags;            // +0xc2c
    unsigned long     ExtraMemUsedByOS;         // +0xc30
    CMemoryArea*      PTreeRoot;                // +0xc34
    void InsertIntoTree(CMemoryArea* root, CMemoryArea* area);
    void InsertFixup(CMemoryArea* area);        // 0x00A64740
};

void CMemoryAllocatorVariableSize::InsertIntoTree(CMemoryArea* root, CMemoryArea* area)
{
    CMemoryArea* parent = 0;
    bool left = false;
    CMemoryArea* node = root;
    while (node) {
        parent = node;
        if (area->DataSize == node->DataSize) {
            if (area < node) {
                node = node->PLeftChild;
                left = true;
            }
            else {
                node = node->PRightChild;
                left = false;
            }
        }
        else if (area->DataSize < node->DataSize) {
            node = node->PLeftChild;
            left = true;
        }
        else {
            node = node->PRightChild;
            left = false;
        }
    }
    area->PParentArea = parent;
    if (parent == 0)
        PTreeRoot = area;
    else if (left)
        parent->PLeftChild = area;
    else
        parent->PRightChild = area;
    area->Colour = 0;
    InsertFixup(area);
}