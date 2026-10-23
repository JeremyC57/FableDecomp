// CVolume::ClearArea(const C3DBoxI& area, const CRGBColour& colour) -- locks the volume, clamps the box to the volume's
// width/height/depth and writes the colour, converted to the volume's pixel format, into every texel of the box
// Leaf: ClearArea
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A67A20, 599 bytes, `ret 8`. Catalog row "CSurface ClearArea" is CVolume's (the
//   IDirect3DVolume9 GetDesc slot 8 / UnlockBox slot 10, the CVolume::Lock 0x00A677D0 call).
// Callees: CVolume::Lock(unsigned long) 0x00A677D0 (same-TU body, as in the landed CVolume::Clear 0x00A67950),
//   CPixelFormat::Initialise(D3DFORMAT) 0x009E3830, ConvertRGBToPFDData 0x009E3F60, GetByteColourDepth 0x009E3800.
// Shape: the landed CVolume::Clear 0x00A67950 and CSurface::ClearArea 0x009F4190 (member-by-member box copy -- a struct
//   copy loads the fields in another order; the format in its own block over the description; inline GetWidth/GetHeight/GetDepth
//   each re-read the description; unsigned clamp compares).
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
struct CRGBColour { unsigned char B, G, R, A; };
struct CPixelFormat {
    long FormatIndex;                              // +0x00
    void Initialise(unsigned long d3dFormat);      // 0x009E3830
    unsigned long GetByteColourDepth() const;      // 0x009E3800 (bytes per pixel)
    void ConvertRGBToPFDData(const CRGBColour& colour, unsigned char* data) const;   // 0x009E3F60
};
struct C3DExtentsI { long X, Y, Z; };
struct CVolumeLockedBox {                          // 24 bytes (Ego_r.pdb)
    C3DExtentsI    Dimensions;                     // +0x00
    long           RowPitch;                       // +0x0c
    long           SlicePitch;                     // +0x10
    unsigned char* PMem;                           // +0x14
    CVolumeLockedBox() { }
};
struct CBase {
    virtual ~CBase();
};
struct CVolume : CBase {                           // vtable 0x0129C840
    IDirect3DVolume9* PD3DVolume;                  // +0x04
    CVolumeLockedBox Lock(unsigned long flags);    // 0x00A677D0
    void Unlock() const { PD3DVolume->UnlockBox(); }
    unsigned long GetWidth() const { D3DVOLUME_DESC desc; PD3DVolume->GetDesc(&desc); return desc.Width; }
    unsigned long GetHeight() const { D3DVOLUME_DESC desc; PD3DVolume->GetDesc(&desc); return desc.Height; }
    unsigned long GetDepth() const { D3DVOLUME_DESC desc; PD3DVolume->GetDesc(&desc); return desc.Depth; }
    void ClearArea(const struct C3DBoxI& area, const CRGBColour& colour);
};
struct C3DBoxI { long Left, Top, Right, Bottom, Front, Back; };
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

void CVolume::ClearArea(const C3DBoxI& area, const CRGBColour& colour)
{
    CVolumeLockedBox locked = Lock(0);
    CPixelFormat format;
    {
        D3DVOLUME_DESC desc;
        PD3DVolume->GetDesc(&desc);
        format.Initialise(desc.Format);
    }
    C3DBoxI box;
    box.Left = area.Left;
    box.Top = area.Top;
    box.Right = area.Right;
    box.Bottom = area.Bottom;
    box.Front = area.Front;
    box.Back = area.Back;
    if (box.Left < 0)
        box.Left = 0;
    else if ((unsigned long)box.Left > GetWidth())
        box.Left = GetWidth();
    if (box.Top < 0)
        box.Top = 0;
    else if ((unsigned long)box.Top > GetHeight())
        box.Top = GetHeight();
    if (box.Front < 0)
        box.Front = 0;
    else if ((unsigned long)box.Front > GetDepth())
        box.Front = GetDepth();
    if (box.Right < 0)
        box.Right = 0;
    else if ((unsigned long)box.Right > GetWidth())
        box.Right = GetWidth();
    if (box.Bottom < 0)
        box.Bottom = 0;
    else if ((unsigned long)box.Bottom > GetHeight())
        box.Bottom = GetHeight();
    if (box.Back < 0)
        box.Back = 0;
    else if ((unsigned long)box.Back > GetDepth())
        box.Back = GetDepth();
    unsigned char* dst = locked.PMem + format.GetByteColourDepth() * box.Left + locked.SlicePitch * box.Front + locked.RowPitch * box.Top;
    for (long z = box.Front; z < box.Back; ++z) {
        for (long y = box.Top; y < box.Bottom; ++y) {
            unsigned char* texel = dst;
            for (long x = box.Left; x < box.Right; ++x) {
                format.ConvertRGBToPFDData(colour, texel);
                texel += format.GetByteColourDepth();
            }
            dst += locked.RowPitch;
        }
        dst += locked.SlicePitch;
    }
    Unlock();
}