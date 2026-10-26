// Behaviour test for 009bfb30 CDisplayManager::ResetDevice: final.cpp body verbatim; the D3D / device interfaces, surfaces,
// textures and the display-manager helpers are stubs recording their calls.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
#define __forceinline inline
#include <string.h>
#define long int          // the model's longs are 32-bit words (LP64 g++ would make the 0x88760868 device-lost code positive)
#endif
extern "C" int __cdecl printf(const char* format, ...);
// CDisplayManager::ResetDevice(const CCreateDeviceParameters&) -- remember the parameters, release the device-dependent surfaces and the back-buffer copy texture, re-pick the display mode, refill the present parameters (multisample validated against the device, interval only when exclusive), reset the device (device-lost reports 1 and sets the lost flag), re-read the adapter refresh rate and re-wrap the back / depth buffers, then rebuild the copy textures and run the post-reset callbacks
// Leaf: ResetDevice
#include <string.h>
struct _D3DDISPLAYMODE {
    unsigned long Width;
    unsigned long Height;
    unsigned long RefreshRate;
    unsigned long Format;                          // D3DFORMAT
};
struct _D3DPRESENT_PARAMETERS_ {
    unsigned int  BackBufferWidth;
    unsigned int  BackBufferHeight;
    unsigned long BackBufferFormat;
    unsigned int  BackBufferCount;
    unsigned long MultiSampleType;
    unsigned long MultiSampleQuality;
    unsigned long SwapEffect;
    void*         hDeviceWindow;
    int           Windowed;
    int           EnableAutoDepthStencil;
    unsigned long AutoDepthStencilFormat;
    unsigned long Flags;
    unsigned int  FullScreen_RefreshRateInHz;
    unsigned int  PresentationInterval;
};
struct IDirect3DSurface9;
struct IDirect3DDevice9;
struct IDirect3D9 {
    virtual long __stdcall QueryInterface(const void* iid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall RegisterSoftwareDevice(void* initializeFunction);
    virtual unsigned long __stdcall GetAdapterCount();
    virtual long __stdcall GetAdapterIdentifier(unsigned long adapter, unsigned long flags, void* identifier);
    virtual unsigned long __stdcall GetAdapterModeCount(unsigned long adapter, unsigned long format);
    virtual long __stdcall EnumAdapterModes(unsigned long adapter, unsigned long format, unsigned long mode, _D3DDISPLAYMODE* out);
    virtual long __stdcall GetAdapterDisplayMode(unsigned long adapter, _D3DDISPLAYMODE* mode);                    // slot 8
    virtual long __stdcall CheckDeviceType(unsigned long, unsigned long, unsigned long, unsigned long, int);
    virtual long __stdcall CheckDeviceFormat(unsigned long, unsigned long, unsigned long, unsigned long, unsigned long, unsigned long);
    virtual long __stdcall CheckDeviceMultiSampleType(unsigned long adapter, unsigned long deviceType, unsigned long format, int windowed,
                                                      unsigned long multiSampleType, unsigned long* qualityLevels);   // slot 11
    virtual long __stdcall CheckDepthStencilMatch(unsigned long, unsigned long, unsigned long, unsigned long, unsigned long);
    virtual long __stdcall CheckDeviceFormatConversion(unsigned long, unsigned long, unsigned long, unsigned long);
    virtual long __stdcall GetDeviceCaps(unsigned long, unsigned long, void*);
    virtual void* __stdcall GetAdapterMonitor(unsigned long);
    virtual long __stdcall CreateDevice(unsigned long adapter, unsigned long deviceType, void* focusWindow, unsigned long behaviorFlags,
                                        _D3DPRESENT_PARAMETERS_* presentationParameters, IDirect3DDevice9** device);   // slot 16
};
struct IDirect3DDevice9 {
    virtual long __stdcall QueryInterface(const void* iid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall v03(); virtual long __stdcall v04(); virtual long __stdcall v05(); virtual long __stdcall v06();
    virtual long __stdcall v07(); virtual long __stdcall v08(); virtual long __stdcall v09(); virtual long __stdcall v10();
    virtual long __stdcall v11(); virtual long __stdcall v12(); virtual long __stdcall v13(); virtual long __stdcall v14();
    virtual long __stdcall v15();
    virtual long __stdcall Reset(_D3DPRESENT_PARAMETERS_* presentationParameters);   // slot 16
    virtual long __stdcall v17();
    virtual long __stdcall GetBackBuffer(unsigned int swapChain, unsigned int backBuffer, unsigned long type, IDirect3DSurface9** out);   // slot 18
    virtual long __stdcall v19(); virtual long __stdcall v20(); virtual long __stdcall v21(); virtual long __stdcall v22();
    virtual long __stdcall v23(); virtual long __stdcall v24(); virtual long __stdcall v25(); virtual long __stdcall v26();
    virtual long __stdcall v27(); virtual long __stdcall v28(); virtual long __stdcall v29(); virtual long __stdcall v30();
    virtual long __stdcall v31(); virtual long __stdcall v32(); virtual long __stdcall v33(); virtual long __stdcall v34();
    virtual long __stdcall v35(); virtual long __stdcall v36(); virtual long __stdcall v37(); virtual long __stdcall v38();
    virtual long __stdcall v39();
    virtual long __stdcall GetDepthStencilSurface(IDirect3DSurface9** out);   // slot 40
};
class C2DExtentsI { public: long X; long Y; };
class C2DBoxI { public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) {}
};
struct SPixelFormatInfo {              // 36-byte format table entry
    unsigned long m_d3dFormat;         // +0 D3DFORMAT
    unsigned char _rest[0x20];
};
extern SPixelFormatInfo g_PixelFormatTable[];   // 0x0129BA40
class CPixelFormat { public:
    long FormatIndex;
    CPixelFormat() {}
    void Initialise(unsigned long format);         // 0x009E3830
    unsigned long GetD3DFormat() const { return g_PixelFormatTable[FormatIndex].m_d3dFormat; }
};
struct CPresentationIntervalDesc {
    bool PresentImmediate;
    bool PresentImmediateIfSyncMissed;
    long VSyncCount;
};
struct CCreateDeviceParameters {
    void*                     WinHandle;            // +0x00
    C2DExtentsI               BackBufferDimensions; // +0x04
    long                      BackBufferBitDepth;   // +0x0c
    long                      ZBufferBitDepth;      // +0x10
    long                      BackBufferCount;      // +0x14
    long                      RefreshRate;          // +0x18
    bool                      ExclusiveMode;        // +0x1c
    CPresentationIntervalDesc PresentationInterval; // +0x20
    long                      MultisampleType;      // +0x28
};
class CSurface { public:
    void*              _vtable;
    IDirect3DSurface9* PD3DSurface;
    unsigned long      AllocationSource;
    void*              PAllocatedMemory;
    void InitialiseFromPreallocatedSurface(IDirect3DSurface9* surface, bool flag);   // 0x009F2EC0
    C2DExtentsI GetDimensions() const;             // 0x009F2F90
    CPixelFormat GetPixelFormat() const;           // 0x009F2F60
    CSurface& operator=(const CSurface& other);    // 0x009F2E70
    void Uninitialise();                           // 0x009F2E20
};
class CTexture { public: void* _vtable; void* PD3DTexture; void Uninitialise(); };   // 0x009F9F70
struct C2DExtentsIRet { long X; long Y; };
class CVsyncCallbackTimer { public: IDirect3DDevice9* Device; };
struct CD3DFailure { CD3DFailure() {} };
enum ECreateDeviceResult { CREATE_DEVICE_FAILED = 0, CREATE_DEVICE_LOST = 1, CREATE_DEVICE_OK = 2 };

class CDisplayManager { public:
    void*             _vtable;                     // +0x00
    void*             SystemManager;               // +0x04
    void*             PRenderManager;              // +0x08
    CSurface          RenderTargetSurface;         // +0x0c
    CSurface          RenderTargetDepthBuffer;     // +0x1c
    CSurface          BackBufferSurface;           // +0x2c
    CSurface          DepthBufferSurface;          // +0x3c
    CTexture          BackBufferCopyTexture;       // +0x4c
    IDirect3D9*       D3D;                         // +0x54
    IDirect3DDevice9* D3DDevice;                   // +0x58
    unsigned long     DeviceType;                  // +0x5c D3DDeviceCaps.DeviceType
    unsigned long     AdapterOrdinal;              // +0x60 D3DDeviceCaps.AdapterOrdinal
    unsigned char     _caps08[0x14];
    unsigned long     DevCaps;                     // +0x78 D3DDeviceCaps.DevCaps
    unsigned char     _caps20[0x18c - 0x7c];
    C2DExtentsI       BackBufferDimensions;        // +0x18c
    C2DExtentsI       RenderTargetDimensions;      // +0x194
    long              NoBackBuffers;               // +0x19c
    bool              ExclusiveMode;               // +0x1a0
    long              MultisampleType;             // +0x1a4
    CPixelFormat      BackBufferFormat;            // +0x1a8
    CPixelFormat      DepthBufferFormat;           // +0x1ac
    bool              UsingRenderManager;          // +0x1b0
    bool              UsingShaderRenderManager;    // +0x1b1
    void*             WindowHandle;                // +0x1b4
    _D3DDISPLAYMODE   CurrentMode;                 // +0x1b8
    long              RefreshRate;                 // +0x1c8
    CPresentationIntervalDesc PresentationInterval;   // +0x1cc
    float             PresentationIntervalTime;    // +0x1d4
    unsigned char     _pad1d8[0x218 - 0x1d8];
    long              CurrentScreenIndex;          // +0x218
    _D3DPRESENT_PARAMETERS_ PresentParams;         // +0x21c
    bool              VirtualCoordsResolutionIndependant;   // +0x254
    bool              DeviceLostFlag;              // +0x255
    unsigned char     _pad256[0x268 - 0x256];
    CCreateDeviceParameters LastDeviceCreationParameters;   // +0x268
    CVsyncCallbackTimer* VsyncCallbackTimer;       // +0x294
    _D3DDISPLAYMODE GetClosestMode(const C2DExtentsI& resolution, unsigned long colourDepth, bool fullScreen);   // 0x009BF600
    CPixelFormat GetDepthBufferFormat(const _D3DDISPLAYMODE& mode, long depthBits);   // 0x009BEBB0
    unsigned long CalculateD3DPresentationInterval(const CPresentationIntervalDesc& desc);   // 0x009BE2D0
    bool SetupDepthAndBackBufferTextures();        // 0x009BEE40
    void SetViewport(const C2DBoxI& box);          // 0x009BEF80
    void PreDeviceReset();                         // 0x009BF500
    bool PostDeviceReset();                        // 0x009BF550
    ECreateDeviceResult ResetDevice(const CCreateDeviceParameters& params);
};

ECreateDeviceResult CDisplayManager::ResetDevice(const CCreateDeviceParameters& params)
{
    LastDeviceCreationParameters = params;
    PreDeviceReset();
    BackBufferSurface.Uninitialise();
    DepthBufferSurface.Uninitialise();
    RenderTargetSurface.Uninitialise();
    RenderTargetDepthBuffer.Uninitialise();
    BackBufferCopyTexture.Uninitialise();
    CurrentMode = GetClosestMode(params.BackBufferDimensions, params.BackBufferBitDepth, params.ExclusiveMode);
    CurrentScreenIndex = 0;
    NoBackBuffers = params.BackBufferCount;
    PresentationInterval = params.PresentationInterval;
    memset(&PresentParams, 0, sizeof(PresentParams));
    PresentParams.BackBufferWidth = CurrentMode.Width;
    PresentParams.BackBufferHeight = CurrentMode.Height;
    PresentParams.BackBufferFormat = CurrentMode.Format;
    PresentParams.Windowed = !params.ExclusiveMode;
    if (params.MultisampleType != 0 && !PresentParams.Windowed) {
        CPixelFormat format;
        format.Initialise(PresentParams.BackBufferFormat);
        long multisample = params.MultisampleType;
        if (D3D->CheckDeviceMultiSampleType(0, DeviceType, format.GetD3DFormat(), 0, multisample, 0) == 0)
            MultisampleType = params.MultisampleType;
        else
            MultisampleType = 0;
    } else
        MultisampleType = 0;
    PresentParams.MultiSampleType = MultisampleType;
    PresentParams.SwapEffect = 1;
    PresentParams.EnableAutoDepthStencil = 1;
    PresentParams.AutoDepthStencilFormat = GetDepthBufferFormat(CurrentMode, params.ZBufferBitDepth).GetD3DFormat();
    PresentParams.hDeviceWindow = WindowHandle;
    PresentParams.BackBufferCount = NoBackBuffers;
    if (params.ExclusiveMode) {
        PresentParams.FullScreen_RefreshRateInHz = params.RefreshRate;
        PresentParams.PresentationInterval = CalculateD3DPresentationInterval(PresentationInterval);
    } else {
        PresentParams.FullScreen_RefreshRateInHz = 0;
        PresentParams.PresentationInterval = 0;
    }
    long result = D3DDevice->Reset(&PresentParams);
    if (result < 0) {
        if (result == (long)0x88760868) {
            DeviceLostFlag = true;
            return CREATE_DEVICE_LOST;
        }
        DeviceLostFlag = false;
        return CREATE_DEVICE_FAILED;
    }
    DeviceLostFlag = false;
    {
        _D3DDISPLAYMODE mode;
        if (D3D->GetAdapterDisplayMode(0, &mode) < 0)
            throw CD3DFailure();
        RefreshRate = mode.RefreshRate;
        PresentationIntervalTime = (float)PresentationInterval.VSyncCount / (long)mode.RefreshRate;
    }
    IDirect3DSurface9* backBuffer;
    if (D3DDevice->GetBackBuffer(0, 0, 0, &backBuffer) < 0)
        return CREATE_DEVICE_FAILED;
    BackBufferSurface.InitialiseFromPreallocatedSurface(backBuffer, false);
    BackBufferDimensions = BackBufferSurface.GetDimensions();
    BackBufferFormat = BackBufferSurface.GetPixelFormat();
    IDirect3DSurface9* depthBuffer;
    if (D3DDevice->GetDepthStencilSurface(&depthBuffer) < 0)
        return CREATE_DEVICE_FAILED;
    DepthBufferSurface.InitialiseFromPreallocatedSurface(depthBuffer, false);
    DepthBufferFormat = DepthBufferSurface.GetPixelFormat();
    RenderTargetSurface = BackBufferSurface;
    RenderTargetDepthBuffer = DepthBufferSurface;
    RenderTargetDimensions = BackBufferDimensions;
    ExclusiveMode = params.ExclusiveMode;
    if (!SetupDepthAndBackBufferTextures())
        return CREATE_DEVICE_FAILED;
    return PostDeviceReset() ? CREATE_DEVICE_OK : CREATE_DEVICE_FAILED;
}

static int g_fail;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); g_fail = 1; } }
SPixelFormatInfo g_PixelFormatTable[64];
static int g_uninit, g_texUninit, g_pre, g_assign, g_msCalls, g_wrapCalls;
static unsigned long g_msDevType, g_msFormat, g_msType;
static long g_msResult, g_resetResult, g_modeResult, g_bbResult, g_dsResult;
static unsigned long g_refresh;
static bool g_setupOk, g_postOk, g_closestFull;
static _D3DPRESENT_PARAMETERS_* g_resetPP;
static IDirect3DSurface9* const g_back = (IDirect3DSurface9*)0x1000;
static IDirect3DSurface9* const g_depth = (IDirect3DSurface9*)0x2000;

long IDirect3D9::QueryInterface(const void*, void**) { return 0; }
unsigned long IDirect3D9::AddRef() { return 0; }
unsigned long IDirect3D9::Release() { return 0; }
long IDirect3D9::RegisterSoftwareDevice(void*) { return 0; }
unsigned long IDirect3D9::GetAdapterCount() { return 0; }
long IDirect3D9::GetAdapterIdentifier(unsigned long, unsigned long, void*) { return 0; }
unsigned long IDirect3D9::GetAdapterModeCount(unsigned long, unsigned long) { return 0; }
long IDirect3D9::EnumAdapterModes(unsigned long, unsigned long, unsigned long, _D3DDISPLAYMODE*) { return 0; }
long IDirect3D9::GetAdapterDisplayMode(unsigned long adapter, _D3DDISPLAYMODE* mode) { mode->RefreshRate = g_refresh; return g_modeResult; }
long IDirect3D9::CheckDeviceType(unsigned long, unsigned long, unsigned long, unsigned long, int) { return 0; }
long IDirect3D9::CheckDeviceFormat(unsigned long, unsigned long, unsigned long, unsigned long, unsigned long, unsigned long) { return 0; }
long IDirect3D9::CheckDeviceMultiSampleType(unsigned long adapter, unsigned long deviceType, unsigned long format, int windowed,
                                            unsigned long multiSampleType, unsigned long* qualityLevels)
{
    ++g_msCalls; g_msDevType = deviceType; g_msFormat = format; g_msType = multiSampleType;
    return (adapter == 0 && windowed == 0 && qualityLevels == 0) ? g_msResult : -99;
}
long IDirect3D9::CheckDepthStencilMatch(unsigned long, unsigned long, unsigned long, unsigned long, unsigned long) { return 0; }
long IDirect3D9::CheckDeviceFormatConversion(unsigned long, unsigned long, unsigned long, unsigned long) { return 0; }
long IDirect3D9::GetDeviceCaps(unsigned long, unsigned long, void*) { return 0; }
void* IDirect3D9::GetAdapterMonitor(unsigned long) { return 0; }
long IDirect3D9::CreateDevice(unsigned long, unsigned long, void*, unsigned long, _D3DPRESENT_PARAMETERS_*, IDirect3DDevice9**) { return 0; }

long IDirect3DDevice9::QueryInterface(const void*, void**) { return 0; }
unsigned long IDirect3DDevice9::AddRef() { return 0; }
unsigned long IDirect3DDevice9::Release() { return 0; }
#define DEVSTUB(n) long IDirect3DDevice9::n() { return 0; }
DEVSTUB(v03) DEVSTUB(v04) DEVSTUB(v05) DEVSTUB(v06) DEVSTUB(v07) DEVSTUB(v08) DEVSTUB(v09) DEVSTUB(v10) DEVSTUB(v11)
DEVSTUB(v12) DEVSTUB(v13) DEVSTUB(v14) DEVSTUB(v15) DEVSTUB(v17) DEVSTUB(v19) DEVSTUB(v20) DEVSTUB(v21) DEVSTUB(v22)
DEVSTUB(v23) DEVSTUB(v24) DEVSTUB(v25) DEVSTUB(v26) DEVSTUB(v27) DEVSTUB(v28) DEVSTUB(v29) DEVSTUB(v30) DEVSTUB(v31)
DEVSTUB(v32) DEVSTUB(v33) DEVSTUB(v34) DEVSTUB(v35) DEVSTUB(v36) DEVSTUB(v37) DEVSTUB(v38) DEVSTUB(v39)
long IDirect3DDevice9::Reset(_D3DPRESENT_PARAMETERS_* pp) { g_resetPP = pp; return g_resetResult; }
long IDirect3DDevice9::GetBackBuffer(unsigned int sc, unsigned int bb, unsigned long type, IDirect3DSurface9** out)
{ *out = g_back; return (sc == 0 && bb == 0 && type == 0) ? g_bbResult : -98; }
long IDirect3DDevice9::GetDepthStencilSurface(IDirect3DSurface9** out) { *out = g_depth; return g_dsResult; }

void CPixelFormat::Initialise(unsigned long format) { FormatIndex = (long)(format % 64); }
void CSurface::InitialiseFromPreallocatedSurface(IDirect3DSurface9* surface, bool flag)
{ ++g_wrapCalls; PD3DSurface = surface; AllocationSource = (surface == g_back) ? 7 : 9; PAllocatedMemory = flag ? (void*)1 : 0; }
C2DExtentsI CSurface::GetDimensions() const { C2DExtentsI e; e.X = (long)AllocationSource * 100; e.Y = (long)AllocationSource * 10; return e; }
CPixelFormat CSurface::GetPixelFormat() const { CPixelFormat f; f.FormatIndex = (long)AllocationSource + 1; return f; }
CSurface& CSurface::operator=(const CSurface& o) { ++g_assign; PD3DSurface = o.PD3DSurface; AllocationSource = o.AllocationSource; return *this; }
void CSurface::Uninitialise() { ++g_uninit; PD3DSurface = 0; AllocationSource = 0; }
void CTexture::Uninitialise() { ++g_texUninit; }
_D3DDISPLAYMODE CDisplayManager::GetClosestMode(const C2DExtentsI& res, unsigned long depth, bool fullScreen)
{ _D3DDISPLAYMODE m; m.Width = res.X; m.Height = res.Y; m.RefreshRate = 75; m.Format = depth; g_closestFull = fullScreen; return m; }
CPixelFormat CDisplayManager::GetDepthBufferFormat(const _D3DDISPLAYMODE& mode, long depthBits)
{ CPixelFormat f; f.FormatIndex = (long)((depthBits + mode.Format) % 64); return f; }
unsigned long CDisplayManager::CalculateD3DPresentationInterval(const CPresentationIntervalDesc& d) { return 0x100 + d.VSyncCount; }
bool CDisplayManager::SetupDepthAndBackBufferTextures() { return g_setupOk; }
void CDisplayManager::PreDeviceReset() { ++g_pre; }
bool CDisplayManager::PostDeviceReset() { return g_postOk; }

static IDirect3D9 g_d3d;
static IDirect3DDevice9 g_dev;
static CDisplayManager g_dm;
static CCreateDeviceParameters Params(bool exclusive, long ms)
{
    CCreateDeviceParameters p;
    memset(&p, 0, sizeof(p));
    p.BackBufferDimensions.X = 1024; p.BackBufferDimensions.Y = 768; p.BackBufferBitDepth = 32; p.ZBufferBitDepth = 4;
    p.BackBufferCount = 2; p.RefreshRate = 85; p.ExclusiveMode = exclusive; p.PresentationInterval.VSyncCount = 3;
    p.MultisampleType = ms;
    return p;
}
static void Reset()
{
    memset(&g_dm, 0, sizeof(g_dm));
    g_dm.D3D = &g_d3d; g_dm.D3DDevice = &g_dev; g_dm.DeviceType = 1; g_dm.WindowHandle = (void*)0x55;
    g_dm.PresentParams.Flags = 0xdead;
    g_uninit = g_texUninit = g_pre = g_assign = g_msCalls = g_wrapCalls = 0;
    g_msResult = 0; g_resetResult = 0; g_modeResult = 0; g_bbResult = 0; g_dsResult = 0; g_refresh = 60;
    g_setupOk = true; g_postOk = true; g_resetPP = 0;
}

int main()
{
    for (int i = 0; i < 64; ++i) g_PixelFormatTable[i].m_d3dFormat = 1000 + i;
    // exclusive, multisample accepted
    Reset();
    CCreateDeviceParameters p = Params(true, 4);
    Check(g_dm.ResetDevice(p) == CREATE_DEVICE_OK, "exclusive ok");
    Check(g_pre == 1 && g_uninit == 4 && g_texUninit == 1, "released surfaces");
    Check(g_dm.LastDeviceCreationParameters.RefreshRate == 85 && g_dm.LastDeviceCreationParameters.MultisampleType == 4, "params saved");
    Check(g_dm.CurrentMode.Width == 1024 && g_dm.CurrentMode.Height == 768 && g_dm.CurrentMode.Format == 32 && g_closestFull, "closest mode");
    Check(g_msCalls == 1 && g_msDevType == 1 && g_msFormat == 1032 && g_msType == 4 && g_dm.MultisampleType == 4, "multisample check");
    _D3DPRESENT_PARAMETERS_& pp = g_dm.PresentParams;
    Check(g_resetPP == &pp && pp.BackBufferWidth == 1024 && pp.BackBufferHeight == 768 && pp.BackBufferFormat == 32 &&
          pp.BackBufferCount == 2 && pp.MultiSampleType == 4 && pp.MultiSampleQuality == 0 && pp.SwapEffect == 1 &&
          pp.hDeviceWindow == (void*)0x55 && pp.Windowed == 0 && pp.EnableAutoDepthStencil == 1 &&
          pp.AutoDepthStencilFormat == 1000 + 36 && pp.Flags == 0 && pp.FullScreen_RefreshRateInHz == 85 &&
          pp.PresentationInterval == 0x103, "present params exclusive");
    Check(g_dm.NoBackBuffers == 2 && g_dm.CurrentScreenIndex == 0 && g_dm.PresentationInterval.VSyncCount == 3, "counts");
    Check(g_dm.RefreshRate == 60 && g_dm.PresentationIntervalTime == 3.0f / 60, "refresh / interval time");
    Check(g_wrapCalls == 2 && g_dm.BackBufferSurface.PD3DSurface == g_back && g_dm.DepthBufferSurface.PD3DSurface == g_depth, "wrapped");
    Check(g_dm.BackBufferDimensions.X == 700 && g_dm.BackBufferDimensions.Y == 70 && g_dm.BackBufferFormat.FormatIndex == 8 &&
          g_dm.DepthBufferFormat.FormatIndex == 10, "dimensions / formats");
    Check(g_assign == 2 && g_dm.RenderTargetSurface.PD3DSurface == g_back && g_dm.RenderTargetDepthBuffer.PD3DSurface == g_depth &&
          g_dm.RenderTargetDimensions.X == 700 && g_dm.RenderTargetDimensions.Y == 70 && g_dm.ExclusiveMode && !g_dm.DeviceLostFlag, "render target");
    // windowed with multisample request: no check, zero type, no refresh/interval
    Reset();
    p = Params(false, 4);
    Check(g_dm.ResetDevice(p) == CREATE_DEVICE_OK && g_msCalls == 0 && g_dm.MultisampleType == 0 && pp.Windowed == 1 &&
          pp.FullScreen_RefreshRateInHz == 0 && pp.PresentationInterval == 0 && !g_dm.ExclusiveMode && !g_closestFull, "windowed");
    // exclusive, multisample rejected
    Reset(); g_dm.MultisampleType = 9; g_msResult = -5;
    p = Params(true, 2);
    Check(g_dm.ResetDevice(p) == CREATE_DEVICE_OK && g_msCalls == 1 && g_dm.MultisampleType == 0 && pp.MultiSampleType == 0, "ms rejected");
    // no multisample requested
    Reset(); g_dm.MultisampleType = 9;
    p = Params(true, 0);
    Check(g_dm.ResetDevice(p) == CREATE_DEVICE_OK && g_msCalls == 0 && g_dm.MultisampleType == 0, "ms none");
    // device lost / failed
    Reset(); g_resetResult = (long)0x88760868;
    Check(g_dm.ResetDevice(p) == CREATE_DEVICE_LOST && g_dm.DeviceLostFlag && g_wrapCalls == 0, "lost");
    Reset(); g_dm.DeviceLostFlag = true; g_resetResult = -1;
    Check(g_dm.ResetDevice(p) == CREATE_DEVICE_FAILED && !g_dm.DeviceLostFlag, "reset failed");
    // adapter mode failure throws
    Reset(); g_modeResult = -1;
    bool thrown = false;
    try { g_dm.ResetDevice(p); } catch (CD3DFailure&) { thrown = true; }
    Check(thrown && g_wrapCalls == 0, "throws");
    // later failures
    Reset(); g_bbResult = -1;
    Check(g_dm.ResetDevice(p) == CREATE_DEVICE_FAILED && g_wrapCalls == 0, "back buffer fail");
    Reset(); g_dsResult = -1;
    Check(g_dm.ResetDevice(p) == CREATE_DEVICE_FAILED && g_wrapCalls == 1, "depth fail");
    Reset(); g_setupOk = false;
    Check(g_dm.ResetDevice(p) == CREATE_DEVICE_FAILED && g_assign == 2, "setup fail");
    Reset(); g_postOk = false;
    Check(g_dm.ResetDevice(p) == CREATE_DEVICE_FAILED, "post fail");
    printf(g_fail ? "009bfb30_TEST FAIL\n" : "009bfb30_TEST PASS\n");
    return g_fail;
}