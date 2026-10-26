// CEngineLandscapeMemoryManager::AllocateVertexBuffer(long owner, long type, long count, long stride, unsigned long usage) -- after the disposal-list step, allocates a CVertexBufferWin32 from the global allocator (constructor inline) and initialises it in pool 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDA3D0, length 148 bytes (`ret 14h`; the first argument is unused; Initialise runs even
// on a failed allocation, as in retail).
// Callees: the disposal-list step @ 0x00BDA150, GFCreateGlobalAllocator / lock / unlock (0x009F2870 / 0x009F2000 / 0x009F2010),
// CMemoryAllocator::InternalAllocate(0x34, 0, 0) @ 0x009F2A40, CBase::CBase @ 0x0099A2F0, CVertexBufferWin32::Initialise @ 0x00A63150
// (return value ignored). Vtable 0x0129BA20 (CVertexBufferWin32).
// Layout: the landed CVertexBufferWin32 model (0x00A8E160): CBase root, CVertexBufferBase members PVertexBuffer +4, Length +8, Type +0xC,
// Stride +0x10, CompressionParams +0x14 (scale 1.0 x4, bias 0 x4); engine/CVertexBufferWin32.h only fixes the size.
// Shape: inlined, the constructor stores the vtable BEFORE PVertexBuffer/Stride, so those two are body assignments of the most
// derived constructor (in the base constructor body they land before the vtable store); ebx is the shared zero register.
class CMemoryAllocator { public:
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

struct CBase {                                     // the engine's polymorphic root (landed 0x00A8E160 model)
    virtual void VSlot0();
    CBase();                                       // 0x0099A2F0
    ~CBase();                                      // 0x0099A300
};
#include "engine/CVertexBufferWin32.h"             // PDB header: opaque flattened base, members modelled below
struct IDirect3DVertexBuffer9;
class CVertexCompressionParams { public:                  // PDB: PosScale XYZW then PosBias XYZW
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
    CVertexCompressionParams()
        : PosScaleX(1.0f), PosScaleY(1.0f), PosScaleZ(1.0f), PosScaleW(1.0f),
          PosBiasX(0.0f), PosBiasY(0.0f), PosBiasZ(0.0f), PosBiasW(0.0f) { }
};
struct CVertexBufferBase : CBase {                 // 0x34 bytes, PDB member names
    IDirect3DVertexBuffer9*  PVertexBuffer;        // +0x04
    unsigned long            Length;               // +0x08
    long                     Type;                 // +0x0c (EVertexType)
    unsigned char            Stride;               // +0x10
    CVertexCompressionParams CompressionParams;    // +0x14
    CVertexBufferBase() : Length(0), Type(0), CompressionParams() { }
};
struct CVertexBufferWin32_Retail : CVertexBufferBase {   // vtable 0x0129BA20
    virtual void VSlot1();
    CVertexBufferWin32_Retail()
    {
        PVertexBuffer = 0;
        Stride = 0;
    }
    bool Initialise(long type, long count, long stride, unsigned long usage, long pool);   // 0x00A63150
    static void* operator new(unsigned int size) { return AllocFromGlobalAllocator(size); }
};
FABLE_STATIC_ASSERT(sizeof(CVertexBufferWin32_Retail) == sizeof(CVertexBufferWin32));

class CEngineLandscapeMemoryManager { public:
    void ProcessDisposalLists();                   // 0x00BDA150 (manifest "CDrawResourcePool_FlushCycle")
    CVertexBufferWin32_Retail* AllocateVertexBuffer(long owner, long type, long count, long stride, unsigned long usage);
};

CVertexBufferWin32_Retail* CEngineLandscapeMemoryManager::AllocateVertexBuffer(long owner, long type, long count, long stride, unsigned long usage)
{
    ProcessDisposalLists();
    CVertexBufferWin32_Retail* buffer = new CVertexBufferWin32_Retail;
    buffer->Initialise(type, count, stride, usage, 1);
    return buffer;
}