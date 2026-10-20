// CVolume::CopyFromMemoryAutoSwizzle(void* data, long rowPitch, long slicePitch) -- locks the volume and copies a linear source image into it row by row (width * bytes-per-pixel per row, the source and destination pitches stepped separately); no swizzle on PC
// Leaf: CopyFromMemoryAutoSwizzle
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A67860, 237 bytes (padded to 240), `ret 0Ch`.
//   PDB name ?CopyFromMemoryAutoSwizzle@CVolume@@QAEXPAXJJ@Z.
// Callees: CVolume::Lock(unsigned long) 0x00A677D0 (CVolumeLockedBox by value), IDirect3DVolume9 GetDesc (slot 8) and
//   UnlockBox (slot 10, the inline CVolume::Unlock), CPixelFormat::Initialise(D3DFORMAT) 0x009E3830,
//   CPixelFormat::GetByteColourDepth 0x009E3800; the row copy is the memcpy intrinsic (`rep movsd ; rep movsb`).
// Shape: two slice pointers stepped by the two slice pitches, two row pointers by the row pitches; the loop counts
//   are strength-reduced into count-down frame words (the row count homed in the dead `data` argument slot).
// Layout: CVolume (Ego_r.pdb, 8 bytes) vptr +0, PD3DVolume +4 (landed CVolume siblings 0x00A674E0 / 0x00A67480).
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int size);
#pragma intrinsic(memcpy)
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
    void CopyFromMemoryAutoSwizzle(void* data, long rowPitch, long slicePitch);
    void Clear(const CRGBColour& colour);
};
struct D3DLOCKED_BOX_Local { long RowPitch; long SlicePitch; void* pBits; };
struct D3DBOX_Local { unsigned long Left, Top, Right, Bottom, Front, Back; };
// Same-TU stand-in for CVolume::Lock 0x00A677D0 (GetDesc, LockBox over the whole volume): with its body in view VC7.1 knows the
// returned box's address goes no further.
#pragma auto_inline(off)
__declspec(noinline) CVolumeLockedBox CVolume::Lock(unsigned long flags)
{
    D3DVOLUME_DESC desc;
    PD3DVolume->GetDesc(&desc);
    D3DBOX_Local region;
    region.Left = 0; region.Top = 0; region.Front = 0;
    region.Right = desc.Width; region.Bottom = desc.Height; region.Back = desc.Depth;
    D3DLOCKED_BOX_Local locked;
    CVolumeLockedBox result;
    if (PD3DVolume->LockBox(&locked, &region, flags) >= 0) {
        result.Dimensions.X = (long)desc.Width; result.Dimensions.Y = (long)desc.Height; result.Dimensions.Z = (long)desc.Depth;
        result.RowPitch = locked.RowPitch; result.SlicePitch = locked.SlicePitch; result.PMem = (unsigned char*)locked.pBits;
    }
    return result;
}
#pragma auto_inline(on)
void CVolume::CopyFromMemoryAutoSwizzle(void* data, long rowPitch, long slicePitch)
{
    CVolumeLockedBox box = Lock(0);
    D3DVOLUME_DESC desc;
    PD3DVolume->GetDesc(&desc);
    unsigned long bytesPerPixel;
    {
        CPixelFormat format;
        format.Initialise(desc.Format);
        bytesPerPixel = format.GetByteColourDepth();
    }
    for (long z = 0; z < box.Dimensions.Z; ++z)
        for (long y = 0; y < box.Dimensions.Y; ++y)
            memcpy(box.PMem + z * box.SlicePitch + y * box.RowPitch, (unsigned char*)data + z * slicePitch + y * rowPitch,
                   box.Dimensions.X * bytesPerPixel);
    Unlock();
}