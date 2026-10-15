#include <cstdio>
// Behaviour test for 009a6a00.

// CDisplayManagerInit copy constructor: every member copied, the shader path through the string's own copy constructor.
static int g_stringCopies = 0;
struct CWideString { const char* m_pData; CWideString(const char* s) : m_pData(s) { } CWideString(const CWideString& o) : m_pData(o.m_pData) { ++g_stringCopies; } };
struct CDisplayManagerInit {
    long Width, Height, Depth, RefreshRate; bool UseRenderManager, UseShaderRenderManager; void* WindowHandle;
    unsigned char ZBufferDepth, NoBackBuffers; long MultisampleType, PushBufferSize, KickOffSize; bool PresentImmediate,
    PresentImmediatelyIfMissedVSync; long PresentationIntevalVSyncCount; bool SkipConfigDetection; unsigned int RenderManagerInitFlags;
    CWideString ShaderResourcePath;
    CDisplayManagerInit() : ShaderResourcePath("shaders") { }
    CDisplayManagerInit(const CDisplayManagerInit& other)
        : Width(other.Width), Height(other.Height), Depth(other.Depth), RefreshRate(other.RefreshRate),
          UseRenderManager(other.UseRenderManager), UseShaderRenderManager(other.UseShaderRenderManager),
          WindowHandle(other.WindowHandle), ZBufferDepth(other.ZBufferDepth), NoBackBuffers(other.NoBackBuffers),
          MultisampleType(other.MultisampleType), PushBufferSize(other.PushBufferSize), KickOffSize(other.KickOffSize),
          PresentImmediate(other.PresentImmediate), PresentImmediatelyIfMissedVSync(other.PresentImmediatelyIfMissedVSync),
          PresentationIntevalVSyncCount(other.PresentationIntevalVSyncCount), SkipConfigDetection(other.SkipConfigDetection),
          RenderManagerInitFlags(other.RenderManagerInitFlags), ShaderResourcePath(other.ShaderResourcePath) { }
};
int main()
{
    CDisplayManagerInit a; a.Width = 800; a.Height = 600; a.Depth = 32; a.RefreshRate = 60; a.UseRenderManager = true;
    a.UseShaderRenderManager = false; a.WindowHandle = &a; a.ZBufferDepth = 24; a.NoBackBuffers = 2; a.MultisampleType = 4;
    a.PushBufferSize = 1; a.KickOffSize = 2; a.PresentImmediate = true; a.PresentImmediatelyIfMissedVSync = true;
    a.PresentationIntevalVSyncCount = 1; a.SkipConfigDetection = true; a.RenderManagerInitFlags = 0x10;
    CDisplayManagerInit b(a);
    int ok = b.Width == 800 && b.Height == 600 && b.Depth == 32 && b.UseRenderManager && b.WindowHandle == &a && b.ZBufferDepth == 24
          && b.NoBackBuffers == 2 && b.MultisampleType == 4 && b.PresentImmediatelyIfMissedVSync && b.SkipConfigDetection
          && b.RenderManagerInitFlags == 0x10 && b.ShaderResourcePath.m_pData == a.ShaderResourcePath.m_pData && g_stringCopies == 1;
    if (!ok) { std::printf("009a6a00_TEST FAIL\n"); return 1; }
    std::printf("009a6a00_TEST PASS\n");
    return 0;
}
