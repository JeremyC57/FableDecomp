// Behaviour test for 004016f0: body copied from src/004016f0.cpp (CDisplayManagerInit::CDisplayManagerInit), the CWideString constructor stubbed.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct HWND__;
enum ESurfaceMultisampleType { SURFACE_MULTISAMPLE_NONE = 0 };
static int g_stringCtor = 0;
class CWideString { public:
    void* m_pStringData;
    CWideString() : m_pStringData(0) { ++g_stringCtor; }
};
class CDisplayManagerInit { public:
    long                    Width;
    long                    Height;
    long                    Depth;
    long                    RefreshRate;
    bool                    UseRenderManager;
    bool                    UseShaderRenderManager;
    HWND__*                 WindowHandle;
    unsigned char           ZBufferDepth;
    unsigned char           NoBackBuffers;
    ESurfaceMultisampleType MultisampleType;
    long                    PushBufferSize;
    long                    KickOffSize;
    bool                    PresentImmediate;
    bool                    PresentImmediatelyIfMissedVSync;
    long                    PresentationIntevalVSyncCount;
    bool                    SkipConfigDetection;
    unsigned long           RenderManagerInitFlags;
    CWideString             ShaderResourcePath;
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
int main()
{
    unsigned char raw[sizeof(CDisplayManagerInit)]; memset(raw, 0xAA, sizeof(raw));
    CDisplayManagerInit* p = new (raw) CDisplayManagerInit;
    CHECK(g_stringCtor == 1); CHECK(p->ShaderResourcePath.m_pStringData == 0);
    CHECK(p->Width == 0); CHECK(p->Height == 0); CHECK(p->Depth == 0); CHECK(p->RefreshRate == 0); CHECK(p->WindowHandle == 0);
    CHECK(p->MultisampleType == SURFACE_MULTISAMPLE_NONE); CHECK(p->RenderManagerInitFlags == 0);
    CHECK(p->UseRenderManager); CHECK(p->UseShaderRenderManager); CHECK(p->PresentationIntevalVSyncCount == 1);
    CHECK(p->PresentImmediatelyIfMissedVSync); CHECK(p->PresentImmediate);
    CHECK(p->ZBufferDepth == 16); CHECK(p->NoBackBuffers == 1); CHECK(p->PushBufferSize == 0x80000); CHECK(p->KickOffSize == 0x8000);
    // SkipConfigDetection is not initialised by the constructor
    CHECK(raw[0x30] == 0xAA);
    if (fails == 0) printf("004016f0_TEST PASS\n"); else printf("004016f0_TEST FAIL\n");
    return fails;
}