// CSimpleMemoryPool::Allocate(unsigned long size) -- first-fit allocation from the free-area list: round to 16, unlink the first area big enough, mark it used and split off a free tail when more than a header remains; with no fit grow the pool by a new block and retry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5C910, length 134 bytes.
// Callees: operator new @ 0x00BFEA1A, operator delete @ 0x00BFE9BC, AllocateAligned @ 0x009F2BE0 / FreeAligned @ 0x009F2B20 (`__fastcall`),
// Reset @ 0x00A5C790; the block deleting dtor 0x00A5C9B0 is slot 0 of CSimpleMemoryPool::CMemoryBlock (vtable 0x0129D568).
// Layout: Ego_r.pdb CSimpleMemoryPool (0x1c: FirstMemoryBlock +4, Capacity +8, GrowthGranularity +0xc, Root CArea +0x10) and
// CMemoryBlock (0x18: CIVCountedPointeeBase {vptr, count +4}, BlockData +8, OwnsData +0xc, BlockSize +0x10, NextBlock +0x14);
// the engine header carries Root as opaque bytes, so this local model spells out CArea {Size, NextFreeArea, Status}.
// Areas carry a 16-byte header; Status 0x61 = free, 0x67 = used. The free-list push is one inline AddFreeArea (Size, Status,
// Next, head) used by every row of the class.
// Shape: the retry is a tail call `return Allocate(size)` (VC turns it into the jump back to the rounding at the loop top, so the size is re-rounded); the growth size is `grow = Capacity; if (FirstMemoryBlock) grow = GrowthGranularity;` (the ternary gives an extra jmp) then max(grow, size).
// Leaf: Allocate
// Shared model for the CSimpleMemoryPool rows (Ego_r.pdb layouts; the engine header's Root is opaque, so a local model is used).
void* __cdecl operator new(unsigned int size);                     // 0x00BFEA1A
void __cdecl operator delete(void* block);                         // 0x00BFE9BC
void* __fastcall AllocateAligned(unsigned long size, unsigned long alignment);   // 0x009F2BE0
void __fastcall FreeAligned(void* block);                          // 0x009F2B20

enum EAreaStatus { AREA_FREE = 0x61, AREA_USED = 0x67 };

struct CArea {                                     // 12 bytes; each area has a 16-byte header in the block
    unsigned long Size;                            // +0x00
    CArea*        NextFreeArea;                    // +0x04
    EAreaStatus   Status;                          // +0x08
};

class CIVCountedPointeeBase { public:                     // vtable 0x012354D0
    long m_references;                             // +0x04
    CIVCountedPointeeBase() : m_references(1) {}
    virtual ~CIVCountedPointeeBase() {}
};

struct CMemoryBlock : CIVCountedPointeeBase {      // CSimpleMemoryPool::CMemoryBlock, vtable 0x0129D568, 0x18 bytes
    unsigned char* BlockData;                      // +0x08
    bool           OwnsData;                       // +0x0c
    unsigned long  BlockSize;                      // +0x10
    CMemoryBlock*  NextBlock;                      // +0x14
    CMemoryBlock(unsigned long size)
    {
        BlockSize = size + 0x10;
        NextBlock = 0;
        BlockData = (unsigned char*)AllocateAligned(BlockSize, 0x10);
        OwnsData = true;
    }
    CMemoryBlock(void* data, unsigned long size)
        : BlockData((unsigned char*)data)
    {
        BlockSize = size;
        NextBlock = 0;
        OwnsData = false;
    }
    virtual ~CMemoryBlock() { if (OwnsData) FreeAligned(BlockData); }   // 0x00A5C9B0 (deleting)
};

class CSimpleMemoryPool { public:
    void*         __vftable;                       // +0x00
    CMemoryBlock* FirstMemoryBlock;                // +0x04
    unsigned long Capacity;                        // +0x08
    unsigned long GrowthGranularity;               // +0x0c
    CArea         Root;                            // +0x10 (Root.NextFreeArea = free list head, +0x14)

    void AddFreeArea(CArea* area, unsigned long size)
    {
        area->Size = size;
        area->Status = AREA_FREE;
        area->NextFreeArea = Root.NextFreeArea;
        Root.NextFreeArea = area;
    }
    void AddBlockArea(CMemoryBlock* block)
    {
        AddFreeArea((CArea*)block->BlockData, block->BlockSize - 0x10);
    }
    __forceinline void DeleteBlocks()
    {
        CMemoryBlock* block = FirstMemoryBlock;
        if (block) {
            if (block->OwnsData) {
                do {
                    CMemoryBlock* next = block->NextBlock;
                    delete block;
                    block = next;
                } while (block);
            } else {
                delete block;
            }
            FirstMemoryBlock = 0;
        }
    }
    void Reset();                                  // 0x00A5C790
    void FreeAll();
    void SetMemoryPool(void* data, unsigned long size);
    void CreateNewMemoryBlock(unsigned long size);
    void* Allocate(unsigned long size);
};
void* CSimpleMemoryPool::Allocate(unsigned long size)
{
    size = (size + 15) / 16 * 16;
    CArea* prev = &Root;
    for (CArea* area = Root.NextFreeArea; area; area = area->NextFreeArea) {
        if (size <= area->Size) {
            prev->NextFreeArea = area->NextFreeArea;
            unsigned long remaining = area->Size - size;
            area->Status = AREA_USED;
            if (remaining > 0x10) {
                AddFreeArea((CArea*)((unsigned char*)area + size + 0x10), remaining - 0x10);
                area->Size = size;
            }
            return (unsigned char*)area + 0x10;
        }
        prev = area;
    }
    unsigned long grow = Capacity;
    if (FirstMemoryBlock)
        grow = GrowthGranularity;
    CreateNewMemoryBlock(grow > size ? grow : size);
    return Allocate(size);
}