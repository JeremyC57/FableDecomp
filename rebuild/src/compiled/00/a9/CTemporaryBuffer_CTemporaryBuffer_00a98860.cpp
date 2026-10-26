// CTemporaryBuffer::CTemporaryBuffer(unsigned long size, CHandleBasedMemoryPool* pool, CScratchBuffer* scratch) -- a scoped buffer: taken from the scratch buffer when it is big enough and free, else from the handle-based pool (allocate, lock, resolve), else from the heap with new[]
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A98860, 170 bytes (+6 int3). `ret 0Ch`. CTemporaryBuffer vtable 0x0129DFD8.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable() 0x0099A2F0; CScratchBuffer::Lock(void**) 0x009F4980 (manifest
//          "CNavSwitchableLeafNode::GetRegion" is propagated: it hands out Buffer when not Locked and sets Locked);
//          CHandleBasedMemoryPool::Allocate(long*, unsigned long, unsigned long) 0x00A03180, ::Lock(long) 0x00A026F0,
//          ::GetPointerFromHandle(long) 0x00A026B0; operator new[] 0x00BFEB22.
// Shape: the scratch pointer is loaded and tested first (the test scheduled above the member stores), the pool and size are
//        held in ecx/ebp; Data (+4, its address kept in ebx for the three final stores) is zeroed and the two pointers stored
//        before the branches. Three return paths each store AllocateType (1 scratch, 0 pool, 2 heap) and return `this`; the
//        handle's address (edi) escapes to Allocate and is re-read for Lock and GetPointerFromHandle.
// Layout: Ego_r.pdb CTemporaryBuffer (0x18): Data +0x04, MemoryPool +0x08, MemoryHandle +0x0c, ScratchBuffer +0x10,
//         AllocateType +0x14.
void* __cdecl operator new[](unsigned int size);   // 0x00BFEB22
struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                   // 0x0099A2F0
};
class CScratchBuffer { public:
    unsigned char _pad00[8];
    unsigned char* Buffer;                     // +0x08
    bool           Locked;                     // +0x0c
    long           NoBytes;                    // +0x10
    bool Lock(void** ppBuffer);                // 0x009F4980
};
class CHandleBasedMemoryPool { public:
    bool  Allocate(long* handle, unsigned long size, unsigned long flags);   // 0x00A03180
    void  Lock(long handle);                   // 0x00A026F0
    void* GetPointerFromHandle(long handle);   // 0x00A026B0
};
struct CTemporaryBuffer : CBaseClassNonCopyable {   // vtable 0x0129DFD8
    enum EAllocateType { ALLOCATE_POOL = 0, ALLOCATE_SCRATCH = 1, ALLOCATE_HEAP = 2 };
    void*                   Data;              // +0x04
    CHandleBasedMemoryPool* MemoryPool;        // +0x08
    long                    MemoryHandle;      // +0x0c
    CScratchBuffer*         ScratchBuffer;     // +0x10
    EAllocateType           AllocateType;      // +0x14
    virtual void v00();
    CTemporaryBuffer(unsigned long size, CHandleBasedMemoryPool* pool, CScratchBuffer* scratch);
};
CTemporaryBuffer::CTemporaryBuffer(unsigned long size, CHandleBasedMemoryPool* pool, CScratchBuffer* scratch)
    : Data(0), MemoryPool(pool), ScratchBuffer(scratch)
{
    if (scratch && size <= (unsigned long)scratch->NoBytes && !scratch->Locked) {
        scratch->Lock(&Data);
        AllocateType = ALLOCATE_SCRATCH;
    } else if (pool && pool->Allocate(&MemoryHandle, size, 0)) {
        MemoryPool->Lock(MemoryHandle);
        Data = MemoryPool->GetPointerFromHandle(MemoryHandle);
        AllocateType = ALLOCATE_POOL;
    } else {
        Data = new unsigned char[size];
        AllocateType = ALLOCATE_HEAP;
    }
}