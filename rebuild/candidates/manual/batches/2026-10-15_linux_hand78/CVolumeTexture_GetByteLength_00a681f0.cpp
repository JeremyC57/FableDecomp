// CVolumeTexture::GetByteLength(unsigned long level) -- size in bytes of one mip level of the volume texture: the level's description (IDirect3DVolumeTexture9::GetLevelDesc, COM slot 17) for the dimensions, the texture's pixel format (an inline GetPixelFormat on level 0) for the bytes per voxel, then bytes * Width * Height * Depth
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A681F0, length 86 bytes.
// Callees: IDirect3DVolumeTexture9::GetLevelDesc (`call [ecx+44h]`, `this` pushed) twice,
// CPixelFormat::CPixelFormat(D3DFORMAT) @ 0x009E3830, CPixelFormat::GetByteColourDepth @ 0x009E3800.
// Shape: a 0x38-byte frame holding the two D3DVOLUME_DESCs (the level's at +0, level 0's at +0x1c); the
// CPixelFormat temporary is built in the DEAD slot of the `level` argument (`lea ecx,[esp+44h]` / `[esp+40h]`),
// then `imul` from the frame by Depth, Height, Width; `ret 4`. The manifest's "CSurface::GetByteLength" label
// is propagated; the neighbours 0x00A68060 (Uninitialise) .. 0x00A682E0 are CVolumeTexture's.
struct D3DVOLUME_DESC_Local {                      // D3DVOLUME_DESC, 0x1c bytes
    unsigned long Format;                          // +0x00 D3DFORMAT
    unsigned long Type;                            // +0x04
    unsigned long Usage;                           // +0x08
    unsigned long Pool;                            // +0x0c
    unsigned long Width;                           // +0x10
    unsigned long Height;                          // +0x14
    unsigned long Depth;                           // +0x18
};

struct IDirect3DVolumeTexture9 {                   // IUnknown + IDirect3DResource9 + IDirect3DBaseTexture9 slots
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
    virtual long __stdcall GetLevelDesc(unsigned long level, D3DVOLUME_DESC_Local* desc);   // slot 17 (+0x44)
};

// CPixelFormat with its D3DFORMAT constructor (engine/CPixelFormat.h holds only the layout).
struct CPixelFormatFromD3D {
    long FormatIndex;                              // +0x00 (PDB CPixelFormat::FormatIndex)
    CPixelFormatFromD3D(unsigned long d3dFormat);  // 0x009E3830
    unsigned long GetByteColourDepth() const;      // 0x009E3800 (bytes per pixel)
};

// CVolumeTexture (PDB, 16 bytes). No engine header exists; the vptr is never touched here.
struct CVolumeTexture {
    void*                    __vftable;            // +0x00
    IDirect3DVolumeTexture9* PD3DVolumeTexture;    // +0x04
    long                     ByteLength;           // +0x08
    void*                    PData;                // +0x0c

    CPixelFormatFromD3D GetPixelFormat() const
    {
        D3DVOLUME_DESC_Local desc;
        PD3DVolumeTexture->GetLevelDesc(0, &desc);
        return CPixelFormatFromD3D(desc.Format);
    }
    unsigned long GetByteLength(unsigned long level) const;
};

unsigned long CVolumeTexture::GetByteLength(unsigned long level) const
{
    D3DVOLUME_DESC_Local desc;
    PD3DVolumeTexture->GetLevelDesc(level, &desc);
    return GetPixelFormat().GetByteColourDepth() * desc.Width * desc.Height * desc.Depth;
}
