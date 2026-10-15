// CSurface::InitialiseAsImage(const C2DExtentsI& size, const CPixelFormat& format, bool) -- release any held surface, then CreateOffscreenPlainSurface (D3DPOOL_SCRATCH) on the render device; true on success
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F3230, length 107 bytes (+5 int3 padding). `ret 0Ch` (the bool is unused).
// Callees: GFGetSystemManager @ 0x009A4EC0 (device = system +0x60 -> +0x58); IDirect3DSurface9::Release (vtable +8);
// IDirect3DDevice9::CreateOffscreenPlainSurface (vtable +0x90). The D3DFORMAT comes from the 0x24-byte pixel-format table
// @ 0x0129BA40 (`lea edx,[ecx+ecx*8]` then `*4`).
// Shape: the inline Uninitialise (out of line at 0x009F2E20) leaves &PD3DSurface in edi, reused as the out pointer. The extents
// are read into locals (ebx/ebp) BEFORE the device fetch; the format after it. `>= 0 ? true : false` gives `setge al`; the
// AllocationSource = 1 store sits between the call and the test.
// Model: engine/CSurface.h member names (+4 PD3DSurface, +8 AllocationSource, +0xC PAllocatedMemory); the generated header
// flattens the vptr into a plain word, so a polymorphic local model is used (as in the landed CRenderTarget rows).
struct IDirect3DSurface9 {
    virtual long __stdcall QueryInterface(void*, void**);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
};
struct IDirect3DDevice9 {
    virtual void __stdcall v00(); virtual void __stdcall v01(); virtual void __stdcall v02(); virtual void __stdcall v03();
    virtual void __stdcall v04(); virtual void __stdcall v05(); virtual void __stdcall v06(); virtual void __stdcall v07();
    virtual void __stdcall v08(); virtual void __stdcall v09(); virtual void __stdcall v10(); virtual void __stdcall v11();
    virtual void __stdcall v12(); virtual void __stdcall v13(); virtual void __stdcall v14(); virtual void __stdcall v15();
    virtual void __stdcall v16(); virtual void __stdcall v17(); virtual void __stdcall v18(); virtual void __stdcall v19();
    virtual void __stdcall v20(); virtual void __stdcall v21(); virtual void __stdcall v22(); virtual void __stdcall v23();
    virtual void __stdcall v24(); virtual void __stdcall v25(); virtual void __stdcall v26(); virtual void __stdcall v27();
    virtual long __stdcall CreateRenderTarget(unsigned long w, unsigned long h, long format, long multisample,
        unsigned long quality, int lockable, IDirect3DSurface9** out, void** shared);                       // +0x70
    virtual long __stdcall CreateDepthStencilSurface(unsigned long w, unsigned long h, long format, long multisample,
        unsigned long quality, int discard, IDirect3DSurface9** out, void** shared);                        // +0x74
    virtual void __stdcall v30(); virtual void __stdcall v31(); virtual void __stdcall v32(); virtual void __stdcall v33();
    virtual void __stdcall v34(); virtual void __stdcall v35();
    virtual long __stdcall CreateOffscreenPlainSurface(unsigned long w, unsigned long h, long format, long pool,
        IDirect3DSurface9** out, void** shared);                                                             // +0x90
};
struct CRenderSystem {
    unsigned char     _pad00[0x58];
    IDirect3DDevice9* m_pDevice;            // +0x58
};
struct CSystemManager {
    unsigned char  _pad00[0x60];
    CRenderSystem* m_pRender;               // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
struct SPixelFormatInfo { long d3dFormat; long rest[8]; };   // 0x24 bytes
extern SPixelFormatInfo g_pixelFormatInfo[];                 // 0x0129BA40
struct CPixelFormat { long m_format; };
struct C2DExtentsI { long Width, Height; };

struct CSurface {                               // vtable 0x0122F84C, 0x10 bytes
    virtual ~CSurface();
    IDirect3DSurface9* PD3DSurface;             // +0x04
    unsigned long AllocationSource;             // +0x08
    void* PAllocatedMemory;                     // +0x0c
    void Uninitialise()                         // out of line at 0x009F2E20
    {
        if (PD3DSurface) {
            PD3DSurface->Release();
            PD3DSurface = 0;
            AllocationSource = 0;
        }
    }
    bool InitialiseAsImage(const C2DExtentsI& size, const CPixelFormat& format, bool);
};

bool CSurface::InitialiseAsImage(const C2DExtentsI& size, const CPixelFormat& format, bool)
{
    Uninitialise();
    long width = size.Width;
    long height = size.Height;
    IDirect3DDevice9* device = GFGetSystemManager()->m_pRender->m_pDevice;
    long d3dFormat = g_pixelFormatInfo[format.m_format].d3dFormat;
    long hr = device->CreateOffscreenPlainSurface(width, height, d3dFormat, 3, &PD3DSurface, 0);
    AllocationSource = 1;
    return hr >= 0 ? true : false;
}