// Behaviour test for 0x009C1050 CDisplayManager::SetClientWindow (model copy).
#include <cstdio>

struct C2DExtentsI { int X, Y; C2DExtentsI() : X(0), Y(0) {} };
struct CPresentationIntervalDesc { int Type, Interval; };
struct CCreateDeviceParameters {
    int WinHandle; C2DExtentsI BackBufferDimensions; int BackBufferBitDepth; int ZBufferBitDepth; int BackBufferCount;
    int RefreshRate; bool ExclusiveMode; CPresentationIntervalDesc PresentationInterval; int MultisampleType;
};
struct CDevice { int Released; CDevice() : Released(0) {} void Release() { ++Released; } };
struct CSurface { int Released; CSurface() : Released(0) {} void Release() { ++Released; } };
static int g_Deleted = 0;
struct CRenderManagerBase {
    int Flags; CDevice* Device;
    CRenderManagerBase(int flags) : Flags(flags), Device(0) {}
    virtual ~CRenderManagerBase() { ++g_Deleted; }
    void Init(CDevice* d) { Device = d; }
};
struct CDisplayManager {
    CRenderManagerBase* PRenderManager; CSurface BackBufferSurface, DepthBufferSurface; CDevice* D3DDevice;
    int RenderManagerInitFlags; CCreateDeviceParameters LastDeviceCreationParameters;
    int WinW, WinH, WinDepth, CreateResult, StateInits; CCreateDeviceParameters Seen;
    void GetWindowDimensions(int, C2DExtentsI* d, int* depth) { d->X = WinW; d->Y = WinH; *depth = WinDepth; }
    int CreateDevice(const CCreateDeviceParameters* p) { Seen = *p; return CreateResult; }
    void InitialiseDeviceState() { ++StateInits; }
    bool SetClientWindow(int window)
    {
        BackBufferSurface.Release();
        DepthBufferSurface.Release();
        if (D3DDevice)
            D3DDevice->Release();
        {
            C2DExtentsI dims;
            int bitDepth;
            GetWindowDimensions(window, &dims, &bitDepth);
            int width = dims.X;
            if (width <= 32) width = 32;
            int height = dims.Y;
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
            CRenderManagerBase* manager = new CRenderManagerBase(RenderManagerInitFlags);
            CRenderManagerBase* old = PRenderManager;
            PRenderManager = manager;
            delete old;
            PRenderManager->Init(D3DDevice);
        }
        return true;
    }
};

int main()
{
    CDevice dev; CDisplayManager m;
    m.D3DDevice = &dev; m.RenderManagerInitFlags = 7; m.StateInits = 0;
    m.LastDeviceCreationParameters.ZBufferBitDepth = 24; m.LastDeviceCreationParameters.BackBufferCount = 2;
    m.LastDeviceCreationParameters.RefreshRate = 60; m.LastDeviceCreationParameters.ExclusiveMode = true;
    m.LastDeviceCreationParameters.PresentationInterval.Type = 3; m.LastDeviceCreationParameters.PresentationInterval.Interval = 4;
    m.LastDeviceCreationParameters.MultisampleType = 5;
    m.PRenderManager = new CRenderManagerBase(1);
    m.WinW = 640; m.WinH = 20; m.WinDepth = 32; m.CreateResult = 1;
    bool ok = m.SetClientWindow(99) == false && m.StateInits == 0 && dev.Released == 1 && m.BackBufferSurface.Released == 1 &&
              m.Seen.WinHandle == 99 && m.Seen.BackBufferDimensions.X == 640 && m.Seen.BackBufferDimensions.Y == 32 &&
              m.Seen.BackBufferBitDepth == 32 && m.Seen.ZBufferBitDepth == 24 && m.Seen.BackBufferCount == 2 && m.Seen.RefreshRate == 60 &&
              !m.Seen.ExclusiveMode && m.Seen.PresentationInterval.Type == 3 && m.Seen.PresentationInterval.Interval == 4 &&
              m.Seen.MultisampleType == 5 && g_Deleted == 0;
    m.WinW = 32; m.WinH = 480; m.CreateResult = 2;
    CRenderManagerBase* first = m.PRenderManager;
    ok = ok && m.SetClientWindow(5) && m.StateInits == 1 && m.Seen.BackBufferDimensions.X == 32 && m.Seen.BackBufferDimensions.Y == 480 &&
         g_Deleted == 1 && m.PRenderManager != 0 && m.PRenderManager->Device == &dev && m.PRenderManager->Flags == 7 && dev.Released == 2;
    (void)first;
    delete m.PRenderManager; m.PRenderManager = 0; m.D3DDevice = 0;
    ok = ok && m.SetClientWindow(6) && m.StateInits == 2 && m.PRenderManager == 0;
    if (!ok) { std::printf("009c1050_TEST FAIL\n"); return 1; }
    std::printf("009c1050_TEST PASS\n");
    return 0;
}
