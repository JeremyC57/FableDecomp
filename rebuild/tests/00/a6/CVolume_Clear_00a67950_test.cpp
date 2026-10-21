// Behaviour test for 00a67950: src/00a67950.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CVolume::Clear(const CRGBColour& colour)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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

// ---- stubs and driver ----

static unsigned char g_mem[2 * 3 * 64];
static int g_unlocks = 0;
long __stdcall IDirect3DVolume9::QueryInterface(const void*, void**) { return 0; }
unsigned long __stdcall IDirect3DVolume9::AddRef() { return 1; }
unsigned long __stdcall IDirect3DVolume9::Release() { return 1; }
long __stdcall IDirect3DVolume9::GetDevice(void**) { return 0; }
long __stdcall IDirect3DVolume9::SetPrivateData(const void*, const void*, unsigned long, unsigned long) { return 0; }
long __stdcall IDirect3DVolume9::GetPrivateData(const void*, void*, unsigned long*) { return 0; }
long __stdcall IDirect3DVolume9::FreePrivateData(const void*) { return 0; }
long __stdcall IDirect3DVolume9::GetContainer(const void*, void**) { return 0; }
long __stdcall IDirect3DVolume9::GetDesc(D3DVOLUME_DESC* desc) { memset(desc, 0, sizeof *desc); desc->Format = 21; desc->Width = 3; desc->Height = 2; desc->Depth = 2; return 0; }
long __stdcall IDirect3DVolume9::LockBox(void* lockedVolume, const void* box, unsigned long flags)
{
    const D3DBOX* b = (const D3DBOX*)box; if (b->Right != 3 || b->Bottom != 2 || b->Back != 2 || b->Left || b->Top || b->Front || flags) return -1;
    D3DLOCKED_BOX* l = (D3DLOCKED_BOX*)lockedVolume; l->RowPitch = 16; l->SlicePitch = 64; l->pBits = g_mem; return 0;
}
long __stdcall IDirect3DVolume9::UnlockBox() { ++g_unlocks; return 0; }
static unsigned long g_format = 0; static int g_converts = 0;
void CPixelFormat::Initialise(unsigned long d3dFormat) { g_format = d3dFormat; FormatIndex = 4; }
unsigned long CPixelFormat::GetByteColourDepth() const { return (unsigned long)FormatIndex; }
void CPixelFormat::ConvertRGBToPFDData(const CRGBColour& colour, unsigned char* data) const
{ ++g_converts; data[0] = colour.B; data[1] = colour.G; data[2] = colour.R; data[3] = colour.A; }
CBase::~CBase() {}
int main()
{
    IDirect3DVolume9 fake;
    CVolume volume; volume.PD3DVolume = &fake;
    CRGBColour c = { 1, 2, 3, 4 };
    volume.Clear(c);
    CHECK(g_format == 21); CHECK(g_converts == 12); CHECK(g_unlocks == 1);
    // texels: slice z at z*(16*2 + 64)? rows advance by RowPitch, then SlicePitch on top
    unsigned char* p = g_mem;
    for (int z = 0; z < 2; ++z) {
        for (int y = 0; y < 2; ++y) {
            for (int x = 0; x < 3; ++x) { CHECK(p[x * 4] == 1 && p[x * 4 + 3] == 4); }
            CHECK(p[12] == 0);                     // the rest of the row pitch untouched
            p += 16;
        }
        p += 64;
    }
    if (fails == 0) printf("00a67950_TEST PASS\n"); else printf("00a67950_TEST FAIL\n");
    return fails;
}