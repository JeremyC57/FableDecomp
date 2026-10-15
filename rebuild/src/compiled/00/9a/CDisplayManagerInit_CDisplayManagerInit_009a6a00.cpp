// CDisplayManagerInit::CDisplayManagerInit(const CDisplayManagerInit&) -- memberwise copy of the display-manager start-up settings (the shader resource path through CWideString's copy constructor)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A6A00, length 125 bytes.
// Callees: CWideString::CWideString(const CWideString&) @ 0x0099B720.
// Layout: PDB CDisplayManagerInit (60 bytes): Width +0, Height +4, Depth +8, RefreshRate +0xc, UseRenderManager +0x10,
// UseShaderRenderManager +0x11, WindowHandle +0x14, ZBufferDepth +0x18, NoBackBuffers +0x19, MultisampleType +0x1c,
// PushBufferSize +0x20, KickOffSize +0x24, PresentImmediate +0x28, PresentImmediatelyIfMissedVSync +0x29,
// PresentationIntevalVSyncCount +0x2c, SkipConfigDetection +0x30, RenderManagerInitFlags +0x34, ShaderResourcePath +0x38.
// Shape: the compiler-shaped memberwise copy: the source in eax, every scalar member copied through ecx/edx alternately (bytes
// as bytes, padding skipped), the string's copy constructor last with the source address formed by `add eax,38h`.
struct HWND__;

enum ESurfaceMultisampleType { SURFACE_MULTISAMPLE_NONE = 0 };

struct CWideString {
    unsigned short* m_pData;
    CWideString(const CWideString& other);         // 0x0099B720
};

struct CDisplayManagerInit {
    long                    Width;                            // +0x00
    long                    Height;                           // +0x04
    long                    Depth;                            // +0x08
    long                    RefreshRate;                      // +0x0c
    bool                    UseRenderManager;                 // +0x10
    bool                    UseShaderRenderManager;           // +0x11
    HWND__*                 WindowHandle;                     // +0x14
    unsigned char           ZBufferDepth;                     // +0x18
    unsigned char           NoBackBuffers;                    // +0x19
    ESurfaceMultisampleType MultisampleType;                  // +0x1c
    long                    PushBufferSize;                   // +0x20
    long                    KickOffSize;                      // +0x24
    bool                    PresentImmediate;                 // +0x28
    bool                    PresentImmediatelyIfMissedVSync;  // +0x29
    long                    PresentationIntevalVSyncCount;    // +0x2c
    bool                    SkipConfigDetection;              // +0x30
    unsigned long           RenderManagerInitFlags;           // +0x34
    CWideString             ShaderResourcePath;               // +0x38
    CDisplayManagerInit(const CDisplayManagerInit& other);
};

CDisplayManagerInit::CDisplayManagerInit(const CDisplayManagerInit& other)
    : Width(other.Width), Height(other.Height), Depth(other.Depth), RefreshRate(other.RefreshRate),
      UseRenderManager(other.UseRenderManager), UseShaderRenderManager(other.UseShaderRenderManager),
      WindowHandle(other.WindowHandle), ZBufferDepth(other.ZBufferDepth), NoBackBuffers(other.NoBackBuffers),
      MultisampleType(other.MultisampleType), PushBufferSize(other.PushBufferSize), KickOffSize(other.KickOffSize),
      PresentImmediate(other.PresentImmediate), PresentImmediatelyIfMissedVSync(other.PresentImmediatelyIfMissedVSync),
      PresentationIntevalVSyncCount(other.PresentationIntevalVSyncCount), SkipConfigDetection(other.SkipConfigDetection),
      RenderManagerInitFlags(other.RenderManagerInitFlags), ShaderResourcePath(other.ShaderResourcePath)
{
}