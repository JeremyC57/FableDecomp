// CMemoryAllocatorVariableSize::CMemoryAreaChunk::AllocAreaFromArea(CMemoryArea* area, long size) -- carve an allocated area of `size` bytes off the top of a free area (or take the whole area when it is not larger than the request plus a header), filing the result at the head of the chunk's allocated list
// Grade: DIFFER(265v265) -- register/scheduling only: retail computes the area header in eax and the request with `mov ecx,[size] ; lea esi,[eax+ecx]`; ours preloads size into edx before `push esi` and builds the header in esi; the second Create's size is re-read late through eax (retail edx right after RemoveAreaFromFreeList); the allocated-list loads rotate eax/ecx/edx. Respelled this session to the Ego_r.pdb nested classes (CMemoryAllocatorVariableSize::CMemoryAreaChunk / CMemoryArea) and the PDB inline helpers CMemoryArea::Create (static __fastcall) and InsertAreaIntoAllocatedList (same bytes). Tried: unsigned needed, 64-bit check first (263), size + header (242), inline GetAlignedSize helper.
// Leaf: ?AllocAreaFromArea@CMemoryAreaChunk@CMemoryAllocatorVariableSize@@QAEPAVCMemoryArea@2@PAV32@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A65480, 265 bytes, thiscall `ret 8`.
// Identity: Ego_r.pdb ?AllocAreaFromArea@CMemoryAreaChunk@CMemoryAllocatorVariableSize@@QAEPAVCMemoryArea@2@PAV32@J@Z -- the
//   chunk and area classes are NESTED in CMemoryAllocatorVariableSize (class keys V); the same name set gives the inline
//   helpers this body expands: CMemoryArea::Create(void*, CMemoryAreaChunk*, unsigned long) (static __fastcall) and the
//   protected CMemoryAreaChunk::InsertAreaIntoAllocatedList(CMemoryArea*).
// Callees: CMemoryAreaChunk::RemoveAreaFromFreeList 0x00A65060 / InsertAreaIntoFreeList 0x00A650F0 (protected, Ego_r IAE).
// Globals: the active allocator's alignment 0x013D28D4 and alignment shift 0x013D28D8 (g_CurrentAlignment /
//   g_CurrentAlignmentShift, the landed Init 0x00A65410 spelling; descriptive -- no PDB data symbol).
// Layout: Ego_r.pdb CMemoryArea (0x20) and CMemoryAreaChunk (0x34), as the landed Init 0x00A65410.
struct CMemoryAllocatorVariableSize_Fwd;

extern unsigned long g_CurrentAlignment;        // 0x013D28D4
extern unsigned long g_CurrentAlignmentShift;   // 0x013D28D8

// Round a header size up to the current alignment: ((size + align - 1) >> shift) << shift.
#define ALIGNED_SIZE(size) ((((size) + g_CurrentAlignment - 1) >> g_CurrentAlignmentShift) << g_CurrentAlignmentShift)

class CMemoryAllocatorVariableSize {
public:
    class CMemoryAreaChunk;
    class CMemoryArea {                             // Ego_r.pdb: 32 bytes
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
        static CMemoryArea* __fastcall Create(void* memory, CMemoryAreaChunk* chunk, unsigned long size)
        {
            CMemoryArea* area = (CMemoryArea*)memory;
            area->PNextArea = 0;
            area->PRightChild = 0;
            area->PPreviousArea = 0;
            area->PParentArea = 0;
            area->PLeftChild = 0;
            area->Colour = 0;
            area->PParentChunk = chunk;
            area->DataSize = size;
            return area;
        }
    };
    class CMemoryAreaChunk {                        // Ego_r.pdb: 52 bytes
    public:
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
    protected:
        void RemoveAreaFromFreeList(CMemoryArea* area);   // 0x00A65060
        void InsertAreaIntoFreeList(CMemoryArea* area);   // 0x00A650F0
        void InsertAreaIntoAllocatedList(CMemoryArea* area)
        {
            area->PNextArea = PFirstAllocatedArea;
            area->PPreviousArea = 0;
            if (PFirstAllocatedArea)
                PFirstAllocatedArea->PPreviousArea = area;
            ++NoAllocatedAreas;
            PFirstAllocatedArea = area;
        }
    public:
        CMemoryArea* AllocAreaFromArea(CMemoryArea* area, long size);
    };
};

CMemoryAllocatorVariableSize::CMemoryArea* CMemoryAllocatorVariableSize::CMemoryAreaChunk::AllocAreaFromArea(CMemoryArea* area, long size)
{
    long needed = ALIGNED_SIZE(sizeof(CMemoryArea)) + size;
    __int64 checked = needed;
    if (checked < 0 || checked > 0x7FFFFFFF)
        return 0;
    if (area->DataSize > needed) {
        RemoveAreaFromFreeList(area);
        CMemoryArea::Create(area, this, area->DataSize - needed);
        CMemoryArea* allocated = CMemoryArea::Create((char*)area + ALIGNED_SIZE(sizeof(CMemoryArea)) + area->DataSize, this, size);
        InsertAreaIntoFreeList(area);
        InsertAreaIntoAllocatedList(allocated);
        return allocated;
    }
    RemoveAreaFromFreeList(area);
    InsertAreaIntoAllocatedList(area);
    return area;
}
