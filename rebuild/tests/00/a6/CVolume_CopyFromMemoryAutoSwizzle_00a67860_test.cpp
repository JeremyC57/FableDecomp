// Behaviour test for 00a67860: src/00a67860.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CVolume::CopyFromMemoryAutoSwizzle(void* data, long rowPitch, long slicePitch)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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

// ---- stubs and driver ----
long __stdcall IDirect3DVolume9::QueryInterface(const void*, void**) { return 0; }
unsigned long __stdcall IDirect3DVolume9::AddRef() { return 0; }
unsigned long __stdcall IDirect3DVolume9::Release() { return 0; }
long __stdcall IDirect3DVolume9::GetDevice(void**) { return 0; }
long __stdcall IDirect3DVolume9::SetPrivateData(const void*, const void*, unsigned long, unsigned long) { return 0; }
long __stdcall IDirect3DVolume9::GetPrivateData(const void*, void*, unsigned long*) { return 0; }
long __stdcall IDirect3DVolume9::FreePrivateData(const void*) { return 0; }
long __stdcall IDirect3DVolume9::GetContainer(const void*, void**) { return 0; }
long __stdcall IDirect3DVolume9::GetDesc(D3DVOLUME_DESC*) { return -1; }
long __stdcall IDirect3DVolume9::LockBox(void*, const void*, unsigned long) { return -1; }
long __stdcall IDirect3DVolume9::UnlockBox() { return -1; }
struct CFakeVolume : IDirect3DVolume9 {
    unsigned long w, h, d; long rowPitch, slicePitch; unsigned char* bits; int unlocked; unsigned long lastFlags;
    long __stdcall GetDesc(D3DVOLUME_DESC* desc) { memset(desc, 0, sizeof(*desc)); desc->Format = 21; desc->Width = w; desc->Height = h; desc->Depth = d; return 0; }
    long __stdcall LockBox(void* lb, const void* box, unsigned long flags)
    {
        const D3DBOX_Local* b = (const D3DBOX_Local*)box; lastFlags = flags;
        if (b->Left || b->Top || b->Front || b->Right != w || b->Bottom != h || b->Back != d) return -2;
        D3DLOCKED_BOX_Local* l = (D3DLOCKED_BOX_Local*)lb; l->RowPitch = rowPitch; l->SlicePitch = slicePitch; l->pBits = bits; return 0;
    }
    long __stdcall UnlockBox() { ++unlocked; return 0; }
};
static unsigned long g_format = 0;
void CPixelFormat::Initialise(unsigned long d3dFormat) { g_format = d3dFormat; FormatIndex = 3; }
unsigned long CPixelFormat::GetByteColourDepth() const { return FormatIndex == 3 ? 2 : 0; }
void CPixelFormat::ConvertRGBToPFDData(const CRGBColour&, unsigned char*) const {}
CBase::~CBase() {}
int main()
{
    // 3 x 2 x 2 texels of 2 bytes: rows of 6 bytes, volume row pitch 8, slice pitch 20; source row pitch 7, slice pitch 16
    unsigned char vol[40]; unsigned char src[32];
    for (int i = 0; i < 40; ++i) vol[i] = (unsigned char)(0xA0 + i);
    for (int i = 0; i < 32; ++i) src[i] = (unsigned char)i;
    CFakeVolume fv; fv.w = 3; fv.h = 2; fv.d = 2; fv.rowPitch = 8; fv.slicePitch = 20; fv.bits = vol; fv.unlocked = 0; fv.lastFlags = 9;
    CVolume v; v.PD3DVolume = &fv;
    v.CopyFromMemoryAutoSwizzle(src, 7, 16);
    CHECK(fv.unlocked == 1); CHECK(fv.lastFlags == 0); CHECK(g_format == 21);
    for (int z = 0; z < 2; ++z) for (int y = 0; y < 2; ++y) {
        for (int x = 0; x < 6; ++x) CHECK(vol[z * 20 + y * 8 + x] == src[z * 16 + y * 7 + x]);
        for (int x = 6; x < 8; ++x) CHECK(vol[z * 20 + y * 8 + x] == (unsigned char)(0xA0 + z * 20 + y * 8 + x));
    }
    CHECK(vol[16] == 0xA0 + 16); CHECK(vol[19] == 0xA0 + 19);
    if (fails == 0) printf("00a67860_TEST PASS\n"); else printf("00a67860_TEST FAIL\n");
    return fails;
}