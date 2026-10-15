// CTexture::GetSurfaceLevel(unsigned long level) -- returns a CSurface for one mip level: IDirect3DTexture9::GetSurfaceLevel (COM slot 18) fetches the level's surface, a local CSurface adopts it (InitialiseFromTextureMipmap) and is copied out
// Leaf: ?GetSurfaceLevel@CTexture
// Retail: Fable.exe @ 0x009F9E00, 96 bytes. Returns the CSurface through the hidden result pointer (`ret 8`).
// Callees: IDirect3DTexture9::GetSurfaceLevel (`call [ecx+48h]`, `this` pushed), CSurface::InitialiseFromTextureMipmap
// @ 0x009F2F10, CSurface::CSurface(const CSurface&) @ 0x009F2D60, CSurface::Uninitialise @ 0x009F2E20 (the inline
// destructor of the local); CSurface vtable 0x0122F84C.
// Label: the manifest's "CTexture::GetNoLevels" is wrong -- the body calls slot 18 (GetSurfaceLevel) with the level
// and an out-pointer and returns a CSurface by value; 62 callers.
// Shape: the out-pointer local is homed in the DEAD slot of `level` (`lea edx,[esp+1Ch]` pushed before `level`);
// the local CSurface's inline constructor zeroes PD3DSurface/AllocationSource before its vtable store; no NRVO, so the
// local is copied into the result with the copy constructor and destroyed (vtable store + Uninitialise).
// Layout: PDB CTexture (PD3DTexture +0, bitfield word +4) and CSurface (vptr, PD3DSurface +4, AllocationSource +8,
// PAllocatedMemory +0xc). engine/CTexture.h / engine/CSurface.h model the vptr / bitfields as plain bytes and cannot
// carry the constructors, so both are modelled locally with the PDB names (as the landed CSurface sources do).
struct IDirect3DSurface9;

struct IDirect3DTexture9 {                         // IUnknown + IDirect3DResource9 + IDirect3DBaseTexture9 + texture slots
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1
    virtual unsigned long __stdcall Release();                               // slot 2
    virtual long __stdcall GetDevice(void** device);                         // slot 3
    virtual long __stdcall SetPrivateData(const void* guid, const void* data, unsigned long size, unsigned long flags);
    virtual long __stdcall GetPrivateData(const void* guid, void* data, unsigned long* size);
    virtual long __stdcall FreePrivateData(const void* guid);
    virtual unsigned long __stdcall SetPriority(unsigned long priority);
    virtual unsigned long __stdcall GetPriority();
    virtual void __stdcall PreLoad();
    virtual long __stdcall GetType();
    virtual unsigned long __stdcall SetLOD(unsigned long lod);
    virtual unsigned long __stdcall GetLOD();
    virtual unsigned long __stdcall GetLevelCount();
    virtual long __stdcall SetAutoGenFilterType(long filter);
    virtual long __stdcall GetAutoGenFilterType();
    virtual void __stdcall GenerateMipSubLevels();                           // slot 16
    virtual long __stdcall GetLevelDesc(unsigned long level, void* desc);    // slot 17 (+0x44)
    virtual long __stdcall GetSurfaceLevel(unsigned long level, IDirect3DSurface9** surface);   // slot 18 (+0x48)
};

class CSurface {                                   // vtable 0x0122F84C
public:
    IDirect3DSurface9* PD3DSurface;                // +0x04
    unsigned long      AllocationSource;           // +0x08
    void*              PAllocatedMemory;           // +0x0c

    CSurface() : PD3DSurface(0), AllocationSource(0) { }
    CSurface(const CSurface& other);                                   // 0x009F2D60
    virtual ~CSurface() { Uninitialise(); }
    void Uninitialise();                                               // 0x009F2E20
    void InitialiseFromTextureMipmap(IDirect3DSurface9* mipmapSurface);   // 0x009F2F10
};

class CTexture {
public:
    IDirect3DTexture9* PD3DTexture;                // +0x00
    unsigned long      ByteLength : 28;            // +0x04
    unsigned long      AllocationSource : 4;       // +0x04
    CSurface GetSurfaceLevel(unsigned long level) const;
};

CSurface CTexture::GetSurfaceLevel(unsigned long level) const
{
    IDirect3DSurface9* d3dSurface;
    PD3DTexture->GetSurfaceLevel(level, &d3dSurface);
    CSurface surface;
    surface.InitialiseFromTextureMipmap(d3dSurface);
    return surface;
}