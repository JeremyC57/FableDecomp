// CTemporaryBuffer::~CTemporaryBuffer() -- gives the buffer back to wherever AllocateType says it came from (pool: unlock + free the handle; scratch buffer: unlock it; heap: delete[] Data), then the CBase destructor
// Leaf: ??1CTemporaryBuffer
// Retail: Fable.exe @ 0x00A98910, 91 bytes. Vtable 0x0129DFD8 (CTemporaryBuffer, rtti-vtables.tsv).
// Callees: CHandleBasedMemoryPool::Unlock(long) @ 0x00A02710, CHandleBasedMemoryPool::Free(long) @ 0x00A02B30,
// CScratchBuffer::Unlock() @ 0x009F49A0 (`mov byte ptr [ecx+0Ch],0` = Locked = false; labelled "CXMVPlaybackInfo"),
// operator delete[] @ 0x00BFEB1C, CBase::~CBase @ 0x0099A300 (tail-jumped from every path).
// Layout: PDB CTemporaryBuffer (0x18 bytes, CBase vptr +0): Data +4, MemoryPool +8, MemoryHandle +0xc,
// ScratchBuffer +0x10, AllocateType +0x14; no engine header, modelled locally with the PDB names.
// Shape: a three-case `switch` lowered to `sub eax,0 ; je / dec eax ; je / dec eax ; jne` with the case bodies laid out
// last-first; the pool and handle are re-read for the second pool call; each case ends in its own copy of the base
// destructor tail-jump.
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C (the lane <new> lacks it)
struct CHandleBasedMemoryPool {
    void Unlock(long handle);                      // 0x00A02710
    void Free(long handle);                        // 0x00A02B30
};

struct CScratchBuffer {
    void Unlock();                                 // 0x009F49A0
};

namespace N_0099a300 { struct CBase {
    virtual void v00();
    ~CBase();                                      // 0x0099A300
}; } using N_0099a300::CBase;

enum EAllocateType {
    ALLOCATE_FROM_MEMORY_POOL = 0,
    ALLOCATE_FROM_SCRATCH_BUFFER = 1,
    ALLOCATE_FROM_HEAP = 2
};

class CTemporaryBuffer : public CBase {            // vtable 0x0129DFD8
public:
    void*                   Data;                  // +0x04
    CHandleBasedMemoryPool* MemoryPool;            // +0x08
    long                    MemoryHandle;          // +0x0c
    CScratchBuffer*         ScratchBuffer;         // +0x10
    EAllocateType           AllocateType;          // +0x14
    virtual void v00();
    ~CTemporaryBuffer();
};

CTemporaryBuffer::~CTemporaryBuffer()
{
    switch (AllocateType) {
    case ALLOCATE_FROM_MEMORY_POOL:
        MemoryPool->Unlock(MemoryHandle);
        MemoryPool->Free(MemoryHandle);
        break;
    case ALLOCATE_FROM_SCRATCH_BUFFER:
        ScratchBuffer->Unlock();
        break;
    case ALLOCATE_FROM_HEAP:
        delete[] (char*)Data;
        break;
    }
}