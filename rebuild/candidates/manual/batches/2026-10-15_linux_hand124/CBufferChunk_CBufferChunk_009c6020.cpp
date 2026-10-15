// CBufferChunk::~CBufferChunk() -- release the chunk's memory block back to the global allocator (the inlined GFFreeFromGlobalAllocator), tearing the allocator down when it is flagged to die once empty
// Leaf: ??1CBufferChunk
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C6020, length 98 bytes.
// Callees: GFEnterAllocatorLock @ 0x009F2000 / GFLeaveAllocatorLock @ 0x009F2010 (the tail jump),
// CMemoryAllocator::InternalDeallocate(void*) @ 0x009F2920, CMemoryAllocator::GetAllocationCount() @ 0x009F1F10,
// CMemoryAllocator::~CMemoryAllocator @ 0x009F2310, the class's `__fastcall` operator delete @ 0x009F1F90.
// Globals: the allocator @ 0x013CAA90, the destroy-when-empty flag @ 0x013CAA94.
// Layout = Ego_r.pdb CBufferChunk (8 bytes): Mem +0, Used +4, IsInput +5.
// Shape: `if (Mem) GFFreeFromGlobalAllocator(Mem);` with the landed free function (0x009A4350) inlined: Mem is read
// once into esi, the early `pop esi ; ret` is the outer null test, the inlined body re-tests the block after taking
// the lock, `delete` of the global is the null-tested destructor + class operator delete pair, and the lock release is
// the tail jump.
struct CMemoryAllocator {
    void InternalDeallocate(void* block);     // 0x009F2920
    long GetAllocationCount();                // 0x009F1F10
    ~CMemoryAllocator();                      // 0x009F2310
    static void __fastcall operator delete(void* block);   // 0x009F1F90
};

void __cdecl GFEnterAllocatorLock();          // 0x009F2000
void __cdecl GFLeaveAllocatorLock();          // 0x009F2010

extern CMemoryAllocator* g_pGlobalAllocator_013caa90;          // 0x013CAA90
extern bool              g_bDestroyAllocatorWhenEmpty_013caa94; // 0x013CAA94

__forceinline void __fastcall GFFreeFromGlobalAllocator(void* block)   // 0x009A4350, inlined here (a plain `inline` stays out of line)
{
    GFEnterAllocatorLock();
    if (block && g_pGlobalAllocator_013caa90) {
        g_pGlobalAllocator_013caa90->InternalDeallocate(block);
        if (g_bDestroyAllocatorWhenEmpty_013caa94) {
            if (g_pGlobalAllocator_013caa90->GetAllocationCount() == 0) {
                delete g_pGlobalAllocator_013caa90;
                g_pGlobalAllocator_013caa90 = 0;
            }
        }
    }
    GFLeaveAllocatorLock();
}

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
        GFFreeFromGlobalAllocator(Mem);
}
