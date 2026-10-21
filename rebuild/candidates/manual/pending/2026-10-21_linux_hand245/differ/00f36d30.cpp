// CGPUScratchBufferTextureManager::EndShutdown() -- after a shutdown, recreate every resource entry's texture (type 1) or depth-buffer surface (type 2); on the first failure free everything again and report false
// Grade: DIFFER(ours v retail) -- register allocation: ebx/ebp swapped for the index and the byte offset, the new texture lands in ecx (retail eax), and the early `return true` gets its own epilogue instead of sharing the false tail
// Leaf: EndShutdown
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F36D30, length 385 bytes (`ret`, bool in al).
// Callees: operator new @ 0x00BFEA1A, CTexture::Initialise(const C2DExtentsI*, long, const CPixelFormat*, ulong, ESurfacePool,
//   bool, bool) @ 0x009FA280, CSurface::InitialiseAsDepthBuffer(const C2DExtentsI*, const CPixelFormat*,
//   ESurfaceMultisampleType, bool) @ 0x009F32A0, CTexture::Uninitialise @ 0x009F9F70 (the inline ~CTexture), operator delete
//   @ 0x00BFE9BC, the surface's virtual deleting destructor (slot 0; CSurface vtable 0x0122F84C).
// Layout: the landed BeginShutdown 0x00F37170 model (retail 12-byte arrays, 44-byte resource entries, ShutdownFlag +0x40).
// Shape: each pass re-derives Resources.Size() (the 0x2E8BA2E9 divide by 44); the failure path walks the entries again
//   deleting texture and surface and clearing both pointers.
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
struct IDirect3DTexture9;
struct IDirect3DSurface9;
struct C2DExtentsI { long X, Y; };
struct CPixelFormat { long FormatIndex; };
struct CScratchTexture {                                // CTexture
    IDirect3DTexture9* PD3DTexture;
    unsigned long      Bits;
    CScratchTexture() : PD3DTexture(0), Bits(0) {}
    ~CScratchTexture() { Uninitialise(); }
    void Uninitialise();                                // 0x009F9F70
    bool Initialise(const C2DExtentsI* size, long levels, const CPixelFormat* format, unsigned long usage, long pool, bool a, bool b);   // 0x009FA280
};
struct CScratchSurface {                                // CSurface, vtable 0x0122F84C
    virtual ~CScratchSurface();
    IDirect3DSurface9* PD3DSurface;                     // +0x04
    unsigned long      AllocationSource;                // +0x08
    void*              PAllocatedMemory;                // +0x0C
    CScratchSurface() : PD3DSurface(0), AllocationSource(0) {}
    bool InitialiseAsDepthBuffer(const C2DExtentsI* size, const CPixelFormat* format, long multiSampleType, bool b);   // 0x009F32A0
};
struct CScratchResource {                               // 44 bytes
    int              ResourceType;                      // +0x00
    CScratchTexture* Texture;                           // +0x04
    CScratchSurface* Surface;                           // +0x08
    int              PageIndex;                         // +0x0C
    C2DExtentsI      Size;                              // +0x10
    int              NoLevels;                          // +0x18
    CPixelFormat     PixelFormat;                       // +0x1C
    int              MultiSampleType;                   // +0x20
    unsigned         UsageFlags;                        // +0x24
    int              MemoryPool;                        // +0x28
};
template<class T> struct CScratchArray {
    T* Begin; T* End; T* Capacity;
    unsigned Size() const { return unsigned(End - Begin); }
    T& operator[](unsigned i) { return Begin[i]; }
};
struct CScratchPage;
struct CGPUScratchBufferTextureManager {
    void* VTable;
    CScratchArray<CScratchResource> Resources;          // +0x04
    CScratchArray<CScratchPage>     MemoryPages;        // +0x10
    CScratchArray<int> ScratchPools, FreeTexturePageSlots, FreeTextureSlots;   // +0x1C, +0x28, +0x34
    bool ShutdownFlag;                                  // +0x40
    bool EndShutdown();
};

bool CGPUScratchBufferTextureManager::EndShutdown()
{
    if (!ShutdownFlag)
        return true;
    for (unsigned i = 0; i < Resources.Size(); ++i) {
        CScratchResource& r = Resources[i];
        bool ok;
        if (r.ResourceType == 1) {
            CScratchTexture* texture = new CScratchTexture;
            r.Texture = texture;
            ok = texture->Initialise(&r.Size, r.NoLevels, &r.PixelFormat, r.UsageFlags, r.MemoryPool, false, true);
        } else if (r.ResourceType == 2) {
            CScratchSurface* surface = new CScratchSurface;
            r.Surface = surface;
            ok = surface->InitialiseAsDepthBuffer(&r.Size, &r.PixelFormat, r.MultiSampleType, true);
        } else
            continue;
        if (!ok) {
            for (unsigned j = 0; j < Resources.Size(); ++j) {
                CScratchResource& e = Resources[j];
                delete e.Texture;
                delete e.Surface;
                e.Texture = 0;
                e.Surface = 0;
            }
            return false;
        }
    }
    ShutdownFlag = false;
    return true;
}
