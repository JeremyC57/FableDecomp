// CFileInstaller::InitBuffers(unsigned long chunkSize, unsigned long chunkCount) -- record the chunk size/count (+0x64/+0x68), free the old buffers, size the BufferChunks array, allocate every chunk's memory from the global allocator, mark them all available, and GlobalAlloc the read and write buffers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C5310, length 139 bytes (+ int3 padding).
// Callees: CFileInstaller::FreeBuffers @ 0x009C5110 (landed; catalog label "~CPatchTesselationEdgeStrip" is propagated),
// CArray<CBufferChunk>::Resize(unsigned long) @ 0x009C7850 (label "vector<CIntelligentPointer<CParticleEmitter>>::resize" propagated),
// the inlined GFAllocateFromGlobalAllocator: GFCreateGlobalAllocator @ 0x009F2870 (global allocator 0x013CAA90 null),
// GFEnterAllocatorLock @ 0x009F2000, CMemoryAllocator::InternalAllocate(long, unsigned long, bool) @ 0x009F2A40, GFLeaveAllocatorLock @ 0x009F2010;
// KERNEL32 GlobalAlloc (IAT 0x0143FDD4, loaded once into esi).
// Called by the landed CFileInstaller::SetBuffers 0x009C5650 (both arguments forwarded under the lock).
// Layout: Ego_r.pdb CFileInstaller with the retail 12-byte CArray (every member after BufferChunks sits 4 bytes low, and 0x20 low
// past the three 8-byte retail lists): BufferChunks +0x1c, PReadBuffer +0x28, PWriteBuffer +0x2c, AvailableBuffers +0x30,
// BufferChunkSize +0x64, NoBufferChunks +0x68. CBufferChunk = {Mem, Used, IsInput} (8 bytes).
// Shape: the allocation as in the landed AllocVirtual 0x00BD9AF0 (inlined): the block is homed in the dead chunkSize argument slot
// across the unlock, the element store re-reads BufferChunks' first pointer; the unsigned count loop (`jbe`/`jb`).
extern "C" __declspec(dllimport) void* __stdcall GlobalAlloc(unsigned int flags, unsigned long bytes);   // IAT 0x0143FDD4

class CMemoryAllocator { public:
    void* InternalAllocate(long size, unsigned long alignment, bool flag);   // 0x009F2A40
};

void __cdecl GFCreateGlobalAllocator();            // 0x009F2870
void __cdecl GFEnterAllocatorLock();               // 0x009F2000
void __cdecl GFLeaveAllocatorLock();               // 0x009F2010

extern CMemoryAllocator* g_pGlobalAllocator_013caa90;   // 0x013CAA90

__forceinline void* GFAllocateFromGlobalAllocator(long size)
{
    if (!g_pGlobalAllocator_013caa90)
        GFCreateGlobalAllocator();
    GFEnterAllocatorLock();
    void* block = g_pGlobalAllocator_013caa90->InternalAllocate(size, 0, false);
    GFLeaveAllocatorLock();
    return block;
}

struct CBufferChunk {
    unsigned char* Mem;                            // +0x00
    bool           Used;                           // +0x04
    bool           IsInput;                        // +0x05
};

struct CBufferChunkArray {                         // retail CArray<CFileInstaller::CBufferChunk>: first / last / end
    CBufferChunk* m_pFirst;
    CBufferChunk* m_pLast;
    CBufferChunk* m_pEnd;
    void Resize(unsigned long count);              // 0x009C7850
};

struct CRITICAL_SECTION_Fable { unsigned char _body[0x18]; };

class CFileInstaller { public:
    virtual void v00();
    CRITICAL_SECTION_Fable CriticalSection;        // +0x04
    CBufferChunkArray      BufferChunks;           // +0x1c
    unsigned char*         PReadBuffer;            // +0x28
    unsigned char*         PWriteBuffer;           // +0x2c
    long                   AvailableBuffers;       // +0x30
    unsigned char          _pad34[0x30];
    unsigned long          BufferChunkSize;        // +0x64
    unsigned long          NoBufferChunks;         // +0x68
    void FreeBuffers();                            // 0x009C5110
    void InitBuffers(unsigned long chunkSize, unsigned long chunkCount);
};

void CFileInstaller::InitBuffers(unsigned long chunkSize, unsigned long chunkCount)
{
    BufferChunkSize = chunkSize;
    NoBufferChunks = chunkCount;
    FreeBuffers();
    BufferChunks.Resize(chunkCount);
    for (unsigned long i = 0; i < chunkCount; ++i)
        BufferChunks.m_pFirst[i].Mem = (unsigned char*)GFAllocateFromGlobalAllocator(chunkSize);
    AvailableBuffers = chunkCount;
    PReadBuffer = (unsigned char*)GlobalAlloc(0, chunkSize);
    PWriteBuffer = (unsigned char*)GlobalAlloc(0, chunkSize);
}