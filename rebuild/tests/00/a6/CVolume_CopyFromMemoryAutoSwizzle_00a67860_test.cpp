// Behaviour test for 0x00A67860 CVolume::CopyFromMemoryAutoSwizzle (links the REAL row object; the declarations below are the row's own prologue).
// CVolume::CopyFromMemoryAutoSwizzle(void* data, long rowPitch, long slicePitch) -- locks the volume and copies a linear source image into it row by row (width * bytes-per-pixel per row, the source and destination pitches stepped separately); no swizzle on PC
// Leaf: ?CopyFromMemoryAutoSwizzle@CVolume@@QAEXPAXJJ@Z
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A67860, 237 bytes (padded to 240), `ret 0Ch`.
//   PDB name ?CopyFromMemoryAutoSwizzle@CVolume@@QAEXPAXJJ@Z.
// Callees: CVolume::Lock(unsigned long) 0x00A677D0 (CVolumeLockedBox by value), IDirect3DVolume9 GetDesc (slot 8) and
//   UnlockBox (slot 10, the inline CVolume::Unlock), CPixelFormat::Initialise(D3DFORMAT) 0x009E3830,
//   CPixelFormat::GetByteColourDepth 0x009E3800; the row copy is the memcpy intrinsic (`rep movsd ; rep movsb`).
// Shape: two slice pointers stepped by the two slice pitches, two row pointers by the row pitches; the loop counts
//   are strength-reduced into count-down frame words (the row count homed in the dead `data` argument slot).
// Layout: CVolume (Ego_r.pdb, 8 bytes) vptr +0, PD3DVolume +4 (landed CVolume siblings 0x00A674E0 / 0x00A67480).
// Re-port (2026-11-19): the row's local CRGBColour / CPixelFormat / C3DExtentsI models (and its local _D3DFORMAT enum) are replaced by the
//   shared engine/CRGBColour.h, engine/CPixelFormat.h and engine/C3DExtentsI.h (Ego_d.pdb: a C2DExtentsI base plus long Z). CPixelFormat
//   opt-ins: FABLE_RETAIL_CPIXELFORMAT, _BYTECOLOURDEPTH_OUTOFLINE: GetByteColourDepth out of line, as retail calls it here.
//   The format is built by the header's inline CPixelFormat(_D3DFORMAT) constructor (FableWin.pdb ??0CPixelFormat@@QAE@W4_D3DFORMAT@@@Z: Initialise only, no -1 store).
// Same-TU copy: CVolume::Lock is the retail TU-mate 0x00A677D0, written verbatim as the landed owner row over the owner's own D3D /
//   CVolume declarations (the earlier noinline stand-in body is gone); it stays a call here, as in retail (`call 0A677D0h`), with no
//   inline control.
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_BYTECOLOURDEPTH_OUTOFLINE
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
class CVolume:public CBaseClass{public:IDirect3DVolume9*PD3DVolume;virtual~CVolume();C3DExtentsI GetDimensions()const{_D3DVOLUME_DESC desc;PD3DVolume->GetDesc(&desc);return C3DExtentsI(desc.Width,desc.Height,desc.Depth);}CVolumeLockedBox Lock(unsigned long);CVolumeLockedBox ReadOnlyLock(unsigned long)const;void Unlock()const{PD3DVolume->UnlockBox();}void CopyFromMemoryAutoSwizzle(void* data,long rowPitch,long slicePitch);};
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

static unsigned char g_vol[512], g_src[512];
static void Case(unsigned long w, unsigned long h, unsigned long d, unsigned long bpp, long volRow, long volSlice, long srcRow, long srcSlice, _D3DFORMAT format)
{
    Reset(w, h, d, format, bpp, volRow, volSlice, g_vol + 4);
    static unsigned char expect[512];
    for (int i = 0; i < 512; ++i) { g_vol[i] = expect[i] = (unsigned char)(0xA0 + i); g_src[i] = (unsigned char)(i * 3 + 1); }
    for (unsigned long z = 0; z < d; ++z) for (unsigned long y = 0; y < h; ++y)
        memcpy(expect + 4 + z * volSlice + y * volRow, g_src + 2 + z * srcSlice + y * srcRow, w * bpp);
    TestVolume device; CVolume volume; volume.PD3DVolume = &device;
    volume.CopyFromMemoryAutoSwizzle(g_src + 2, srcRow, srcSlice);
    CheckWholeVolumeLock();
    CHECK(g_inits == 1); CHECK(g_format == format); CHECK(g_depthCalls == 1); CHECK(g_converts == 0);
    CHECK(g_descCalls == 2);
    CHECK(memcmp(g_vol, expect, sizeof g_vol) == 0);
}
int main()
{
    Case(3, 2, 2, 2, 8, 20, 7, 16, D3DFMT_R5G6B5);        // rows of 6 bytes: rep movsd + rep movsb tail
    Case(5, 3, 2, 4, 24, 80, 21, 70, D3DFMT_A8R8G8B8);
    Case(7, 2, 3, 1, 9, 20, 8, 17, D3DFMT_A8);            // odd row length
    Case(4, 0, 2, 4, 16, 64, 16, 64, D3DFMT_A8R8G8B8);    // no rows
    Case(4, 2, 0, 4, 16, 64, 16, 64, D3DFMT_A8R8G8B8);    // no slices
    Case(0, 2, 2, 4, 16, 64, 16, 64, D3DFMT_A8R8G8B8);    // empty rows
    if (g_fail) { printf("00a67860_TEST FAIL (%d)\n", g_fail); return 1; }
    puts("00a67860_TEST PASS"); return 0;
}