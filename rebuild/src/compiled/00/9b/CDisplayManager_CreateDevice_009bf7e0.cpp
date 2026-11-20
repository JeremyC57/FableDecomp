// CDisplayManager::CreateDevice(const CCreateDeviceParameters& parameters) -- create the D3D device for the requested mode and wrap its back and depth buffers
// Leaf: ?CreateDevice@CDisplayManager@@AAE?AW4ECreateDeviceSuccess@1@ABUCCreateDeviceParameters@1@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BF7E0, 846 bytes, `ret 4`, no frame pointer (`sub esp, 18h`), three exits (0 at 0x009BFAEE
//   shared by the four failures, 2 at 0x009BFB22). /EHsc because the expanded ReadDeviceRefreshRate throws (0x009BF9FC
//   _CxxThrowException with ThrowInfo 0x013692F8); the function itself has no unwind state and no EH frame.
// Identity: Ego_d.pdb ?CreateDevice@CDisplayManager@@AAE?AW4ECreateDeviceSuccess@1@ABUCCreateDeviceParameters@1@@Z (debug length
//   1688) + the two retail call sites 0x009C0F84 (the landed CDisplayManager::Init 0x009C0E50, result compared with 2) and
//   0x009C1112. Parameters and locals exactly as Ego_d.pdb:
//   parameters; create_flags (unsigned long), hr (HRESULT: Ego_d type 0x0008 T_HRESULT), pdepthbuffer_surface, pbackbuffer_surface. Result enum
//   CDisplayManager::ECreateDeviceSuccess from Ego_d.pdb (LF_ENUM: CREATE_DEVICE_FAILED 0, CREATE_DEVICE_DEVICE_LOST 1,
//   CREATE_DEVICE_SUCCESS 2).
// Shape: Ego_d.pdb's C13 line table (ref/009bf7e0/egod_lines.tsv, source lines 202..392; Ego_d is a VC10 /Od build, so a virtual
//   call is 2 bytes longer than our VC7.1 /Od line): 203 the parameter copy; 205 GetClosestMode (65 = our 64 + VC10's movzx); 207..210 WindowHandle /
//   NoBackBuffers / ExclusiveMode / CurrentScreenIndex; 212 PresentationInterval; 214 memset; 216..218 width / height / format;
//   219 Windowed (23 = our 21 + VC10's `add 1` for `inc`); 220 / 221 SwapEffect / EnableAutoDepthStencil; 222 the depth format through GetAsD3DPixelFormat (45 = ours);
//   223 / 224 hDeviceWindow / BackBufferCount; 226 `if (ExclusiveMode)` (14); 228 `if (parameters.RefreshRate == 0)` (9 = ours);
//   230 the default rate (13 = ours); 232 else (2); 234 the requested rate (15 = ours); 240 / 250 CalculateD3DPresentationInterval
//   in BOTH branches (26 each = ours); 245 the windowed rate 0 (13); 265 create_flags (7); 266 the caps test (13); 268 / 272 the two
//   `|=` (9 each), 270 else; 275 the three-part multisample test, one line of 62 bytes (= ours, either polarity); 279 / 281 two
//   13-byte constant stores then 283 else and 285 the 15-byte store from the parameter: the constant store is the THEN branch, so the
//   test is written as the rejecting `== NONE || Windowed || !IsAntiAliasingModeValid(..)` form (retail has one store per branch:
//   0x009BF999 the constant / 0x009BF991 the parameter; a second constant store in the then branch, as Ego_d's line 281 has, costs 6
//   bytes, 852v846 -- measured for PresentParams.MultiSampleType and MultiSampleQuality; only a repeat of the same store folds away --
//   so that line is debug-only or newer than retail); 289 the D3D multisample type (18 = ours); 296 IDirect3D9::CreateDevice (68 = our 65 +
//   virtual call + 1); 298 / 330 / 347 the failure tests (17 with the debug-only log lines 305 / 336 / 353 of 147 bytes; retail is
//   `test eax, eax ; jl` / `jge`), 306 / 337 / 354 the returns (7 = ours); 310 `RefreshRate = ReadDeviceRefreshRate();` (17 = ours);
//   311 CalcPresentationIntervalTime (37 = ours); 313 VsyncCallbackTimer->SetDevice (28 = ours); 327 GetBackBuffer (33 = 31 + 2);
//   341..343 the back buffer wrap, its dimensions and format (17 / 35 / 26 = ours); 346 GetDepthStencilSurface (30 = 28 + 2);
//   358 / 359 the depth wrap and format (17 / 26 = ours); 380..382 the three render-target assignments; 384 the texture setup test;
//   389 SetViewport; 391 the success return.
// The refresh-rate test is load-bearing in retail although c2 folds it away (both arms store the same value, so only one
//   `mov eax, [ebx+18h] ; mov [ebp+24Ch], eax` remains at 0x009BF905): with the plain store the load is a one-use expression
//   temporary that advances c2's eax / ecx / edx rotation, and the multisample test's format argument at 0x009BF94C then comes out
//   in ecx (846v846, one register); written with the test, parameters.RefreshRate has two uses and takes no rotation slot.
// Same-TU functions expanded by retail: ReadDeviceRefreshRate (0x009BF9D7..0x009BFA01), CalcPresentationIntervalTime (0x009BFA01
//   `fild` / 0x009BFA13 `fidiv`) and IsAntiAliasingModeValid (0x009BF95C..0x009BF98C) are functions of the display manager's own
//   .cpp: Ego_d.pdb gives all three the file id of CreateDevice (600; lines 847..858, 862..865, 956..959, i.e. after CreateDevice),
//   and retail keeps their out-of-line copies at 0x009BE340, 0x009BE380 and 0x009BE3C0. They are defined below in that order as
//   same-TU copies (GOTCHAS 455): this row's object reproduces the three retail copies byte for byte (54 / 11 / 53 bytes, relocation
//   fields masked), and the texts are the owners' texts (0x009BE340, 0x009BE380; 0x009BE3C0 is not catalogued yet).
//   Ego_d line sizes: 850 the GetAdapterDisplayMode call (29 = our 27 + the VC10 COM reload), 857 `return mode.RefreshRate` (3 =
//   ours), 864 `(float)vsync_count / refresh_rate` (12, ours 6: VC10 adds a float store / reload; casting both operands is
//   the same 6 bytes and also matches, the text is the landed owner's single cast), 958 the one-line CheckDeviceMultiSampleType comparison (53, ours 49).
// Inferred: `throw CGenericException();` in ReadDeviceRefreshRate. Ego_d line 854 is 32 bytes, the size of the five
//   `throw CWaveFileException("...")` lines of CWaveLoader::LoadWaveFile in the same PDB (our /Od: 27 without an argument, 32 with a
//   string), so the original passed a message. Its text is in no source (release CExceptionBase discards it: retail copies a 1-byte
//   temporary with no constructor call and no string, 0x009BF9EA / 0x009BE357), so the row throws through the class's other
//   constructor (Ego_d method list 0xCA77: `(const char*)` and `()`); the retail bytes are the same.
// Header inlines expanded by retail: CVsyncCallbackTimer::SetDevice (the store at 0x009BFA23; Ego_d file id 240 line 25, NEW header),
//   CPixelFormat::GetAsD3DPixelFormat (Ego_r.pdb / FableWin.pdb ?GetAsD3DPixelFormat@CPixelFormat@@QBE?AW4_D3DFORMAT@@XZ, Ego_d file
//   id 648 line 92; the GD3DFormats table read at 0x009BF8DC / 0x009BF96B; existing opt-in FABLE_RETAIL_CPIXELFORMAT_SIGNED), the
//   implicit CCreateDeviceParameters / D3DDISPLAYMODE / C2DExtentsI / CPixelFormat copies, the C2DBoxI(long, long, long, long)
//   constructor, boost::scoped_ptr operator->.
// Callees: 0x009BF600 CDisplayManager::GetClosestMode, 0x009BEBB0 CDisplayManager::GetClosestDepthBufferFormat, 0x009BE2D0
//   CDisplayManager::CalculateD3DPresentationInterval (twice), 0x009E3830 CPixelFormat::Initialise(_D3DFORMAT) (through the inline CPixelFormat(_D3DFORMAT) constructor), 0x00BFEB84
//   _CxxThrowException, 0x009F2EC0 CSurface::InitialiseFromPreallocatedSurface (twice), 0x009F2F90 CSurface::GetDimensions,
//   0x009F2F60 CSurface::GetPixelFormat (twice), 0x009F2E70 CSurface::operator= (twice), 0x009BEE40
//   CDisplayManager::SetupDepthAndBackBufferTextures, 0x009BEF80 CDisplayManager::SetViewport; virtual IDirect3D9 slots 11
//   CheckDeviceMultiSampleType (+2Ch), 16 CreateDevice (+40h), 8 GetAdapterDisplayMode (+20h); IDirect3DDevice9 slots 18
//   GetBackBuffer (+48h), 40 GetDepthStencilSurface (+A0h).
// Globals: GD3DFormats (the pixel-format table, two reads), the CGenericException ThrowInfo 0x013692F8 (RTTI read from the image:
//   .?AVCGenericException@@ then .?AVCExceptionBase@@).
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_SETVIEWPORT
#define FABLE_RETAIL_CDISPLAYMANAGER_CREATEDEVICE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_SIGNED
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_PREALLOCATED
#define FABLE_RETAIL_CSURFACE_ASSIGN
#define FABLE_RETAIL_CSURFACE_BITMAP
#define FABLE_RETAIL_CSURFACE_PIXELFORMAT
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTEXTURE
#include <string.h>
#include "engine/C2DExtentsI.h"
#include "engine/CPixelFormat.h"
#include "engine/CDisplayManager.h"
#include "engine/IDirect3D9.h"
#include "engine/IDirect3DDevice9.h"
#include "engine/CGenericException.h"
#include "engine/CVsyncCallbackTimer.h"

CDisplayManager::ECreateDeviceSuccess CDisplayManager::CreateDevice(const CCreateDeviceParameters& parameters)
{
    LastDeviceCreationParameters = parameters;

    CurrentMode = GetClosestMode(parameters.BackBufferDimensions, parameters.BackBufferBitDepth, parameters.ExclusiveMode);

    WindowHandle = parameters.WinHandle;
    NoBackBuffers = parameters.BackBufferCount;
    ExclusiveMode = parameters.ExclusiveMode;
    CurrentScreenIndex = 0;

    PresentationInterval = parameters.PresentationInterval;

    memset(&PresentParams, 0, sizeof(PresentParams));

    PresentParams.BackBufferWidth = CurrentMode.Width;
    PresentParams.BackBufferHeight = CurrentMode.Height;
    PresentParams.BackBufferFormat = CurrentMode.Format;
    PresentParams.Windowed = !parameters.ExclusiveMode;
    PresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
    PresentParams.EnableAutoDepthStencil = 1;
    PresentParams.AutoDepthStencilFormat = GetClosestDepthBufferFormat(CurrentMode, parameters.ZBufferBitDepth).GetAsD3DPixelFormat();
    PresentParams.hDeviceWindow = WindowHandle;
    PresentParams.BackBufferCount = parameters.BackBufferCount;

    if (ExclusiveMode)
    {
        if (parameters.RefreshRate == 0)
        {
            PresentParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
        }
        else
        {
            PresentParams.FullScreen_RefreshRateInHz = parameters.RefreshRate;
        }
        PresentParams.PresentationInterval = CalculateD3DPresentationInterval(PresentationInterval);
    }
    else
    {
        PresentParams.FullScreen_RefreshRateInHz = 0;
        PresentParams.PresentationInterval = CalculateD3DPresentationInterval(PresentationInterval);
    }

    unsigned long create_flags = D3DCREATE_FPU_PRESERVE | D3DCREATE_MULTITHREADED;
    if (D3DDeviceCaps.DevCaps & D3DDEVCAPS_PUREDEVICE)
    {
        create_flags |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
    }
    else
    {
        create_flags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

    if (parameters.MultisampleType == SURFACE_MULTISAMPLE_NONE || PresentParams.Windowed || !IsAntiAliasingModeValid(parameters.MultisampleType, CPixelFormat(PresentParams.BackBufferFormat)))
    {
        MultisampleType = SURFACE_MULTISAMPLE_NONE;
    }
    else
    {
        MultisampleType = parameters.MultisampleType;
    }
    PresentParams.MultiSampleType = (D3DMULTISAMPLE_TYPE)MultisampleType;

    HRESULT hr = D3D->CreateDevice(D3DDeviceCaps.AdapterOrdinal, D3DDeviceCaps.DeviceType, WindowHandle, create_flags, &PresentParams, &D3DDevice);
    if (hr < 0)
    {
        return CREATE_DEVICE_FAILED;
    }

    RefreshRate = ReadDeviceRefreshRate();
    PresentationIntervalTime = CalcPresentationIntervalTime(RefreshRate, PresentationInterval.VSyncCount);
    VsyncCallbackTimer->SetDevice(D3DDevice);

    IDirect3DSurface9* pbackbuffer_surface;
    hr = D3DDevice->GetBackBuffer(0, 0, 0, &pbackbuffer_surface);
    if (hr < 0)
    {
        return CREATE_DEVICE_FAILED;
    }
    BackBufferSurface.InitialiseFromPreallocatedSurface(pbackbuffer_surface, false);
    BackBufferDimensions = BackBufferSurface.GetDimensions();
    BackBufferFormat = BackBufferSurface.GetPixelFormat();

    IDirect3DSurface9* pdepthbuffer_surface;
    hr = D3DDevice->GetDepthStencilSurface(&pdepthbuffer_surface);
    if (hr < 0)
    {
        return CREATE_DEVICE_FAILED;
    }
    DepthBufferSurface.InitialiseFromPreallocatedSurface(pdepthbuffer_surface, false);
    DepthBufferFormat = DepthBufferSurface.GetPixelFormat();

    RenderTargetSurface = BackBufferSurface;
    RenderTargetDepthBuffer = DepthBufferSurface;
    RenderTargetDimensions = BackBufferDimensions;

    if (!SetupDepthAndBackBufferTextures())
    {
        return CREATE_DEVICE_FAILED;
    }

    SetViewport(C2DBoxI(0, 0, RenderTargetDimensions.X, RenderTargetDimensions.Y));

    return CREATE_DEVICE_SUCCESS;
}

// Fidelity: same-TU strong definition of 0x009BE340 (retail TU-mate)
long CDisplayManager::ReadDeviceRefreshRate()
{
    D3DDISPLAYMODE mode;
    HRESULT hr = D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
    if (hr < 0)
    {
        throw CGenericException();
    }

    return mode.RefreshRate;
}

// Fidelity: same-TU strong definition of 0x009BE380 (retail TU-mate)
float CDisplayManager::CalcPresentationIntervalTime(long refresh_rate, long vsync_count)
{
    return (float)vsync_count / refresh_rate;
}

// Fidelity: same-TU strong definition of 0x009BE3C0 (retail TU-mate)
bool CDisplayManager::IsAntiAliasingModeValid(ESurfaceMultisampleType multisampling_mode, const CPixelFormat& pixel_format)
{
    return D3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDeviceCaps.DeviceType, pixel_format.GetAsD3DPixelFormat(), 0, (D3DMULTISAMPLE_TYPE)multisampling_mode, 0) == 0;
}