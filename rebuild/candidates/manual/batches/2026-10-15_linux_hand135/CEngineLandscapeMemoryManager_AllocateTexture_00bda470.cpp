// CEngineLandscapeMemoryManager::AllocateTexture(unsigned long id, const C2DExtentsI& size, long levels, const CPixelFormat& format, unsigned long usage, bool dynamic) -- flushes the disposal cycle, allocates a CTexture from the global allocator and initialises it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDA470, length 120 bytes.
// Callees: CEngineLandscapeMemoryManager::FlushCycle @ 0x00BDA150 (thiscall on `this`, manifest
// "CDrawResourcePool_FlushCycle"), GFCreateGlobalAllocator @ 0x009F2870 (when the global allocator
// @ 0x013CAA90 is null), GFEnterAllocatorLock @ 0x009F2000 / GFLeaveAllocatorLock @ 0x009F2010
// (manifest "_Dest_val<...CFlash>" labels are propagated, the landed AllocVirtual @ 0x00BD9AF0 names
// them), CMemoryAllocator::InternalAllocate(8, 0, false) @ 0x009F2A40, CTexture::Initialise @ 0x009FA280.
// Shape: CTexture has a class operator new that goes to the landscape allocator under its lock (same
// body as AllocVirtual, inlined), the inline constructor zeroes both words; VC merges the `new` null
// test with the early `return 0`, and schedules the two zero stores after the seven argument pushes.
// The pool argument is `~usage & 1` (bit 0 of usage = default pool, else managed), the last flag is 0.
// `id` (arg 1) is unused, `ret 18h` = six arguments. Layout: the PDB header puts the disposal lists at
// +0x10.., FlushCycle reads +0x34/+0x40, so the manager is modelled locally (only the member call is needed).
struct C2DExtentsI;
struct CPixelFormat;
enum ESurfacePool { SURFACE_POOL_DEFAULT = 0, SURFACE_POOL_MANAGED = 1 };

struct CMemoryAllocator {
    void* InternalAllocate(long size, unsigned long alignment, bool flag);   // 0x009F2A40
};

void __cdecl GFCreateGlobalAllocator();            // 0x009F2870
void __cdecl GFEnterAllocatorLock();               // 0x009F2000
void __cdecl GFLeaveAllocatorLock();               // 0x009F2010

extern CMemoryAllocator* g_pGlobalAllocator_013caa90;   // 0x013CAA90

struct IDirect3DTexture9;

class CTexture {
public:
    IDirect3DTexture9* PD3DTexture;                // +0x00
    unsigned long      ByteLengthAndSource;        // +0x04 ByteLength:28 | AllocationSource:4
    CTexture() : PD3DTexture(0), ByteLengthAndSource(0) { }
    static void* operator new(unsigned int size)
    {
        if (!g_pGlobalAllocator_013caa90)
            GFCreateGlobalAllocator();
        GFEnterAllocatorLock();
        void* block = g_pGlobalAllocator_013caa90->InternalAllocate(size, 0, false);
        GFLeaveAllocatorLock();
        return block;
    }
    bool Initialise(const C2DExtentsI& size, long levels, const CPixelFormat& format, unsigned long usage,
                    ESurfacePool pool, bool dynamic, bool renderTarget);   // 0x009FA280
};

struct CEngineLandscapeMemoryManager {
    void FlushCycle();                             // 0x00BDA150
    CTexture* AllocateTexture(unsigned long id, const C2DExtentsI& size, long levels, const CPixelFormat& format,
                              unsigned long usage, bool dynamic);
};

CTexture* CEngineLandscapeMemoryManager::AllocateTexture(unsigned long id, const C2DExtentsI& size, long levels,
                                                         const CPixelFormat& format, unsigned long usage, bool dynamic)
{
    FlushCycle();
    CTexture* texture = new CTexture;
    if (!texture)
        return 0;
    texture->Initialise(size, levels, format, usage, (ESurfacePool)(~usage & 1), dynamic, false);
    return texture;
}
