#include <cstdio>

class CRenderStateManager;
static CRenderStateManager* g_pRenderStateManager = 0;
static int g_baseDestroyed = 0;
static void* g_lastDevice = (void*)1;

struct CBaseClassNonCopyable {
    virtual void v00() { }
    ~CBaseClassNonCopyable() { ++g_baseDestroyed; }
};

class CRenderStateManager : public CBaseClassNonCopyable { public:
    void* PD3DDevice;
    CRenderStateManager() : PD3DDevice((void*)0x10) { g_pRenderStateManager = this; }
    virtual ~CRenderStateManager()
    {
        g_pRenderStateManager = 0;
        PD3DDevice = 0;
        g_lastDevice = PD3DDevice;
    }
};

int main()
{
    {
        CRenderStateManager m;
        if (g_pRenderStateManager != &m) { std::printf("00a044b0_TEST FAIL\n"); return 1; }
    }
    if (g_pRenderStateManager != 0 || g_lastDevice != 0 || g_baseDestroyed != 1) { std::printf("00a044b0_TEST FAIL\n"); return 1; }
    std::printf("00a044b0_TEST PASS\n");
    return 0;
}