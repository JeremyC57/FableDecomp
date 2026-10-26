// Behaviour test for CSurfaceCompositor::~CSurfaceCompositor: render target released, open capture block closed and restored, then CBase (0x00B2E6C0).
#include <cstdio>
static int g_log[32]; static int g_n = 0;
class CCharString { public: int id; ~CCharString() { g_log[g_n++] = id; } };
class CCPPointerInfo { public: unsigned long RefCount; void (*DeleteFunc)(void* data); void* Data; };
static int g_dataDeleted = 0;
static void DeleteData(void* data) { (void)data; ++g_dataDeleted; g_log[g_n++] = 50; }
template<class T> class CCountedPointer { public:
    T* Data; CCPPointerInfo* Info;
    void ReleaseInfo() { if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } } }
    ~CCountedPointer() { ReleaseInfo(); Data = 0; Info = 0; }
};
static CCPPointerInfo* NewInfo(unsigned long refs) { CCPPointerInfo* i = new CCPPointerInfo; i->RefCount = refs; i->DeleteFunc = DeleteData; i->Data = 0; return i; }
static int g_restores = 0;
class CRenderStateManager { public: void RestoreCaptureBlock() { ++g_restores; g_log[g_n++] = 60; } };
static CRenderStateManager g_manager; static CRenderStateManager* g_pRenderStateManager = &g_manager;
struct CRenderStateCaptureBlock { bool Open; ~CRenderStateCaptureBlock() { if (Open) { Open = false; g_pRenderStateManager->RestoreCaptureBlock(); } } };
class CBase { public: virtual ~CBase() { g_log[g_n++] = 100; } };
class CRenderTarget; class CSurface;
class CSurfaceCompositor : public CBase { public: long CurrentBlend; CRenderStateCaptureBlock StateCaptureBlock; bool WasRendering; CSurface* DestSurface; CCountedPointer<CRenderTarget> RenderTarget; float OldWindow[4]; long LayerCount; bool AlphaTarget; };
int main()
{
    { CSurfaceCompositor c; c.StateCaptureBlock.Open = true; c.RenderTarget.Data = 0; c.RenderTarget.Info = NewInfo(1); }
    bool ok1 = g_n == 3 && g_log[0] == 50 && g_log[1] == 60 && g_log[2] == 100 && g_restores == 1;
    g_n = 0;
    { CSurfaceCompositor c; c.StateCaptureBlock.Open = false; c.RenderTarget.Data = 0; c.RenderTarget.Info = 0; }
    bool ok2 = g_n == 1 && g_log[0] == 100 && g_restores == 1;
    if (!(ok1 && ok2)) { std::printf("00b2e6c0_TEST FAIL\n"); return 1; }
    std::printf("00b2e6c0_TEST PASS\n");
    return 0;
}