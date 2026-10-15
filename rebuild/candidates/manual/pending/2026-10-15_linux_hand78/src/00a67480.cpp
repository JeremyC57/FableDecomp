// CVolume::GetPixelFormat() -- ask Direct3D for the volume description (IDirect3DVolume9::GetDesc, COM slot 8) and convert its D3DFORMAT into a CPixelFormat built in the caller's hidden result slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A67480, length 43 bytes.
// Callees: IDirect3DVolume9::GetDesc (`call [ecx+20h]`, `this` pushed), CPixelFormat::CPixelFormat(D3DFORMAT) @ 0x009E3830.
// Shape: identical to CSurface::GetPixelFormat (0x009F2F60) with the 0x1c-byte D3DVOLUME_DESC frame and
// vtable slot 8. The manifest's "CSurface::GetPixelFormat" label is propagated: the neighbours 0x00A67420
// (ReleaseVolume), 0x00A674B0 (GetDimensions) and 0x00A674E0 (GetByteLength) are CVolume's.
struct D3DVOLUME_DESC_Local {                      // D3DVOLUME_DESC, 0x1c bytes
    unsigned long Format;                          // +0x00 D3DFORMAT
    unsigned long Type;                            // +0x04
    unsigned long Usage;                           // +0x08
    unsigned long Pool;                            // +0x0c
    unsigned long Width;                           // +0x10
    unsigned long Height;                          // +0x14
    unsigned long Depth;                           // +0x18
};

struct IDirect3DVolume9 {                          // IUnknown + IDirect3DVolume9 slots
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1 (+0x04)
    virtual unsigned long __stdcall Release();                               // slot 2 (+0x08)
    virtual long __stdcall GetDevice(void** device);                         // slot 3
    virtual long __stdcall SetPrivateData(const void* guid, const void* data, unsigned long size, unsigned long flags);
    virtual long __stdcall GetPrivateData(const void* guid, void* data, unsigned long* size);
    virtual long __stdcall FreePrivateData(const void* guid);
    virtual long __stdcall GetContainer(const void* iid, void** container);  // slot 7
    virtual long __stdcall GetDesc(D3DVOLUME_DESC_Local* desc);              // slot 8 (+0x20)
};

// CPixelFormat with its D3DFORMAT constructor (engine/CPixelFormat.h holds only the layout).
struct CPixelFormatFromD3D {
    long FormatIndex;                              // +0x00 (PDB CPixelFormat::FormatIndex)
    CPixelFormatFromD3D(unsigned long d3dFormat);  // 0x009E3830
    unsigned long GetByteColourDepth() const;      // 0x009E3800 (bytes per pixel)
};

// CVolume (PDB, 8 bytes): +0 the vptr, +4 the Direct3D volume. No engine header exists for it; the bodies
// below never touch the vptr, so it is kept as an opaque word.
struct CVolume {
    void*             __vftable;                   // +0x00
    IDirect3DVolume9* PD3DVolume;                  // +0x04
    CPixelFormatFromD3D GetPixelFormat() const;
};

CPixelFormatFromD3D CVolume::GetPixelFormat() const
{
    D3DVOLUME_DESC_Local desc;
    PD3DVolume->GetDesc(&desc);
    return CPixelFormatFromD3D(desc.Format);
}
