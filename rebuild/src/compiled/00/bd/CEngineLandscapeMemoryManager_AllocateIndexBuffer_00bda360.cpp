// CEngineLandscapeMemoryManager::AllocateIndexBuffer(long owner, unsigned long count, unsigned long flags, long format, long primitiveType) -- after the disposal-list step, allocates a CIndexBuffer from the global allocator and initialises it in pool 1; null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDA360, length 107 bytes (`ret 14h`; the first argument is unused).
// Callees: CEngineLandscapeMemoryManager::ProcessDisposalList @ 0x00BDA150 (private, thiscall; manifest "CDrawResourcePool_FlushCycle"), CMemoryAllocator::Create
// @ 0x009F2870, CMemoryAllocator::EnterSynchronisation / CMemoryAllocator::LeaveSynchronisation @ 0x009F2000 / 0x009F2010, CMemoryAllocator::InternalAllocate(0x14, 0, 0)
// @ 0x009F2A40, CIndexBuffer::CIndexBuffer @ 0x00AC1EC0, CIndexBuffer::Initialise(count, flags, format, pool, primitive type) @ 0x00AC1F70.
// Shape: `new CIndexBuffer` through a class operator new whose body is the landed AllocVirtual (0x00BD9AF0) -- the new-expression's
// null test guards the out-of-line constructor, the result is tested again for the early `return 0`; arguments re-read from their slots.
// NF140 (q4197 / q4198): the allocation is the inline public static CMemoryAllocator::Allocate(size, flags) of engine/CMemoryAllocator.h
// (Ego_r.pdb / FableWin.pdb ?Allocate@CMemoryAllocator@@SIPAXKK@Z, inline in release): ?Create@CMemoryAllocator@@KIXXZ 0x009F2870, the protected
// statics ?EnterSynchronisation@CMemoryAllocator@@KIXXZ 0x009F2000 / ?LeaveSynchronisation@CMemoryAllocator@@KIXXZ 0x009F2010 (both PDBs),
// PMemoryAllocator 0x013CAA90 and InternalAllocate 0x009F2A40, as the landed 0x009A3640 binds them.
#include "engine/CMemoryAllocator.h"


inline void* AllocFromGlobalAllocator(unsigned int size)   // the landed AllocVirtual body (0x00BD9AF0)
{
    void* block = CMemoryAllocator::Allocate(size, 0);
    return block;
}

class CIndexBuffer {                               // 0x14 bytes
public:
    unsigned char _body[0x14];
    CIndexBuffer();                                // 0x00AC1EC0
    void Initialise(unsigned long count, unsigned long flags, long format, long pool, long primitiveType);   // 0x00AC1F70
    static void* operator new(unsigned int size) { return AllocFromGlobalAllocator(size); }
};

class CEngineLandscapeMemoryManager {
private:
    void ProcessDisposalList();                    // 0x00BDA150 (Ego_r.pdb / FableWin.pdb ?ProcessDisposalList@CEngineLandscapeMemoryManager@@AAEXXZ, private; manifest "CDrawResourcePool_FlushCycle")
public:
    CIndexBuffer* AllocateIndexBuffer(long owner, unsigned long count, unsigned long flags, long format, long primitiveType);
};

CIndexBuffer* CEngineLandscapeMemoryManager::AllocateIndexBuffer(long owner, unsigned long count, unsigned long flags, long format, long primitiveType)
{
    ProcessDisposalList();
    CIndexBuffer* buffer = new CIndexBuffer;
    if (!buffer)
        return 0;
    buffer->Initialise(count, flags, format, 1, primitiveType);
    return buffer;
}