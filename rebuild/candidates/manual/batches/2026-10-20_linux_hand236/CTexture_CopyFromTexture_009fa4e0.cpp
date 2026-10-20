// CTexture::CopyFromTexture(const CTexture& source, unsigned long filter) -- copy every mip level both textures share: wrap each level's surface in a CSurface and CopyFromSurface the whole source level into the whole destination level with the filter
// Leaf: ?CopyFromTexture@CTexture
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FA4E0, 289 bytes, `ret 8`. (The previously landed copy of this row was a
//   inline-assembly oracle; this is the compiled reconstruction.)
// Callees: IDirect3DTexture9::GetLevelCount (COM slot 13, +0x34) and GetSurfaceLevel (slot 18, +0x48), CSurface::
//   InitialiseFromTextureMipmap @ 0x009F2F10, CSurface::CSurface(const CSurface&) @ 0x009F2D60, CSurface::Uninitialise
//   @ 0x009F2E20, CSurface::CopyFromSurface(const C2DBoxI*, const CSurface&, const C2DBoxI*, unsigned long) @ 0x009F37E0;
//   CSurface vtable 0x0122F84C.
// Layout: the landed CTexture::GetSurfaceLevel 0x009F9E00 model (PDB CTexture / CSurface; local because the engine
//   headers cannot carry the vptr, bitfields and constructors).
// Shape: the level count is a min() by reference over the two counts (`lea eax,[src] ; jl ; lea eax,[dst] ; mov eax,[eax]`);
//   GetSurfaceLevel is inlined from its class definition (local CSurface, InitialiseFromTextureMipmap, copy into the named
//   per-level surface, local destroyed), source first; the two named surfaces die at the end of each iteration.
struct C2DBoxI { long left, top, right, bottom; };
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
    void CopyFromSurface(const C2DBoxI* destinationBox, const CSurface& source, const C2DBoxI* sourceBox,
                         unsigned long filter);                        // 0x009F37E0
};

template <class T> inline const T& Min(const T& a, const T& b) { return a < b ? a : b; }

class CTexture {
public:
    IDirect3DTexture9* PD3DTexture;                // +0x00
    unsigned long      ByteLength : 28;            // +0x04
    unsigned long      AllocationSource : 4;       // +0x04
    CSurface GetSurfaceLevel(unsigned long level) const     // 0x009F9E00 (inlined here)
    {
        IDirect3DSurface9* d3dSurface;
        PD3DTexture->GetSurfaceLevel(level, &d3dSurface);
        CSurface surface;
        surface.InitialiseFromTextureMipmap(d3dSurface);
        return surface;
    }
    void CopyFromTexture(const CTexture& source, unsigned long filter);
};

void CTexture::CopyFromTexture(const CTexture& source, unsigned long filter)
{
    long sourceLevels = (long)source.PD3DTexture->GetLevelCount();
    long destinationLevels = (long)PD3DTexture->GetLevelCount();
    long levels = Min(sourceLevels, destinationLevels);
    for (long level = 0; level < levels; ++level) {
        CSurface sourceSurface = source.GetSurfaceLevel(level);
        CSurface destinationSurface = GetSurfaceLevel(level);
        destinationSurface.CopyFromSurface(0, sourceSurface, 0, filter);
    }
}
