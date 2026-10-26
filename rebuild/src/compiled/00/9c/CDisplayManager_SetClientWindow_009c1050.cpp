// CDisplayManager::SetClientWindow(HWND window) -- re-target the display at a new client window: drop the back/depth buffer surfaces, release the device, recreate it windowed for the window's client size (at least 32x32) reusing the last creation parameters, then re-initialise the device state and replace the render manager (when one exists) with a fresh one bound to the new device
// Leaf: SetClientWindow
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C1050, length 317 bytes (`ret 4`). Callees: CSurface release 0x9F2E20, GetWindowDimensions 0x9BED10, CreateDevice 0x9BF7E0, 0x9BE490, operator new 0xBFEA1A (0x3EA4 bytes), render-manager base ctor 0x9D9A30 + vtable 0x0129B400, Init 0xA0A940.
// Layout: PDB CDisplayManager, except LastDeviceCreationParameters sits at +0x268 in retail (Ego_r.pdb says +0x270): the STLport list DeviceResetCallbacks
// is one node pointer (4 bytes) where the PDB build's list is 12. The skipped +0x284 is ExclusiveMode, stored false.
// Note: the block scope lets bitDepth take the dead argument slot; the store order of params is the retail order.
struct HWND__; typedef HWND__* HWND;
struct IDirect3DDevice9 { virtual long __stdcall QueryInterface(void*, void**); virtual unsigned long __stdcall AddRef(); virtual unsigned long __stdcall Release(); };
class C2DExtentsI { public: long X, Y; C2DExtentsI() : X(0), Y(0) { } };
struct CPresentationIntervalDesc { long Type, Interval; };
struct CCreateDeviceParameters {
    HWND WinHandle; C2DExtentsI BackBufferDimensions; long BackBufferBitDepth; long ZBufferBitDepth; long BackBufferCount;
    long RefreshRate; bool ExclusiveMode; CPresentationIntervalDesc PresentationInterval; long MultisampleType;
};
class CWideString { public: void* PData; };
class CSurface { public: void Release(); unsigned char _d[0x10]; };
class CDisplayManager;
struct CRenderManagerBase {
    CRenderManagerBase(CDisplayManager* display, const CWideString& shaderPath, unsigned long flags);
    virtual ~CRenderManagerBase();
    void Init(IDirect3DDevice9* device);
};
class CRenderManager : public CRenderManagerBase { public:
    CRenderManager(CDisplayManager* display, const CWideString& shaderPath, unsigned long flags) : CRenderManagerBase(display, shaderPath, flags) { }
    virtual ~CRenderManager();
    unsigned char _d[0x3ea4 - 4];
};
class CDisplayManager {
public:
    bool SetClientWindow(HWND window);
    void GetWindowDimensions(HWND window, C2DExtentsI* dims, long* bitDepth);
    long CreateDevice(const CCreateDeviceParameters* params);
    void InitialiseDeviceState();
    long _pad00;
    void* SystemManager;                                  // +0x04
    CRenderManagerBase* PRenderManager;                   // +0x08
    CSurface RenderTargetSurface, RenderTargetDepthBuffer;// +0x0c, +0x1c
    CSurface BackBufferSurface, DepthBufferSurface;       // +0x2c, +0x3c
    unsigned char _pad4c[0x58 - 0x4c];
    IDirect3DDevice9* D3DDevice;                          // +0x58
    unsigned char _pad5c[0x258 - 0x5c];
    CWideString ShaderResourcePath;                       // +0x258
    long FrameCounter;                                    // +0x25c
    unsigned long RenderManagerInitFlags;                 // +0x260
    void* DeviceResetCallbacks;                           // +0x264 (STLport list: one node pointer)
    CCreateDeviceParameters LastDeviceCreationParameters; // +0x268
};
bool CDisplayManager::SetClientWindow(HWND window)
{
    BackBufferSurface.Release();
    DepthBufferSurface.Release();
    if (D3DDevice)
        D3DDevice->Release();
    {
        C2DExtentsI dims;
        long bitDepth;
        GetWindowDimensions(window, &dims, &bitDepth);
        long width = dims.X;
        if (width <= 32) width = 32;
        long height = dims.Y;
        if (height <= 32) height = 32;
        CCreateDeviceParameters params;
        params.ZBufferBitDepth = LastDeviceCreationParameters.ZBufferBitDepth;
        params.BackBufferCount = LastDeviceCreationParameters.BackBufferCount;
        params.RefreshRate = LastDeviceCreationParameters.RefreshRate;
        params.PresentationInterval = LastDeviceCreationParameters.PresentationInterval;
        params.MultisampleType = LastDeviceCreationParameters.MultisampleType;
        params.BackBufferDimensions.Y = height;
        params.BackBufferDimensions.X = width;
        params.WinHandle = window;
        params.BackBufferBitDepth = bitDepth;
        params.ExclusiveMode = false;
        if (CreateDevice(&params) != 2)
            return false;
    }
    InitialiseDeviceState();
    if (PRenderManager) {
        CRenderManagerBase* manager = new CRenderManager(this, ShaderResourcePath, RenderManagerInitFlags);
        CRenderManagerBase* old = PRenderManager;
        PRenderManager = manager;
        delete old;
        PRenderManager->Init(D3DDevice);
    }
    return true;
}