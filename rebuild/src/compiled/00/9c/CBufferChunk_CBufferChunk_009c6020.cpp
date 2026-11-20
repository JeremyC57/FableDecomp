// CBufferChunk::~CBufferChunk() -- release the chunk's memory block back to the global allocator (the inlined GFFreeFromGlobalAllocator), tearing the allocator down when it is flagged to die once empty
// Leaf: ??1CBufferChunk
// Inline-budget: FABLE_FORCEINLINE on the header inlines CMemoryAllocator::Deallocate and Destroy (engine/CMemoryAllocator.h opt-in
//   FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE; the landed 0x009F2B20 forces both the same way, and the HEAD rows forced their GFFree
//   helper) -- retail expands both at every site here; plain inline is called out of line (0x00BD9B90: Destroy DIFFER 68v97, then
//   Deallocate DIFFER 16v97). Preference-2 per-row /d2inl flags were not swept.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C6020, length 98 bytes.
// Callees: CMemoryAllocator::EnterSynchronisation @ 0x009F2000 / CMemoryAllocator::LeaveSynchronisation @ 0x009F2010 (the tail jump),
// CMemoryAllocator::InternalDeallocate(void*) @ 0x009F2920, CMemoryAllocator::GetAllocationCount() @ 0x009F1F10,
// CMemoryAllocator::~CMemoryAllocator @ 0x009F2310, the class's `__fastcall` operator delete @ 0x009F1F90.
// Globals: the allocator @ 0x013CAA90, the destroy-when-empty flag @ 0x013CAA94.
// Layout = Ego_r.pdb CBufferChunk (8 bytes): Mem +0, Used +4, IsInput +5.
// Shape: `if (Mem) CMemoryAllocator::Deallocate(Mem);` with the landed free function (0x009A4350) inlined: Mem is read
// once into esi, the early `pop esi ; ret` is the outer null test, the inlined body re-tests the block after taking
// the lock, `delete` of the global is the null-tested destructor + class operator delete pair, and the lock release is
// the tail jump.
#include "fable_compiler.h"
// NF140 (q4197 / q4198): the allocation is the inline public static CMemoryAllocator::Allocate(size, flags) of engine/CMemoryAllocator.h
// (Ego_r.pdb / FableWin.pdb ?Allocate@CMemoryAllocator@@SIPAXKK@Z, inline in release): ?Create@CMemoryAllocator@@KIXXZ 0x009F2870, the protected
// statics ?EnterSynchronisation@CMemoryAllocator@@KIXXZ 0x009F2000 / ?LeaveSynchronisation@CMemoryAllocator@@KIXXZ 0x009F2010 (both PDBs),
// PMemoryAllocator 0x013CAA90 and InternalAllocate 0x009F2A40, as the landed 0x009A3640 binds them.
// NF140 (q4197 / q4198): the frees are the inline public static CMemoryAllocator::Deallocate(block) (FableWin.pdb ?Deallocate@CMemoryAllocator@@SIXPAX@Z,
// out of line at 0x009A4350; engine/CMemoryAllocator.h opt-in FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE, the body of the landed 0x009F2B20).
#define FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE
#include "engine/CMemoryAllocator.h"


class CBufferChunk {
public:
    unsigned char* Mem;                       // +0x00
    bool           Used;                      // +0x04
    bool           IsInput;                   // +0x05
    ~CBufferChunk();
};

CBufferChunk::~CBufferChunk()
{
    if (Mem)
        CMemoryAllocator::Deallocate(Mem);
}