// Behaviour test for 0x00A67A20 CVolume::DrawBox (links the REAL row object; the declarations below are the row's own prologue).
// CVolume::DrawBox(const C3DBoxI& area, const CRGBColour& colour) -- locks the volume, clamps the box to the volume's
// width/height/depth and writes the colour, converted to the volume's pixel format, into every texel of the box
// Leaf: ?DrawBox@CVolume@@QAEXABVC3DBoxI@@ABVCRGBColour@@@Z
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A67A20, 599 bytes, `ret 8`. Catalog row "CSurface ClearArea" is CVolume's (the
//   IDirect3DVolume9 GetDesc slot 8 / UnlockBox slot 10, the CVolume::Lock 0x00A677D0 call).
// Callees: CVolume::Lock(unsigned long) 0x00A677D0 (same-TU copy, below),
//   CPixelFormat::Initialise(D3DFORMAT) 0x009E3830, ConvertRGBToPFDData 0x009E3F60, GetByteColourDepth 0x009E3800.
// Shape: the landed CVolume::Clear 0x00A67950 and CSurface::ClearArea 0x009F4190 (member-by-member box copy -- a struct
//   copy loads the fields in another order; the format through the inline CVolume::GetPixelFormat (Ego_r.pdb / FableWin.pdb
//   ?GetPixelFormat@CVolume@@QBE?AVCPixelFormat@@XZ: GetDesc, then CPixelFormat(desc.Format)); inline GetWidth/GetHeight/GetDepth
//   each re-read the description; unsigned clamp compares).
// Re-port (2026-11-19): the row's local CRGBColour / CPixelFormat / C3DExtentsI models (and its local _D3DFORMAT enum) are replaced by the
//   shared engine/CRGBColour.h, engine/CPixelFormat.h and engine/C3DExtentsI.h (Ego_d.pdb: a C2DExtentsI base plus long Z). CPixelFormat
//   opt-ins: FABLE_RETAIL_CPIXELFORMAT, _BYTECOLOURDEPTH_OUTOFLINE, _CONVERTRGBTOPFDDATA_OUTOFLINE: both callees out of line, as retail calls them here.
//   The format is built by the header's inline CPixelFormat(_D3DFORMAT) constructor (FableWin.pdb ??0CPixelFormat@@QAE@W4_D3DFORMAT@@@Z: Initialise only, no -1 store).
// Same-TU copy: CVolume::Lock is the retail TU-mate 0x00A677D0, written verbatim as the landed owner row over the owner's own D3D /
//   CVolume declarations (the earlier noinline stand-in body is gone); it stays a call here, as in retail (`call 0A677D0h`), with no
//   inline control.
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_BYTECOLOURDEPTH_OUTOFLINE
#define FABLE_RETAIL_CPIXELFORMAT_CONVERTRGBTOPFDDATA_OUTOFLINE
#include "engine/Direct3D9Types.h"
#include "engine/CRGBColour.h"
#include "engine/CPixelFormat.h"
#include <string.h>
#include "rebuild_abi.h"
struct _GUID;struct IDirect3DDevice9;
enum _D3DRESOURCETYPE{D3DRTYPE_VOLUME=2};enum _D3DPOOL{D3DPOOL_DEFAULT=0};
struct _D3DVOLUME_DESC{_D3DFORMAT Format;_D3DRESOURCETYPE Type;unsigned long Usage;_D3DPOOL Pool;unsigned int Width,Height,Depth;};
struct _D3DBOX{unsigned int Left,Top,Right,Bottom,Front,Back;};struct _D3DLOCKED_BOX{int RowPitch,SlicePitch;void*pBits;};
struct IUnknown{virtual long __stdcall QueryInterface(const _GUID&,void**)=0;virtual unsigned long __stdcall AddRef()=0;virtual unsigned long __stdcall Release()=0;};
struct IDirect3DVolume9:public IUnknown{virtual long __stdcall GetDevice(IDirect3DDevice9**)=0;virtual long __stdcall SetPrivateData(const _GUID&,const void*,unsigned long,unsigned long)=0;virtual long __stdcall GetPrivateData(const _GUID&,void*,unsigned long*)=0;virtual long __stdcall FreePrivateData(const _GUID&)=0;virtual long __stdcall GetContainer(const _GUID&,void**)=0;virtual long __stdcall GetDesc(_D3DVOLUME_DESC*)=0;virtual long __stdcall LockBox(_D3DLOCKED_BOX*,const _D3DBOX*,unsigned long)=0;virtual long __stdcall UnlockBox()=0;};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#include "engine/C3DExtentsI.h"
class CVolumeLockedBox{public:C3DExtentsI Dimensions;long RowPitch,SlicePitch;unsigned char*PMem;CVolumeLockedBox(){}CVolumeLockedBox(const CVolumeLockedBox&o):Dimensions(o.Dimensions),RowPitch(o.RowPitch),SlicePitch(o.SlicePitch),PMem(o.PMem){}};
class CVolume:public CBaseClass{public:IDirect3DVolume9*PD3DVolume;virtual~CVolume();C3DExtentsI GetDimensions()const{_D3DVOLUME_DESC desc;PD3DVolume->GetDesc(&desc);return C3DExtentsI(desc.Width,desc.Height,desc.Depth);}CVolumeLockedBox Lock(unsigned long);CVolumeLockedBox ReadOnlyLock(unsigned long)const;void Unlock()const{PD3DVolume->UnlockBox();}CPixelFormat GetPixelFormat()const{_D3DVOLUME_DESC desc;PD3DVolume->GetDesc(&desc);return CPixelFormat(desc.Format);}unsigned long GetWidth()const{_D3DVOLUME_DESC desc;PD3DVolume->GetDesc(&desc);return desc.Width;}unsigned long GetHeight()const{_D3DVOLUME_DESC desc;PD3DVolume->GetDesc(&desc);return desc.Height;}unsigned long GetDepth()const{_D3DVOLUME_DESC desc;PD3DVolume->GetDesc(&desc);return desc.Depth;}void DrawBox(const class C3DBoxI& area,const CRGBColour& colour);};
class C3DBoxI { public: long Left, Top, Right, Bottom, Front, Back; };
FABLE_STATIC_ASSERT(sizeof(C3DExtentsI)==12);FABLE_STATIC_ASSERT(sizeof(CVolumeLockedBox)==24);FABLE_STATIC_ASSERT(sizeof(CVolume)==8);

// ---- test harness: links the REAL row object (the row function and its same-TU CVolume::Lock copy come from it) ----
#include <stdio.h>
static int g_fail;
#define CHECK(c) do { if (!(c)) { ++g_fail; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static _D3DVOLUME_DESC g_desc; static long g_lockStatus, g_rowPitch, g_slicePitch; static unsigned char* g_bits;
static int g_descCalls, g_locks, g_unlocks; static unsigned long g_lockFlags; static _D3DBOX g_box;
struct TestVolume : IDirect3DVolume9 {
    virtual long __stdcall QueryInterface(const _GUID&, void**) { ++g_fail; return -1; }
    virtual unsigned long __stdcall AddRef() { ++g_fail; return 0; }
    virtual unsigned long __stdcall Release() { ++g_fail; return 0; }
    virtual long __stdcall GetDevice(IDirect3DDevice9**) { ++g_fail; return -1; }
    virtual long __stdcall SetPrivateData(const _GUID&, const void*, unsigned long, unsigned long) { ++g_fail; return -1; }
    virtual long __stdcall GetPrivateData(const _GUID&, void*, unsigned long*) { ++g_fail; return -1; }
    virtual long __stdcall FreePrivateData(const _GUID&) { ++g_fail; return -1; }
    virtual long __stdcall GetContainer(const _GUID&, void**) { ++g_fail; return -1; }
    virtual long __stdcall GetDesc(_D3DVOLUME_DESC* desc) { *desc = g_desc; ++g_descCalls; return 0; }
    virtual long __stdcall LockBox(_D3DLOCKED_BOX* locked, const _D3DBOX* box, unsigned long flags)
    {
        g_box = *box; g_lockFlags = flags; ++g_locks;
        locked->RowPitch = g_rowPitch; locked->SlicePitch = g_slicePitch; locked->pBits = g_bits;
        return g_lockStatus;
    }
    virtual long __stdcall UnlockBox() { if (g_locks != 1) ++g_fail; ++g_unlocks; return 0; }
};
CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { }
CVolume::~CVolume() { }
static _D3DFORMAT g_format; static int g_inits, g_converts, g_depthCalls; static unsigned long g_bpp;
void CPixelFormat::Initialise(_D3DFORMAT format) { g_format = format; ++g_inits; FormatIndex = 7; }
unsigned long CPixelFormat::GetByteColourDepth() const { ++g_depthCalls; if (FormatIndex != 7) ++g_fail; return g_bpp; }
static void Reset(unsigned long w, unsigned long h, unsigned long d, _D3DFORMAT format, unsigned long bpp, long rowPitch, long slicePitch, unsigned char* bits)
{
    memset(&g_desc, 0x71, sizeof g_desc); g_desc.Format = format; g_desc.Width = w; g_desc.Height = h; g_desc.Depth = d;
    g_bpp = bpp; g_rowPitch = rowPitch; g_slicePitch = slicePitch; g_bits = bits; g_lockStatus = 0;
    g_descCalls = g_locks = g_unlocks = g_inits = g_converts = g_depthCalls = 0; g_lockFlags = 99; g_format = D3DFMT_UNKNOWN;
}
static void CheckWholeVolumeLock()
{
    CHECK(g_locks == 1); CHECK(g_unlocks == 1); CHECK(g_lockFlags == 0);
    CHECK(g_box.Left == 0 && g_box.Top == 0 && g_box.Front == 0);
    CHECK(g_box.Right == g_desc.Width && g_box.Bottom == g_desc.Height && g_box.Back == g_desc.Depth);
}

void CPixelFormat::ConvertRGBToPFDData(const CRGBColour& colour, unsigned char* data) const
{
    ++g_converts; if (FormatIndex != 7) ++g_fail;
    for (unsigned long k = 0; k < g_bpp; ++k) data[k] = (unsigned char)(colour.B + k);
}
static unsigned char g_mem[4096];
static long Clamp(long v, unsigned long extent) { if (v < 0) return 0; if ((unsigned long)v > extent) return (long)extent; return v; }
static void Case(long l, long t, long r, long b, long f, long k, unsigned long w, unsigned long h, unsigned long d, unsigned long bpp, long rowPitch, long slicePitch)
{
    Reset(w, h, d, D3DFMT_A8R8G8B8, bpp, rowPitch, slicePitch, g_mem + 16);
    memset(g_mem, 0xEE, sizeof g_mem);
    static unsigned char expect[4096]; memset(expect, 0xEE, sizeof expect);
    long L = Clamp(l, w), T = Clamp(t, h), F = Clamp(f, d), R = Clamp(r, w), B = Clamp(b, h), K = Clamp(k, d);
    int texels = 0;
    unsigned char* dst = expect + 16 + bpp * L + slicePitch * F + rowPitch * T;
    for (long z = F; z < K; ++z) {
        for (long y = T; y < B; ++y) {
            unsigned char* p = dst;
            for (long x = L; x < R; ++x) { for (unsigned long c = 0; c < bpp; ++c) p[c] = (unsigned char)(0x50 + c); p += bpp; ++texels; }
            dst += rowPitch;
        }
        dst += slicePitch;
    }
    TestVolume device; CVolume volume; volume.PD3DVolume = &device;
    C3DBoxI area; area.Left = l; area.Top = t; area.Right = r; area.Bottom = b; area.Front = f; area.Back = k;
    CRGBColour colour(0x52, 0x51, 0x50, 0x53);            // B = 0x50
    volume.DrawBox(area, colour);
    CheckWholeVolumeLock();
    CHECK(g_inits == 1); CHECK(g_format == D3DFMT_A8R8G8B8); CHECK(g_converts == texels);
    CHECK(memcmp(g_mem, expect, sizeof g_mem) == 0);
    CHECK(area.Left == l && area.Top == t && area.Right == r && area.Bottom == b && area.Front == f && area.Back == k);   // the caller's box is not clamped
}
int main()
{
    // width 4 < ... the three extents differ (height 3 < width 4 < depth 5), so a clamp against the wrong extent is visible
    Case(1, 1, 3, 3, 1, 3, 4, 3, 5, 2, 10, 40);           // inside, front above 0
    Case(-3, 1, 9, 3, 0, 1, 4, 3, 5, 2, 10, 40);          // left below 0, right above the width
    Case(0, -2, 2, 7, 0, 1, 4, 3, 5, 4, 20, 64);          // top below 0, bottom above the height
    Case(0, 0, 1, 1, -1, 9, 4, 3, 5, 1, 8, 40);           // front below 0, back above the depth
    Case(6, 0, 9, 2, 0, 1, 4, 3, 5, 2, 10, 40);           // left above the width: empty after the clamp
    Case(0, 5, 2, 9, 0, 1, 4, 3, 5, 2, 10, 40);           // top above the height: empty
    Case(0, 0, 2, 2, 7, 9, 4, 3, 5, 2, 10, 40);           // front above the depth: empty
    Case(2, 2, 1, 3, 0, 2, 4, 3, 5, 2, 10, 40);           // right < left: no texels
    Case(0, 0, -1, 2, 0, 1, 4, 3, 5, 2, 10, 40);          // right below 0
    Case(0, 0, 2, -1, 0, 1, 4, 3, 5, 2, 10, 40);          // bottom below 0
    Case(0, 0, 2, 2, 0, -1, 4, 3, 5, 2, 10, 40);          // back below 0
    Case(4, 3, 4, 3, 5, 5, 4, 3, 5, 2, 10, 40);           // exactly the extents (not clamped, empty)
    Case(0, 0, 4, 3, 0, 5, 4, 3, 5, 3, 12, 5);            // the whole volume
    Case(1, 2, 4, 3, 4, 5, 4, 3, 5, 2, 10, 40);           // the last row of the last slice
    Case(0, 3, 2, 5, 0, 1, 2, 5, 4, 2, 10, 60);           // audit: width 2 < height 5, top 3 above the width (kills a top clamp against the width)
    Case(3, 0, 5, 2, 0, 1, 5, 2, 4, 2, 12, 60);           // audit: height 2 < width 5, left 3 above the height
    if (g_fail) { printf("00a67a20_TEST FAIL (%d)\n", g_fail); return 1; }
    puts("00a67a20_TEST PASS"); return 0;
}