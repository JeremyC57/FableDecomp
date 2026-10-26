// CTexture::GetLevelByteLength(unsigned long level) -- size in bytes of one mip level: the level's surface description (IDirect3DTexture9::GetLevelDesc, COM slot 17) for Width/Height, the texture's pixel format (an inline GetPixelFormat on level 0) for the bits per pixel, then bits * Width * Height / 8
// Retail: Fable.exe @ 0x009F9E80, 82 bytes (+ int3 padding).
// Callees: IDirect3DTexture9::GetLevelDesc (`call [ecx+44h]`, `this` pushed) twice, CPixelFormat::CPixelFormat(D3DFORMAT)
// @ 0x009E3830 (labelled CPixelFormat::Initialise), CPixelFormat::GetColourDepth @ 0x009E3820 (bits per pixel).
// Shape: the sibling of the landed CVolumeTexture::GetByteLength (0x00A681F0): a 0x40-byte frame holding the two
// D3DSURFACE_DESCs (the level's at +0, level 0's at +0x20), the CPixelFormat temporary built in the DEAD slot of the
// `level` argument (`lea ecx,[esp+4Ch]`), `imul` by Height then Width (the source multiplies Width * Height), `shr 3`.
// Layout: PDB CTexture (PD3DTexture +0, the ByteLength/AllocationSource bitfield word +4); engine/CTexture.h flattens
// the bitfields into byte arrays, so the class is modelled locally with the PDB names.
struct D3DSURFACE_DESC_Local {                     // D3DSURFACE_DESC, 0x20 bytes
    unsigned long Format;                          // +0x00 D3DFORMAT
    unsigned long Type;                            // +0x04
    unsigned long Usage;                           // +0x08
    unsigned long Pool;                            // +0x0c
    unsigned long MultiSampleType;                 // +0x10
    unsigned long MultiSampleQuality;              // +0x14
    unsigned long Width;                           // +0x18
    unsigned long Height;                          // +0x1c
};

struct IDirect3DTexture9 {                         // IUnknown + IDirect3DResource9 + IDirect3DBaseTexture9 slots
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
    virtual long __stdcall GetLevelDesc(unsigned long level, D3DSURFACE_DESC_Local* desc);   // slot 17 (+0x44)
};

// CPixelFormat with its D3DFORMAT constructor (engine/CPixelFormat.h holds only the layout).
class CPixelFormat { public:
    long FormatIndex;                              // +0x00 (PDB CPixelFormat::FormatIndex)
    CPixelFormat(unsigned long d3dFormat);  // 0x009E3830
public:
    unsigned long GetColourDepth() const;   // 0x009E3820
};

class CTexture {
public:
    IDirect3DTexture9* PD3DTexture;                // +0x00
    unsigned long      ByteLength : 28;            // +0x04
    unsigned long      AllocationSource : 4;       // +0x04

    CPixelFormat GetPixelFormat() const
    {
        D3DSURFACE_DESC_Local desc;
        PD3DTexture->GetLevelDesc(0, &desc);
        return CPixelFormat(desc.Format);
    }
    unsigned long GetLevelByteLength(unsigned long level) const;
};

unsigned long CTexture::GetLevelByteLength(unsigned long level) const
{
    D3DSURFACE_DESC_Local desc;
    PD3DTexture->GetLevelDesc(level, &desc);
    return GetPixelFormat().GetColourDepth() * desc.Width * desc.Height / 8;
}