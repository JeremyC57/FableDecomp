// CDisplayManagerInit::CDisplayManagerInit() -- default display set-up: no mode (0x0x0 @ 0 Hz), render managers on, 16-bit Z with one back buffer, 512K push buffer / 32K kick-off, immediate presentation with a one-VSync interval
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the five `true`/1 members assigned FIRST in the body, then the zero group, then ZBufferDepth, NoBackBuffers, the two sizes: both constants are materialised at the top (`xor eax,eax ; mov ecx,1`) and the +0x18 immediate stays before the +0x19 store
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004016F0, length 78 bytes. Callees: CWideString::CWideString() @ 0x0099AED0.
// Called from the CSystemManagerInit constructor (0x00403B10, call at 0x00403B6C on `esi+5Ch`).
// Label correction: the manifest calls this a CRegionDef constructor (propagated). Every store lines up with Ego_r.pdb's
// CDisplayManagerInit (60 bytes): Width/Height/Depth/RefreshRate +0x00..+0x0c, UseRenderManager +0x10,
// UseShaderRenderManager +0x11, WindowHandle +0x14, ZBufferDepth +0x18, NoBackBuffers +0x19, MultisampleType +0x1c,
// PushBufferSize +0x20, KickOffSize +0x24, PresentImmediate +0x28, PresentImmediatelyIfMissedVSync +0x29,
// PresentationIntevalVSyncCount +0x2c, SkipConfigDetection +0x30 (left alone), RenderManagerInitFlags +0x34,
// CWideString ShaderResourcePath +0x38 (the constructor call). No engine header exists for the class.
// Shape: the string member is constructed first, so every scalar is a BODY assignment (initialisers would precede the
// call); VC7.1 groups the stores by value (eax = 0, ecx = 1, then the immediates) and keeps the source order inside a
// group -- retail's ones group runs +0x10, +0x11, +0x2c, +0x29, +0x28, so the three Present* members are assigned in
// reverse. Tried: member order (ones group first with eax = 1, zeros in ecx), member order with the Present* trio
// reversed (same group swap), and the retail store order (this file: only the register reuse and +0x19 differ).
// Leaf: ??0CDisplayManagerInit
struct HWND__;

enum ESurfaceMultisampleType { SURFACE_MULTISAMPLE_NONE = 0 };

class CWideString { public:
    void* m_pStringData;
    CWideString();                                      // 0x0099AED0
};

class CDisplayManagerInit { public:
    long                    Width;                              // +0x00
    long                    Height;                             // +0x04
    long                    Depth;                              // +0x08
    long                    RefreshRate;                        // +0x0c
    bool                    UseRenderManager;                   // +0x10
    bool                    UseShaderRenderManager;             // +0x11
    HWND__*                 WindowHandle;                       // +0x14
    unsigned char           ZBufferDepth;                       // +0x18
    unsigned char           NoBackBuffers;                      // +0x19
    ESurfaceMultisampleType MultisampleType;                    // +0x1c
    long                    PushBufferSize;                     // +0x20
    long                    KickOffSize;                        // +0x24
    bool                    PresentImmediate;                   // +0x28
    bool                    PresentImmediatelyIfMissedVSync;    // +0x29
    long                    PresentationIntevalVSyncCount;      // +0x2c
    bool                    SkipConfigDetection;                // +0x30
    unsigned long           RenderManagerInitFlags;             // +0x34
    CWideString             ShaderResourcePath;                 // +0x38
    CDisplayManagerInit();
};

CDisplayManagerInit::CDisplayManagerInit()
{
    UseRenderManager = true;
    UseShaderRenderManager = true;
    PresentationIntevalVSyncCount = 1;
    PresentImmediatelyIfMissedVSync = true;
    PresentImmediate = true;
    Width = 0;
    Height = 0;
    Depth = 0;
    RefreshRate = 0;
    WindowHandle = 0;
    MultisampleType = SURFACE_MULTISAMPLE_NONE;
    RenderManagerInitFlags = 0;
    ZBufferDepth = 16;
    NoBackBuffers = 1;
    PushBufferSize = 0x80000;
    KickOffSize = 0x8000;
}