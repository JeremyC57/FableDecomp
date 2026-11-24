// CDisplayManager::ResetDevice(const CCreateDeviceParameters&) -- remember the parameters, release the device-dependent surfaces and the back-buffer copy texture, re-pick the display mode, refill the present parameters (multisample validated against the device, interval only when exclusive), reset the device (device-lost reports 1 and sets the lost flag), re-read the adapter refresh rate and re-wrap the back / depth buffers, then rebuild the copy textures and run the post-reset callbacks
// Leaf: ?ResetDevice@CDisplayManager@@AAE?AW4ECreateDeviceSuccess@1@ABUCCreateDeviceParameters@1@@Z
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
#include "engine/IDirect3D9.h"   // IDirect3D9 (slots 8 / 11); engine/Direct3D9Types.h: _D3DFORMAT, _D3DDISPLAYMODE, _D3DPRESENT_PARAMETERS_ (RP1)
#include <string.h>






















struct IDirect3DSurface9;
struct IDirect3DDevice9;





















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
    void Initialise(_D3DFORMAT format);         // 0x009E3830
    unsigned long GetD3DFormat() const { return g_PixelFormatTable[FormatIndex].m_d3dFormat; }
};
struct CPresentationIntervalDesc {
    bool PresentImmediate;
    bool PresentImmediateIfSyncMissed;
    long VSyncCount;
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
#include "engine/CVsyncCallbackTimer.h"   // CVsyncCallbackTimer (RP1; was a row-local model)
struct CD3DFailure { CD3DFailure() {} };

class CDisplayManager { public:
    // NF140 (q4056): Ego_r.pdb / FableWin.pdb ?ResetDevice@CDisplayManager@@AAE?AW4ECreateDeviceSuccess@1@ABUCCreateDeviceParameters@1@@Z:
    // the result enum and the parameter block are nested in CDisplayManager (enumerators as Ego_d.pdb LF_ENUM / engine/CDisplayManager.h).
    enum ECreateDeviceSuccess { CREATE_DEVICE_FAILED = 0, CREATE_DEVICE_DEVICE_LOST = 1, CREATE_DEVICE_SUCCESS = 2 };
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
    D3DDEVTYPE        DeviceType;                  // +0x5c D3DDeviceCaps.DeviceType
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
    HWND              WindowHandle;                // +0x1b4
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
private:
    void PreDeviceReset();                         // 0x009BF500
    bool PostDeviceReset();                        // 0x009BF550
    ECreateDeviceSuccess ResetDevice(const CCreateDeviceParameters& params);
};

CDisplayManager::ECreateDeviceSuccess CDisplayManager::ResetDevice(const CCreateDeviceParameters& params)
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
        if (D3D->CheckDeviceMultiSampleType(0, DeviceType, (_D3DFORMAT)format.GetD3DFormat(), 0, (D3DMULTISAMPLE_TYPE)multisample, 0) == 0)
            MultisampleType = params.MultisampleType;
        else
            MultisampleType = 0;
    } else
        MultisampleType = 0;
    PresentParams.MultiSampleType = (D3DMULTISAMPLE_TYPE)MultisampleType;
    PresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
    PresentParams.EnableAutoDepthStencil = 1;
    PresentParams.AutoDepthStencilFormat = (_D3DFORMAT)GetDepthBufferFormat(CurrentMode, params.ZBufferBitDepth).GetD3DFormat();
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
            return CREATE_DEVICE_DEVICE_LOST;
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
    return PostDeviceReset() ? CREATE_DEVICE_SUCCESS : CREATE_DEVICE_FAILED;
}