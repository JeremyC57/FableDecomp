// Behaviour test for 0x00A67950 CVolume::Clear (links the REAL row object; the declarations below are the row's own prologue).
// CVolume::Clear(const CRGBColour& colour) -- locks the volume and writes the colour, converted to the volume's pixel format, into every texel (pixel by pixel, rows by the locked row pitch, slices by the slice pitch on top)
// Leaf: ?Clear@CVolume@@QAEXABVCRGBColour@@@Z
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A67950, 194 bytes (padded to 208), `ret 4`.
//   PDB name ?Clear@CVolume@@QAEXABVCRGBColour@@@Z.
// Callees: CVolume::Lock(unsigned long) 0x00A677D0, IDirect3DVolume9 GetDesc (slot 8) / UnlockBox (slot 10, inline
//   CVolume::Unlock), CPixelFormat::Initialise(D3DFORMAT) 0x009E3830, CPixelFormat::ConvertRGBToPFDData(const
//   CRGBColour&, unsigned char*) 0x009E3F60 (catalog "Graphics_CopyPixelsToBuffer"), GetByteColourDepth 0x009E3800
//   (called again for every texel). CVolume::Lock has its same-TU copy below: with only a declaration VC7.1 hoisted the
//   box's dimension loads out of the row/slice loops.
// Shape: one running destination pointer (read from the box before GetDesc) advanced by the row pitch after each row
//   and by the slice pitch after each slice; the three loop counts are strength-reduced count-downs.
// Layout: CVolume (Ego_r.pdb, 8 bytes) vptr +0, PD3DVolume +4.
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
class CVolume:public CBaseClass{public:IDirect3DVolume9*PD3DVolume;virtual~CVolume();C3DExtentsI GetDimensions()const{_D3DVOLUME_DESC desc;PD3DVolume->GetDesc(&desc);return C3DExtentsI(desc.Width,desc.Height,desc.Depth);}CVolumeLockedBox Lock(unsigned long);CVolumeLockedBox ReadOnlyLock(unsigned long)const;void Unlock()const{PD3DVolume->UnlockBox();}void Clear(const CRGBColour& colour);};
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
static unsigned char g_mem[1024];
static void Case(unsigned long w, unsigned long h, unsigned long d, unsigned long bpp, long rowPitch, long slicePitch, _D3DFORMAT format)
{
    Reset(w, h, d, format, bpp, rowPitch, slicePitch, g_mem + 8);
    memset(g_mem, 0xEE, sizeof g_mem);
    static unsigned char expect[1024]; memset(expect, 0xEE, sizeof expect);
    // the running pointer: + RowPitch after every row, + SlicePitch on top after every slice
    unsigned char* dst = expect + 8;
    for (unsigned long z = 0; z < d; ++z) {
        for (unsigned long y = 0; y < h; ++y) {
            for (unsigned long x = 0; x < w; ++x) for (unsigned long k = 0; k < bpp; ++k) dst[x * bpp + k] = (unsigned char)(0x40 + k);
            dst += rowPitch;
        }
        dst += slicePitch;
    }
    TestVolume device; CVolume volume; volume.PD3DVolume = &device;
    CRGBColour colour(0x42, 0x41, 0x40, 0x43);            // B = 0x40
    volume.Clear(colour);
    CheckWholeVolumeLock();
    CHECK(g_inits == 1); CHECK(g_format == format);
    CHECK(g_converts == (int)(w * h * d)); CHECK(g_depthCalls == (int)(w * h * d));
    CHECK(g_descCalls == 2);                              // Lock's GetDimensions, then the row's own description
    CHECK(memcmp(g_mem, expect, sizeof g_mem) == 0);
    CHECK(volume.PD3DVolume == &device);
}
int main()
{
    Case(3, 2, 2, 4, 16, 64, D3DFMT_A8R8G8B8);
    Case(5, 3, 1, 2, 12, 7, D3DFMT_R5G6B5);
    Case(1, 1, 1, 1, 0, 0, D3DFMT_A8);
    Case(4, 0, 3, 4, 16, 64, D3DFMT_A8R8G8B8);            // no rows: nothing written
    Case(0, 2, 2, 4, 16, 64, D3DFMT_A8R8G8B8);            // no columns
    Case(2, 2, 0, 4, 16, 64, D3DFMT_A8R8G8B8);            // no slices
    if (g_fail) { printf("00a67950_TEST FAIL (%d)\n", g_fail); return 1; }
    puts("00a67950_TEST PASS"); return 0;
}