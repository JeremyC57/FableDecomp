// CVolume::Clear(const CRGBColour& colour) -- locks the volume and writes the colour, converted to the volume's pixel format, into every texel (pixel by pixel, rows by the locked row pitch, slices by the slice pitch on top)
// Leaf: Clear
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A67950, 194 bytes (padded to 208), `ret 4`.
//   PDB name ?Clear@CVolume@@QAEXABVCRGBColour@@@Z.
// Callees: CVolume::Lock(unsigned long) 0x00A677D0, IDirect3DVolume9 GetDesc (slot 8) / UnlockBox (slot 10, inline
//   CVolume::Unlock), CPixelFormat::Initialise(D3DFORMAT) 0x009E3830, CPixelFormat::ConvertRGBToPFDData(const
//   CRGBColour&, unsigned char*) 0x009E3F60 (catalog "Graphics_CopyPixelsToBuffer"), GetByteColourDepth 0x009E3800
//   (called again for every texel). CVolume::Lock has its same-TU body below (as retail compiled it): with only a
//   declaration VC7.1 hoisted the box's dimension loads out of the row/slice loops.
// Shape: one running destination pointer (read from the box before GetDesc) advanced by the row pitch after each row
//   and by the slice pitch after each slice; the three loop counts are strength-reduced count-downs.
// Layout: CVolume (Ego_r.pdb, 8 bytes) vptr +0, PD3DVolume +4.
struct D3DVOLUME_DESC {                            // 0x1c bytes
    unsigned long Format;                          // +0x00 D3DFORMAT
    unsigned long Type;
    unsigned long Usage;
    unsigned long Pool;
    unsigned long Width;
    unsigned long Height;
    unsigned long Depth;
};
struct IDirect3DVolume9 {                          // IUnknown + IDirect3DVolume9 slots
    virtual long __stdcall QueryInterface(const void* iid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall GetDevice(void** device);
    virtual long __stdcall SetPrivateData(const void* guid, const void* data, unsigned long size, unsigned long flags);
    virtual long __stdcall GetPrivateData(const void* guid, void* data, unsigned long* size);
    virtual long __stdcall FreePrivateData(const void* guid);
    virtual long __stdcall GetContainer(const void* iid, void** container);
    virtual long __stdcall GetDesc(D3DVOLUME_DESC* desc);                    // slot 8 (+0x20)
    virtual long __stdcall LockBox(void* lockedVolume, const void* box, unsigned long flags);
    virtual long __stdcall UnlockBox();                                      // slot 10 (+0x28)
};
class CRGBColour { public: unsigned char B, G, R, A; };
class CPixelFormat { public:
    long FormatIndex;                              // +0x00
    void Initialise(unsigned long d3dFormat);      // 0x009E3830
    unsigned long GetByteColourDepth() const;      // 0x009E3800 (bytes per pixel)
    void ConvertRGBToPFDData(const CRGBColour& colour, unsigned char* data) const;   // 0x009E3F60
};
class C3DExtentsI { public: long X, Y, Z; };
class CVolumeLockedBox { public:                          // 24 bytes (Ego_r.pdb)
    C3DExtentsI    Dimensions;                     // +0x00
    long           RowPitch;                       // +0x0c
    long           SlicePitch;                     // +0x10
    unsigned char* PMem;                           // +0x14
    CVolumeLockedBox() { }
};
struct CBase {
    virtual ~CBase();
};
class CVolume : public CBase { public:                           // vtable 0x0129C840
    IDirect3DVolume9* PD3DVolume;                  // +0x04
    CVolumeLockedBox Lock(unsigned long flags);    // 0x00A677D0
    void Unlock() const { PD3DVolume->UnlockBox(); }
    void CopyFromMemoryAutoSwizzle(void* data, long rowPitch, long slicePitch);
    void Clear(const CRGBColour& colour);
};
struct D3DLOCKED_BOX { long RowPitch; long SlicePitch; void* pBits; };
struct D3DBOX { unsigned long Left, Top, Right, Bottom, Front, Back; };
#pragma auto_inline(off)
// Same-TU body of CVolume::Lock (0x00A677D0): lock the whole volume and describe it (dimensions from the description,
// pitches and memory from the locked box); the result is left untouched when the lock fails.
__declspec(noinline) CVolumeLockedBox CVolume::Lock(unsigned long flags)
{
    D3DVOLUME_DESC desc;
    PD3DVolume->GetDesc(&desc);
    D3DBOX region;
    region.Left = 0; region.Top = 0; region.Front = 0;
    region.Right = desc.Width; region.Bottom = desc.Height; region.Back = desc.Depth;
    D3DLOCKED_BOX locked;
    CVolumeLockedBox result;
    if (PD3DVolume->LockBox(&locked, &region, flags) >= 0) {
        result.Dimensions.X = desc.Width;
        result.Dimensions.Y = desc.Height;
        result.Dimensions.Z = desc.Depth;
        result.RowPitch = locked.RowPitch;
        result.SlicePitch = locked.SlicePitch;
        result.PMem = (unsigned char*)locked.pBits;
    }
    return result;
}
#pragma auto_inline(on)
void CVolume::Clear(const CRGBColour& colour)
{
    CVolumeLockedBox box = Lock(0);
    unsigned char* dst = box.PMem;
    D3DVOLUME_DESC desc;
    PD3DVolume->GetDesc(&desc);
    CPixelFormat format;
    format.Initialise(desc.Format);
    for (long z = box.Dimensions.Z; z > 0; --z) {
        for (long y = box.Dimensions.Y; y > 0; --y) {
            unsigned char* texel = dst;
            for (long x = box.Dimensions.X; x > 0; --x) {
                format.ConvertRGBToPFDData(colour, texel);
                texel += format.GetByteColourDepth();
            }
            dst += box.RowPitch;
        }
        dst += box.SlicePitch;
    }
    Unlock();
}