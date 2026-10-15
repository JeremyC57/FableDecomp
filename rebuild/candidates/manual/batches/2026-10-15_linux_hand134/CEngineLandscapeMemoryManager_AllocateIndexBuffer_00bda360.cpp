// CEngineLandscapeMemoryManager::AllocateIndexBuffer(long owner, unsigned long count, unsigned long flags, long format, long primitiveType) -- after the disposal-list step, allocates a CIndexBuffer from the global allocator and initialises it in pool 1; null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDA360, length 107 bytes (`ret 14h`; the first argument is unused).
// Callees: the manager's disposal-list step @ 0x00BDA150 (thiscall; manifest "CDrawResourcePool_FlushCycle"), GFCreateGlobalAllocator
// @ 0x009F2870, GFEnterAllocatorLock / GFLeaveAllocatorLock @ 0x009F2000 / 0x009F2010, CMemoryAllocator::InternalAllocate(0x14, 0, 0)
// @ 0x009F2A40, CIndexBuffer::CIndexBuffer @ 0x00AC1EC0, CIndexBuffer::Initialise(count, flags, format, pool, primitive type) @ 0x00AC1F70.
// Shape: `new CIndexBuffer` through a class operator new whose body is the landed AllocVirtual (0x00BD9AF0) -- the new-expression's
// null test guards the out-of-line constructor, the result is tested again for the early `return 0`; arguments re-read from their slots.
struct CMemoryAllocator {
    void* InternalAllocate(long size, unsigned long alignment, bool flag);   // 0x009F2A40
};

void __cdecl GFCreateGlobalAllocator();            // 0x009F2870
void __cdecl GFEnterAllocatorLock();               // 0x009F2000
void __cdecl GFLeaveAllocatorLock();               // 0x009F2010

extern CMemoryAllocator* g_pGlobalAllocator_013caa90;   // 0x013CAA90

inline void* AllocFromGlobalAllocator(unsigned int size)   // the landed AllocVirtual body (0x00BD9AF0)
{
    if (!g_pGlobalAllocator_013caa90)
        GFCreateGlobalAllocator();
    GFEnterAllocatorLock();
    void* block = g_pGlobalAllocator_013caa90->InternalAllocate(size, 0, false);
    GFLeaveAllocatorLock();
    return block;
}

class CIndexBuffer {                               // 0x14 bytes
public:
    unsigned char _body[0x14];
    CIndexBuffer();                                // 0x00AC1EC0
    void Initialise(unsigned long count, unsigned long flags, long format, long pool, long primitiveType);   // 0x00AC1F70
    static void* operator new(unsigned int size) { return AllocFromGlobalAllocator(size); }
};

struct CEngineLandscapeMemoryManager {
    void ProcessDisposalLists();                   // 0x00BDA150 (manifest "CDrawResourcePool_FlushCycle")
    CIndexBuffer* AllocateIndexBuffer(long owner, unsigned long count, unsigned long flags, long format, long primitiveType);
};

CIndexBuffer* CEngineLandscapeMemoryManager::AllocateIndexBuffer(long owner, unsigned long count, unsigned long flags, long format, long primitiveType)
{
    ProcessDisposalLists();
    CIndexBuffer* buffer = new CIndexBuffer;
    if (!buffer)
        return 0;
    buffer->Initialise(count, flags, format, 1, primitiveType);
    return buffer;
}
