// CDisplayManager::Init(const CDisplayManagerInit& init, bool windowed) -- set up the display: record the 2D/shader/window options, create
// Direct3D 9, replace the dynamic-texture list, read the HAL caps (falling back to the reference device), create the device with the
// requested size clamped to at least 32x32 (throwing when it does not succeed), resize the window to the back buffer and, with 2D enabled,
// create the render manager (plus the shader render manager) before initialising the virtual pixels.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C0E50, length 500 bytes, `ret 8`.
// Callees: Direct3DCreate9 0x00BFEFB0 (static stub); operator new 0x00BFEA1A / delete 0x00BFE9BC; dynamic-texture list ctor 0x009F70E0 /
// dtor 0x009F7330 (manifest "Init_ListHeads" / "CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures" are propagated); IDirect3D9 slot
// 14 (+0x38) GetDeviceCaps; CDisplayManager::CreateDevice 0x009BF7E0; _CxxThrowException 0x00BFEB84 (ThrowInfo 0x013692F8);
// ResizeWindowToBackBufferSize 0x009BE490; CWideString::operator= 0x0099B7D0; CRenderManager2D ctor 0x009D9A30 (derived vtable
// 0x0129B400); CRenderManagerCore::Init 0x00A0A940; CShaderRenderManager::Initialise 0x0098ACF0 on 0x013BC470; InitVirtualPixels 0x009BE980.
// Layout (descriptive, retail offsets): render manager +0x8, IDirect3D9 +0x54, device +0x58, caps +0x5c, windowed +0x1a0, +0x1a4, 2D +0x1b0,
// shaders +0x1b1, window +0x1b4, +0x214, +0x255, font path +0x258, render flags +0x260, dynamic textures +0x294; CDisplayManagerInit and
// CCreateDeviceParameters fields as commented (names descriptive).
// Shape: the two owned pointers are auto pointers whose operator= deletes the old object after storing the new one (the new-expression runs
// first); width, height AND bit depth are read into locals before the clamps (loading only width/height reorders the loads); the device
// parameters come from an inline member-init ctor; the exception is an empty class with a user ctor (no zero store; the byte copy reads the
// dead bool slot); the render-manager base is polymorphic so the derived ctor overwrites the vptr and the object is 0x3ea4 bytes.
// Leaf: Init
class CWideString { public:
    void* m_pData;
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
struct IDirect3DDevice9;
struct D3DCAPS9 { unsigned char _body[0x130]; };
struct IDirect3D9 {
    virtual long __stdcall v00(); virtual long __stdcall v01(); virtual long __stdcall v02(); virtual long __stdcall v03();
    virtual long __stdcall v04(); virtual long __stdcall v05(); virtual long __stdcall v06(); virtual long __stdcall v07();
    virtual long __stdcall v08(); virtual long __stdcall v09(); virtual long __stdcall v10(); virtual long __stdcall v11();
    virtual long __stdcall v12(); virtual long __stdcall v13();
    virtual long __stdcall GetDeviceCaps(unsigned int adapter, long deviceType, D3DCAPS9* caps);   // slot 14 (+0x38)
};
extern "C" IDirect3D9* __stdcall Direct3DCreate9(unsigned int sdkVersion);   // 0x00BFEFB0
class CDisplayManager;
struct CRenderManagerCore {
    virtual ~CRenderManagerCore();
    void Init(IDirect3DDevice9* device);               // 0x00A0A940
};
struct CRenderManager2D : CRenderManagerCore {
    unsigned char _body[0x3ea0];
    CRenderManager2D(CDisplayManager* displayManager, const CWideString& fontPath, unsigned long b);   // 0x009D9A30
};
class CRenderManager : public CRenderManager2D { public:             // vtable 0x0129B400
    CRenderManager(CDisplayManager* displayManager, const CWideString& fontPath, unsigned long b)
        : CRenderManager2D(displayManager, fontPath, b) {}
    virtual ~CRenderManager();
};
class CShaderRenderManager { public:
    void Initialise(CRenderManager* renderManager, IDirect3DDevice9* device);   // 0x0098ACF0
};
extern CShaderRenderManager g_ShaderRenderManager;    // 0x013BC470
struct CDynamicTextureList {                           // 0x14 bytes
    unsigned char _body[0x14];
    CDynamicTextureList();                             // 0x009F70E0
    ~CDynamicTextureList();                            // 0x009F7330
};
template <class T> struct CAutoPtr {
    T* m_p;
    void operator=(T* p)
    {
        T* old = m_p;
        m_p = p;
        delete old;
    }
    T* operator->() const { return m_p; }
    operator T*() const { return m_p; }
};
struct CCreateDeviceParameters {
    unsigned long Window;                              // +0x00
    long Width;                                        // +0x04
    long Height;                                       // +0x08
    long BitDepth;                                     // +0x0c
    unsigned long A;                                   // +0x10
    unsigned long B;                                   // +0x14
    long RefreshRate;                                  // +0x18
    bool Windowed;                                     // +0x1c
    unsigned char _pad1d[3];
    bool C;                                            // +0x20
    bool D;                                            // +0x21
    unsigned char _pad22[2];
    long F;                                            // +0x24
    long G;                                            // +0x28
    CCreateDeviceParameters(unsigned long window, long width, long height, long bitDepth, bool a, bool b, long refreshRate, bool windowed,
                            bool c, bool d, long f, long g)
        : Window(window), Width(width), Height(height), BitDepth(bitDepth), A(a), B(b), RefreshRate(refreshRate), Windowed(windowed),
          C(c), D(d), F(f), G(g) {}
};
class CDisplayManagerInit { public:
    long Width;                                        // +0x00
    long Height;                                       // +0x04
    long BitDepth;                                     // +0x08
    long RefreshRate;                                  // +0x0c
    bool Use2D;                                        // +0x10
    bool UseShaders;                                   // +0x11
    unsigned char _pad12[2];
    unsigned long Window;                              // +0x14
    bool A;                                            // +0x18
    bool B;                                            // +0x19
    unsigned char _pad1a[2];
    long G;                                            // +0x1c
    unsigned char _pad20[8];
    bool C;                                            // +0x28
    bool D;                                            // +0x29
    unsigned char _pad2a[2];
    long F;                                            // +0x2c
    unsigned char _pad30[4];
    unsigned long RenderFlags;                         // +0x34
    CWideString FontPath;                              // +0x38
};
struct CCreateDeviceFailed { CCreateDeviceFailed() {} };
enum ECreateDeviceSuccess { CREATE_DEVICE_FAILED, CREATE_DEVICE_RETRY, CREATE_DEVICE_OK };
class CDisplayManager { public:
    unsigned char _pad000[8];
    CAutoPtr<CRenderManager> PRenderManager;           // +0x008
    unsigned char _pad00c[0x54 - 0xc];
    IDirect3D9* PD3D;                                  // +0x054
    IDirect3DDevice9* PDevice;                         // +0x058
    D3DCAPS9 Caps;                                     // +0x05c
    unsigned char _pad18c[0x1a0 - 0x18c];
    bool Windowed;                                     // +0x1a0
    unsigned char _pad1a1[3];
    long FrameCount;                                   // +0x1a4
    unsigned char _pad1a8[8];
    bool Use2D;                                        // +0x1b0
    bool UseShaders;                                   // +0x1b1
    unsigned char _pad1b2[2];
    unsigned long Window;                              // +0x1b4
    unsigned char _pad1b8[0x214 - 0x1b8];
    bool Flag214;                                      // +0x214
    unsigned char _pad215[0x255 - 0x215];
    bool Flag255;                                      // +0x255
    unsigned char _pad256[2];
    CWideString FontPath;                              // +0x258
    unsigned char _pad25c[4];
    unsigned long RenderFlags;                         // +0x260
    unsigned char _pad264[0x294 - 0x264];
    CAutoPtr<CDynamicTextureList> PDynamicTextures;    // +0x294
    ECreateDeviceSuccess CreateDevice(const CCreateDeviceParameters& params);   // 0x009BF7E0
    void ResizeWindowToBackBufferSize();               // 0x009BE490
    void InitVirtualPixels();                          // 0x009BE980
    void Init(const CDisplayManagerInit& init, bool windowed);
};

void CDisplayManager::Init(const CDisplayManagerInit& init, bool windowed)
{
    Flag255 = false;
    Windowed = windowed;
    Use2D = init.Use2D;
    UseShaders = init.UseShaders && init.Use2D;
    FrameCount = 0;
    Flag214 = false;
    Window = init.Window;
    PD3D = Direct3DCreate9(0x20);
    PDynamicTextures = new CDynamicTextureList;
    if (PD3D->GetDeviceCaps(0, 1, &Caps) < 0)
        PD3D->GetDeviceCaps(0, 2, &Caps);
    long width = init.Width;
    long height = init.Height;
    long bitDepth = init.BitDepth;
    if (width < 32)
        width = 32;
    if (height < 32)
        height = 32;
    CCreateDeviceParameters params(Window, width, height, bitDepth, init.A, init.B, init.RefreshRate, windowed, init.C, init.D,
                                   init.F, init.G);
    if (CreateDevice(params) != CREATE_DEVICE_OK)
        throw CCreateDeviceFailed();
    ResizeWindowToBackBufferSize();
    RenderFlags = init.RenderFlags;
    if (Use2D) {
        FontPath = init.FontPath;
        PRenderManager = new CRenderManager(this, FontPath, init.RenderFlags);
        PRenderManager->Init(PDevice);
        if (UseShaders)
            g_ShaderRenderManager.Initialise(PRenderManager, PDevice);
    }
    InitVirtualPixels();
}