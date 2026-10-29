// CContainmentVolume::LoadBinary(CDataInputStream& stream, CMovableResourceMemoryPool* pool) -- read the plane count, allocate count*16 bytes from the movable pool and read the CPlane array into it
// Leaf: LoadBinary
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A754E0, 178 bytes (192 with padding), `ret 8`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the inlined 4-byte Read's slow path),
//   CMovableResourceMemoryPool::AllocateResource(long*, CMovableResource*, ulong, ulong) 0x00A62E40,
//   CHandleBasedMemoryPool::GetPointerFromHandle(long) 0x00A026B0, the stream's virtual Read (vtable slot +0x10).
// Layout: engine/CContainmentVolume.h (ContainmentPlanes +4, Size +8, MemoryPool +0xc). The stream is a local model of
//   CDataInputStream (engine header layout: StreamPos +4, CurrentSourcePtr +0xc, SourceChunkBytesRemaining +0x14)
//   because the engine header models its vptr as a plain void* and this body makes a virtual call.
// Shape: the inlined Read keeps the count in eax on the fast path; the count (an inline ReadULong returning by value) and the allocation handle both live in
//   the dead `stream` argument slot; `Size > 0` gives the unsigned `jbe`.
#include "engine/CContainmentVolume.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDataInputStream { public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void Read(void* buffer, long size);                    // vtable +0x10
    unsigned long StreamPos;                                       // +0x04
    long          StreamSize;                                      // +0x08
    void*         CurrentSourcePtr;                                // +0x0c
    unsigned long SourceChunkStreamPos;                            // +0x10
    long          SourceChunkBytesRemaining;                       // +0x14
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void ReadInline(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline unsigned long ReadULong()
    {
        unsigned long value;
        ReadInline(&value, 4);
        return value;
    }
private:
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
};

class CHandleBasedMemoryPool { public:
    void* GetPointerFromHandle(long handle);                       // 0x00A026B0
};

class CMovableResourceMemoryPool : public CHandleBasedMemoryPool { public:
    bool AllocateResource(long* handle, void* owner, unsigned long size, unsigned long alignment);   // 0x00A62E40
};

struct CContainmentVolume_Methods : CContainmentVolume {
    bool LoadBinary(CDataInputStream& stream, CMovableResourceMemoryPool* pool);
};

bool CContainmentVolume_Methods::LoadBinary(CDataInputStream& stream, CMovableResourceMemoryPool* pool)
{
    Size = stream.ReadULong();
    MemoryPool = pool;
    if (Size > 0) {
        long handle;
        if (!pool->AllocateResource(&handle, this, Size * 16, 0))
            return false;
        ContainmentPlanes = (CPlane*)MemoryPool->GetPointerFromHandle(handle);
        stream.Read(ContainmentPlanes, Size * 16);
    }
    else
        ContainmentPlanes = 0;
    return true;
}