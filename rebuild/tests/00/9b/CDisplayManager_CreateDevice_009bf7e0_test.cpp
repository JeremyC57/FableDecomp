// Behaviour test for 0x009BF7E0 CDisplayManager::CreateDevice (links the REAL row object: the parameter copy, the present-parameter
// block, the create flags, the expanded IsAntiAliasingModeValid / ReadDeviceRefreshRate / CalcPresentationIntervalTime /
// CVsyncCallbackTimer::SetDevice bodies and the buffer wrapping run as compiled; the CGenericException throw goes through the CRT).
// Fixtures (raw native layout, every offset read or written by an instruction of the row):
//   CDisplayManager: RenderTargetSurface +0x0C, RenderTargetDepthBuffer +0x1C, BackBufferSurface +0x2C, DepthBufferSurface +0x3C
//     (16-byte CSurface), D3D +0x54, D3DDevice +0x58, D3DDeviceCaps +0x5C (DeviceType +0x5C, AdapterOrdinal +0x60, DevCaps +0x78),
//     BackBufferDimensions +0x18C, RenderTargetDimensions +0x194, NoBackBuffers +0x19C, ExclusiveMode +0x1A0 (bool),
//     MultisampleType +0x1A4, BackBufferFormat +0x1A8, DepthBufferFormat +0x1AC, WindowHandle +0x1B4, CurrentMode +0x1B8 (16 bytes),
//     RefreshRate +0x1C8, PresentationInterval +0x1CC (8 bytes, VSyncCount +0x1D0), PresentationIntervalTime +0x1D4,
//     CurrentScreenIndex +0x218, PresentParams +0x21C (56 bytes, SDK order), LastDeviceCreationParameters +0x268 (44 bytes),
//     VsyncCallbackTimer +0x294 (pointer; CVsyncCallbackTimer::Device +0).
//   CCreateDeviceParameters: WinHandle +0, BackBufferDimensions +4, BackBufferBitDepth +0xC, ZBufferBitDepth +0x10, BackBufferCount
//     +0x14, RefreshRate +0x18, ExclusiveMode +0x1C, PresentationInterval +0x20, MultisampleType +0x28.
//   GD3DFormats: 36-byte entries, D3DFormat +0 (0x009BF8DC / 0x009BF96B). IDirect3D9 slots 8 / 11 / 16, IDirect3DDevice9 slots
//     18 / 40 (fake vtables, every other slot traps).
// Stubs (the row's out-of-line callees, recorded with their arguments in call order): CDisplayManager::GetClosestMode 0x009BF600,
//   GetClosestDepthBufferFormat 0x009BEBB0, CalculateD3DPresentationInterval 0x009BE2D0, SetupDepthAndBackBufferTextures 0x009BEE40,
//   SetViewport 0x009BEF80; CPixelFormat::Initialise 0x009E3830; CSurface::InitialiseFromPreallocatedSurface 0x009F2EC0,
//   GetDimensions 0x009F2F90, GetPixelFormat 0x009F2F60, operator= 0x009F2E70; the five D3D methods.
// Reference model (independent): builds the expected call list and the expected object image for every case -- the windowed flag,
//   the refresh rate (the requested one in exclusive mode, 0 windowed), the create flags (6 | 0x50 with the pure-device cap, 6 | 0x20
//   without), the multisample decision (requested, exclusive and CheckDeviceMultiSampleType == 0 exactly; the test and the format
//   wrap are skipped when the first two fail), the four failure exits (result 0, nothing further called or written), the
//   CGenericException throw when GetAdapterDisplayMode fails, the interval time vsync / refresh, the timer device, both surface
//   wraps (add_ref false), the dimension and format copies, the render-target assignments, the viewport box (0, 0, X, Y) and the
//   result 2.
// Checks: (1) hand-written cases on every decision (each HRESULT at 0, 1, -1 and E_FAIL); (2) 6000 seeded random cases; in each the
//   result, the thrown / not thrown state, the full call list with arguments, the present parameters as they were when
//   IDirect3D9::CreateDevice ran, the timer and the whole 0x2C0-byte object image are compared with the model.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct HWND__;
struct IDirect3DSurface9;
enum _D3DFORMAT { D3DFMT_FORCE_DWORD = 0x7fffffff };
struct _D3DDISPLAYMODE { unsigned int Width, Height, RefreshRate; _D3DFORMAT Format; };
class C2DExtentsI { public: long X, Y; };
class C2DBoxI { public: long v[4]; };
class CPixelFormat { public: long FormatIndex; void Initialise(_D3DFORMAT format); };
class CD3DFormatInfo { public: _D3DFORMAT D3DFormat; unsigned long pad[8]; };
class CExceptionBase { };
class CGenericException : public CExceptionBase { };
class CSurface
{
public:
    unsigned long d[4];
    void InitialiseFromPreallocatedSurface(IDirect3DSurface9* d3dsurface, bool add_ref);
    C2DExtentsI GetDimensions() const;
    CPixelFormat GetPixelFormat() const;
    CSurface& operator=(const CSurface& other);
};
class CDisplayManager
{
public:
    struct CCreateDeviceParameters
    {
        HWND__* WinHandle; C2DExtentsI BackBufferDimensions; long BackBufferBitDepth, ZBufferBitDepth, BackBufferCount, RefreshRate;
        bool ExclusiveMode; long PresentationInterval[2]; long MultisampleType;
    };
    class CPresentationIntervalDesc;
    enum ECreateDeviceSuccess { CREATE_DEVICE_FAILED = 0, CREATE_DEVICE_DEVICE_LOST = 1, CREATE_DEVICE_SUCCESS = 2 };
    void SetViewport(const C2DBoxI& box);
    static long Call(void* m, const CCreateDeviceParameters& p) { return ((CDisplayManager*)m)->CreateDevice(p); }
private:
    ECreateDeviceSuccess CreateDevice(const CCreateDeviceParameters& parameters);
    _D3DDISPLAYMODE GetClosestMode(const C2DExtentsI& resolution, long bit_depth, bool exclusive) const;
    CPixelFormat GetClosestDepthBufferFormat(const _D3DDISPLAYMODE& mode, long bit_depth) const;
    unsigned int CalculateD3DPresentationInterval(const CPresentationIntervalDesc& desc);
    bool SetupDepthAndBackBufferTextures();
};

enum { NFORMATS = 8 };
extern const CD3DFormatInfo GD3DFormats[NFORMATS] =
{
    { (_D3DFORMAT)0 }, { (_D3DFORMAT)22 }, { (_D3DFORMAT)21 }, { (_D3DFORMAT)23 }, { (_D3DFORMAT)75 }, { (_D3DFORMAT)77 }, { (_D3DFORMAT)80 }, { (_D3DFORMAT)71 }
};

template<class T> static T& At(void* base, int off) { return *(T*)((unsigned char*)base + off); }
typedef unsigned long UL;

// ---- call log
enum { E_MODE = 1, E_DEPTHFMT, E_INTERVAL, E_PIXINIT, E_CHECK, E_CREATE, E_ADAPTER, E_BACKBUF, E_SURFINIT, E_DIMS, E_PIXFMT, E_DEPTHSURF, E_ASSIGN, E_SETUP, E_VIEWPORT };
struct Ev { int id; UL a[7]; };
enum { MAXEV = 40 };
static Ev g_got[MAXEV], g_want[MAXEV]; static int g_ngot, g_nwant;
static void Put(Ev* list, int& n, int id, UL a0 = 0, UL a1 = 0, UL a2 = 0, UL a3 = 0, UL a4 = 0, UL a5 = 0, UL a6 = 0)
{
    if (n >= MAXEV) { ++n; return; }
    Ev& e = list[n++]; e.id = id; e.a[0] = a0; e.a[1] = a1; e.a[2] = a2; e.a[3] = a3; e.a[4] = a4; e.a[5] = a5; e.a[6] = a6;
}
#define GOT g_got, g_ngot
#define WANT g_want, g_nwant

// ---- per-case stub programme
struct FakeSurf { long X, Y, Format; };
struct Case
{
    CDisplayManager::CCreateDeviceParameters p;
    UL devtype, adapter, devcaps;
    _D3DDISPLAYMODE mode; long depth_index; UL interval;
    long pix_index;                       // what CPixelFormat::Initialise maps the back-buffer format to
    long check_hr, create_hr, adapter_hr, backbuf_hr, depthsurf_hr; UL refresh; bool setup_ok;
    FakeSurf back, depth;
};
static Case g_c;
static unsigned char g_obj[0x2C0], g_exp[0x2C0];
static void* g_d3d_vtbl[20]; static void* g_dev_vtbl[48];
static struct { void** vtbl; } g_d3d, g_dev;
static struct { void* Device; UL guard; } g_timer;
static unsigned char g_pp_seen[56];
static int g_fail, g_cases;

// ---- stubs
_D3DDISPLAYMODE CDisplayManager::GetClosestMode(const C2DExtentsI& resolution, long bit_depth, bool exclusive) const
{
    Put(GOT, E_MODE, (UL)this, (UL)&resolution, (UL)bit_depth, (UL)(exclusive ? 1 : 0));
    return g_c.mode;
}
CPixelFormat CDisplayManager::GetClosestDepthBufferFormat(const _D3DDISPLAYMODE& mode, long bit_depth) const
{
    Put(GOT, E_DEPTHFMT, (UL)this, (UL)&mode, (UL)bit_depth);
    CPixelFormat f; f.FormatIndex = g_c.depth_index; return f;
}
unsigned int CDisplayManager::CalculateD3DPresentationInterval(const CPresentationIntervalDesc& desc)
{
    Put(GOT, E_INTERVAL, (UL)this, (UL)&desc);
    return g_c.interval;
}
bool CDisplayManager::SetupDepthAndBackBufferTextures()
{
    Put(GOT, E_SETUP, (UL)this);
    return g_c.setup_ok;
}
void CDisplayManager::SetViewport(const C2DBoxI& box)
{
    Put(GOT, E_VIEWPORT, (UL)this, (UL)box.v[0], (UL)box.v[1], (UL)box.v[2], (UL)box.v[3]);
}
void CPixelFormat::Initialise(_D3DFORMAT format)
{
    Put(GOT, E_PIXINIT, (UL)format);
    FormatIndex = g_c.pix_index;
}
void CSurface::InitialiseFromPreallocatedSurface(IDirect3DSurface9* d3dsurface, bool add_ref)
{
    Put(GOT, E_SURFINIT, (UL)this, (UL)d3dsurface, (UL)(add_ref ? 1 : 0));
    d[0] = (UL)d3dsurface; d[1] = 0x51515151; d[2] = add_ref ? 0xADD0ADD0 : 0x0BADF00D; d[3] = (UL)this;
}
C2DExtentsI CSurface::GetDimensions() const
{
    Put(GOT, E_DIMS, (UL)this);
    C2DExtentsI e; e.X = ((FakeSurf*)d[0])->X; e.Y = ((FakeSurf*)d[0])->Y; return e;
}
CPixelFormat CSurface::GetPixelFormat() const
{
    Put(GOT, E_PIXFMT, (UL)this);
    CPixelFormat f; f.FormatIndex = ((FakeSurf*)d[0])->Format; return f;
}
CSurface& CSurface::operator=(const CSurface& other)
{
    Put(GOT, E_ASSIGN, (UL)this, (UL)&other);
    memcpy(d, other.d, sizeof d);
    return *this;
}
static long __stdcall D3D_GetAdapterDisplayMode(void* self, UL adapter, _D3DDISPLAYMODE* mode)
{
    Put(GOT, E_ADAPTER, (UL)self, adapter);
    mode->Width = 0x77777777; mode->Height = 0x66666666; mode->RefreshRate = g_c.refresh; mode->Format = (_D3DFORMAT)0x55555555;
    return g_c.adapter_hr;
}
static long __stdcall D3D_CheckDeviceMultiSampleType(void* self, UL adapter, UL devtype, UL format, UL windowed, UL type, UL* quality)
{
    Put(GOT, E_CHECK, (UL)self, adapter, devtype, format, windowed, type, (UL)quality);
    return g_c.check_hr;
}
static long __stdcall D3D_CreateDevice(void* self, UL adapter, UL devtype, HWND__* window, UL flags, void* pp, void** device)
{
    Put(GOT, E_CREATE, (UL)self, adapter, devtype, (UL)window, flags, (UL)pp, (UL)device);
    memcpy(g_pp_seen, pp, sizeof g_pp_seen);
    if (g_c.create_hr >= 0) *device = &g_dev;
    return g_c.create_hr;
}
static long __stdcall Dev_GetBackBuffer(void* self, UL swap_chain, UL back_buffer, UL type, void** surface)
{
    Put(GOT, E_BACKBUF, (UL)self, swap_chain, back_buffer, type);
    *surface = &g_c.back;
    return g_c.backbuf_hr;
}
static long __stdcall Dev_GetDepthStencilSurface(void* self, void** surface)
{
    Put(GOT, E_DEPTHSURF, (UL)self);
    *surface = &g_c.depth;
    return g_c.depthsurf_hr;
}
static void Trap() { printf("009bf7e0_TEST FAIL (unexpected virtual slot)\n"); exit(1); }

// ---- reference model: fills g_want and g_exp, returns the expected result (-1 = throws)
static long Model()
{
    const CDisplayManager::CCreateDeviceParameters& p = g_c.p;
    unsigned char* o = g_exp;
    g_nwant = 0;
    memcpy(o + 0x268, &p, 44);
    Put(WANT, E_MODE, (UL)g_obj, (UL)&p.BackBufferDimensions, (UL)p.BackBufferBitDepth, p.ExclusiveMode ? 1 : 0);
    memcpy(o + 0x1B8, &g_c.mode, 16);
    At<HWND__*>(o, 0x1B4) = p.WinHandle;
    At<long>(o, 0x19C) = p.BackBufferCount;
    At<unsigned char>(o, 0x1A0) = p.ExclusiveMode ? 1 : 0;
    At<long>(o, 0x218) = 0;
    At<long>(o, 0x1CC) = p.PresentationInterval[0]; At<long>(o, 0x1D0) = p.PresentationInterval[1];
    UL pp[14]; memset(pp, 0, sizeof pp);
    pp[0] = g_c.mode.Width; pp[1] = g_c.mode.Height; pp[2] = (UL)g_c.mode.Format;
    pp[8] = p.ExclusiveMode ? 0 : 1;                           // Windowed
    pp[6] = 1;                                                 // SwapEffect: D3DSWAPEFFECT_DISCARD
    pp[9] = 1;                                                 // EnableAutoDepthStencil
    Put(WANT, E_DEPTHFMT, (UL)g_obj, (UL)(g_obj + 0x1B8), (UL)p.ZBufferBitDepth);
    pp[10] = (UL)GD3DFormats[g_c.depth_index].D3DFormat;       // AutoDepthStencilFormat
    pp[7] = (UL)p.WinHandle;                                   // hDeviceWindow
    pp[3] = (UL)p.BackBufferCount;
    pp[12] = p.ExclusiveMode ? (UL)p.RefreshRate : 0;          // FullScreen_RefreshRateInHz
    Put(WANT, E_INTERVAL, (UL)g_obj, (UL)(g_obj + 0x1CC));
    pp[13] = g_c.interval;                                     // PresentationInterval
    UL flags = (g_c.devcaps & 0x00100000) ? 0x56 : 0x26;
    long ms = 0;
    if (p.MultisampleType != 0 && p.ExclusiveMode)
    {
        Put(WANT, E_PIXINIT, (UL)g_c.mode.Format);
        Put(WANT, E_CHECK, (UL)&g_d3d, 0, g_c.devtype, (UL)GD3DFormats[g_c.pix_index].D3DFormat, 0, (UL)p.MultisampleType, 0);
        if (g_c.check_hr == 0) ms = p.MultisampleType;
    }
    At<long>(o, 0x1A4) = ms;
    pp[4] = (UL)ms;
    memcpy(o + 0x21C, pp, 56);
    Put(WANT, E_CREATE, (UL)&g_d3d, g_c.adapter, g_c.devtype, (UL)p.WinHandle, flags, (UL)(g_obj + 0x21C), (UL)(g_obj + 0x58));
    if (g_c.create_hr < 0) return 0;
    At<void*>(o, 0x58) = &g_dev;
    Put(WANT, E_ADAPTER, (UL)&g_d3d, 0);
    if (g_c.adapter_hr < 0) return -1;
    At<UL>(o, 0x1C8) = g_c.refresh;
    At<float>(o, 0x1D4) = (float)p.PresentationInterval[1] / (float)(long)g_c.refresh;
    Put(WANT, E_BACKBUF, (UL)&g_dev, 0, 0, 0);
    if (g_c.backbuf_hr < 0) return 0;
    Put(WANT, E_SURFINIT, (UL)(g_obj + 0x2C), (UL)&g_c.back, 0);
    UL* bs = &At<UL>(o, 0x2C); bs[0] = (UL)&g_c.back; bs[1] = 0x51515151; bs[2] = 0x0BADF00D; bs[3] = (UL)(g_obj + 0x2C);
    Put(WANT, E_DIMS, (UL)(g_obj + 0x2C));
    At<long>(o, 0x18C) = g_c.back.X; At<long>(o, 0x190) = g_c.back.Y;
    Put(WANT, E_PIXFMT, (UL)(g_obj + 0x2C));
    At<long>(o, 0x1A8) = g_c.back.Format;
    Put(WANT, E_DEPTHSURF, (UL)&g_dev);
    if (g_c.depthsurf_hr < 0) return 0;
    Put(WANT, E_SURFINIT, (UL)(g_obj + 0x3C), (UL)&g_c.depth, 0);
    UL* ds = &At<UL>(o, 0x3C); ds[0] = (UL)&g_c.depth; ds[1] = 0x51515151; ds[2] = 0x0BADF00D; ds[3] = (UL)(g_obj + 0x3C);
    Put(WANT, E_PIXFMT, (UL)(g_obj + 0x3C));
    At<long>(o, 0x1AC) = g_c.depth.Format;
    Put(WANT, E_ASSIGN, (UL)(g_obj + 0x0C), (UL)(g_obj + 0x2C));
    memcpy(o + 0x0C, o + 0x2C, 16);
    Put(WANT, E_ASSIGN, (UL)(g_obj + 0x1C), (UL)(g_obj + 0x3C));
    memcpy(o + 0x1C, o + 0x3C, 16);
    At<long>(o, 0x194) = g_c.back.X; At<long>(o, 0x198) = g_c.back.Y;
    Put(WANT, E_SETUP, (UL)g_obj);
    if (!g_c.setup_ok) return 0;
    Put(WANT, E_VIEWPORT, (UL)g_obj, 0, 0, (UL)g_c.back.X, (UL)g_c.back.Y);
    return 2;
}

static UL g_seed = 2024;
static UL Rnd() { g_seed = g_seed * 1664525u + 1013904223u; return g_seed >> 8; }
static void Fail(const char* what, long got, long want)
{
    if (g_fail++ < 16) printf("FAIL case %d: %s got 0x%lX want 0x%lX\n", g_cases, what, got, want);
}
static void Run()
{
    ++g_cases;
    int i;
    for (i = 0; i < (int)sizeof g_obj; ++i) g_obj[i] = (unsigned char)(Rnd() | 1);
    At<void*>(g_obj, 0x54) = &g_d3d;
    At<UL>(g_obj, 0x5C) = g_c.devtype; At<UL>(g_obj, 0x60) = g_c.adapter; At<UL>(g_obj, 0x78) = g_c.devcaps;
    At<void*>(g_obj, 0x294) = &g_timer;
    memcpy(g_exp, g_obj, sizeof g_obj);
    g_timer.Device = (void*)0x12345678; g_timer.guard = 0xFEEDFACE;
    memset(g_pp_seen, 0xEE, sizeof g_pp_seen);
    CDisplayManager::CCreateDeviceParameters p0 = g_c.p;
    long want = Model();
    g_ngot = 0;
    long got; bool threw = false;
    try { got = CDisplayManager::Call(g_obj, g_c.p); } catch (CGenericException&) { threw = true; got = -1; }
    if (threw != (want == -1)) Fail("thrown", threw, want == -1);
    if (got != want) Fail("result", got, want);
    if (memcmp(&p0, &g_c.p, sizeof p0)) Fail("parameters written", 0, 0);
    if (g_ngot != g_nwant) Fail("call count", g_ngot, g_nwant);
    for (i = 0; i < g_ngot && i < g_nwant && i < MAXEV; ++i)
    {
        if (g_got[i].id != g_want[i].id) { Fail("call id", g_got[i].id, g_want[i].id); break; }
        for (int k = 0; k < 7; ++k)
            if (g_got[i].a[k] != g_want[i].a[k]) { Fail("call argument", (long)g_got[i].a[k], (long)g_want[i].a[k]); printf("  (call %d id %d arg %d)\n", i, g_got[i].id, k); }
    }
    for (i = 0; i < (int)sizeof g_obj; ++i)
        if (g_obj[i] != g_exp[i]) { Fail("object byte", g_obj[i], g_exp[i]); printf("  (offset 0x%X)\n", i); break; }
    if (memcmp(g_pp_seen, g_exp + 0x21C, 56)) Fail("present parameters at IDirect3D9::CreateDevice", 0, 0);
    bool timer_set = want != -1 && g_c.create_hr >= 0;
    if (g_timer.Device != (timer_set ? (void*)&g_dev : (void*)0x12345678)) Fail("timer device", (long)g_timer.Device, timer_set);
    if (g_timer.guard != 0xFEEDFACE) Fail("timer guard", (long)g_timer.guard, 0);
}

static void Base()
{
    memset(&g_c, 0, sizeof g_c);
    g_c.p.WinHandle = (HWND__*)0x00C0FFEE; g_c.p.BackBufferDimensions.X = 1024; g_c.p.BackBufferDimensions.Y = 768;
    g_c.p.BackBufferBitDepth = 32; g_c.p.ZBufferBitDepth = 24; g_c.p.BackBufferCount = 2; g_c.p.RefreshRate = 85;
    g_c.p.ExclusiveMode = true; g_c.p.PresentationInterval[0] = 3; g_c.p.PresentationInterval[1] = 2; g_c.p.MultisampleType = 4;
    g_c.devtype = 1; g_c.adapter = 3; g_c.devcaps = 0x00100000 | 0x00010051;
    g_c.mode.Width = 1024; g_c.mode.Height = 768; g_c.mode.RefreshRate = 85; g_c.mode.Format = (_D3DFORMAT)22;
    g_c.depth_index = 4; g_c.interval = 0x80000000; g_c.pix_index = 1;
    g_c.check_hr = 0; g_c.create_hr = 0; g_c.adapter_hr = 0; g_c.backbuf_hr = 0; g_c.depthsurf_hr = 0; g_c.refresh = 60; g_c.setup_ok = true;
    g_c.back.X = 1024; g_c.back.Y = 768; g_c.back.Format = 1; g_c.depth.X = 1024; g_c.depth.Y = 768; g_c.depth.Format = 4;
}
static const long HRS[4] = { 0, 1, -1, (long)0x80004005 };

static void Exact()
{
    int i;
    Base(); Run();                                            // everything succeeds, multisampling accepted
    Base(); g_c.p.ExclusiveMode = false; Run();               // windowed: rate 0, no multisample test
    Base(); g_c.p.ExclusiveMode = false; g_c.p.MultisampleType = 0; Run();
    Base(); g_c.p.MultisampleType = 0; Run();                 // nothing requested: no multisample test
    Base(); g_c.p.RefreshRate = 0; Run();                     // default rate
    Base(); g_c.devcaps = 0xFFEFFFFF; Run();                  // every cap but the pure device: software vertex processing
    Base(); g_c.devcaps = 0x00100000; Run();
    Base(); g_c.devcaps = 0; Run();
    for (i = 0; i < 4; ++i) { Base(); g_c.check_hr = HRS[i]; Run(); }        // only D3D_OK accepts the mode
    for (i = 0; i < 4; ++i) { Base(); g_c.create_hr = HRS[i]; Run(); }
    for (i = 0; i < 4; ++i) { Base(); g_c.adapter_hr = HRS[i]; Run(); }      // negative: CGenericException
    for (i = 0; i < 4; ++i) { Base(); g_c.backbuf_hr = HRS[i]; Run(); }
    for (i = 0; i < 4; ++i) { Base(); g_c.depthsurf_hr = HRS[i]; Run(); }
    Base(); g_c.setup_ok = false; Run();
    Base(); g_c.back.X = 640; g_c.back.Y = 480; g_c.back.Format = 2; g_c.depth.Format = 5; g_c.depth.X = 7; g_c.depth.Y = 9; Run();
    Base(); g_c.p.PresentationInterval[1] = 1; g_c.refresh = 3; Run();       // inexact quotient
    Base(); g_c.p.PresentationInterval[1] = 0; g_c.refresh = 75; Run();
    Base(); g_c.pix_index = 7; g_c.depth_index = 6; g_c.mode.Format = (_D3DFORMAT)23; Run();
}

int main()
{
    int i;
    for (i = 0; i < 20; ++i) g_d3d_vtbl[i] = (void*)Trap;
    for (i = 0; i < 48; ++i) g_dev_vtbl[i] = (void*)Trap;
    g_d3d_vtbl[8] = (void*)D3D_GetAdapterDisplayMode; g_d3d_vtbl[11] = (void*)D3D_CheckDeviceMultiSampleType; g_d3d_vtbl[16] = (void*)D3D_CreateDevice;
    g_dev_vtbl[18] = (void*)Dev_GetBackBuffer; g_dev_vtbl[40] = (void*)Dev_GetDepthStencilSurface;
    g_d3d.vtbl = g_d3d_vtbl; g_dev.vtbl = g_dev_vtbl;
    Exact();
    for (int n = 0; n < 6000; ++n)
    {
        Base();
        g_c.p.WinHandle = (HWND__*)(Rnd() | 0x10000);
        g_c.p.BackBufferDimensions.X = 320 + Rnd() % 2000; g_c.p.BackBufferDimensions.Y = 200 + Rnd() % 1400;
        g_c.p.BackBufferBitDepth = (Rnd() & 1) ? 16 : 32; g_c.p.ZBufferBitDepth = 16 + 8 * (Rnd() % 3);
        g_c.p.BackBufferCount = 1 + Rnd() % 3; g_c.p.RefreshRate = (Rnd() % 4 == 0) ? 0 : 50 + Rnd() % 70;
        g_c.p.ExclusiveMode = (Rnd() % 3) != 0;
        g_c.p.PresentationInterval[0] = Rnd() % 5; g_c.p.PresentationInterval[1] = Rnd() % 5;
        g_c.p.MultisampleType = (Rnd() % 3 == 0) ? 0 : 2 + Rnd() % 15;
        g_c.devtype = 1 + Rnd() % 3; g_c.adapter = Rnd() % 4; g_c.devcaps = (Rnd() << 8) ^ Rnd();
        g_c.mode.Width = 320 + Rnd() % 2000; g_c.mode.Height = 200 + Rnd() % 1400; g_c.mode.RefreshRate = Rnd() % 120;
        g_c.mode.Format = (_D3DFORMAT)(20 + Rnd() % 6);
        g_c.depth_index = Rnd() % NFORMATS; g_c.pix_index = Rnd() % NFORMATS; g_c.interval = (Rnd() & 1) ? 0x80000000 : 1 + Rnd() % 4;
        g_c.check_hr = HRS[(Rnd() % 3 == 0) ? 1 + Rnd() % 3 : 0];
        g_c.create_hr = HRS[(Rnd() % 8 == 0) ? 2 + Rnd() % 2 : Rnd() % 2];
        g_c.adapter_hr = HRS[(Rnd() % 8 == 0) ? 2 + Rnd() % 2 : Rnd() % 2];
        g_c.backbuf_hr = HRS[(Rnd() % 8 == 0) ? 2 + Rnd() % 2 : Rnd() % 2];
        g_c.depthsurf_hr = HRS[(Rnd() % 8 == 0) ? 2 + Rnd() % 2 : Rnd() % 2];
        g_c.refresh = 50 + Rnd() % 110; g_c.setup_ok = (Rnd() % 8) != 0;
        g_c.back.X = 320 + Rnd() % 2000; g_c.back.Y = 200 + Rnd() % 1400; g_c.back.Format = Rnd() % NFORMATS;
        g_c.depth.X = Rnd() % 4000; g_c.depth.Y = Rnd() % 4000; g_c.depth.Format = Rnd() % NFORMATS;
        Run();
    }
    if (g_fail) { printf("009bf7e0_TEST FAIL (%d of %d)\n", g_fail, g_cases); return 1; }
    printf("009bf7e0_TEST PASS (%d cases)\n", g_cases);
    return 0;
}