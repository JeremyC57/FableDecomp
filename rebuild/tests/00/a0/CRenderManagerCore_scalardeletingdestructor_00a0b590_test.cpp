#include <cstdio>

static char g_log[16]; static int g_n = 0;
static void Log(char c) { if (g_n < 15) g_log[g_n++] = c; g_log[g_n] = 0; }

struct CRenderStateManager {
    virtual ~CRenderStateManager() { Log('S'); }
};

struct CInitBaseClass {
    bool initialised;
    CInitBaseClass() : initialised(false) { }
    bool IsInitialised() { return initialised; }
    void Uninitialise_00a6c120() { Log('U'); initialised = false; }
    ~CInitBaseClass() { Log('F'); }
};

struct CSystemFont : CInitBaseClass { };

struct CRenderManagerCore : CRenderStateManager {
    CSystemFont SystemFont;
    void Uninit() { if (SystemFont.IsInitialised()) SystemFont.Uninitialise_00a6c120(); }
    virtual ~CRenderManagerCore() { Uninit(); }
};

int main()
{
    CRenderStateManager* p = new CRenderManagerCore;
    static_cast<CRenderManagerCore*>(p)->SystemFont.initialised = true;
    delete p;                                       // the scalar deleting destructor through the vtable
    if (g_log[0] != 'U' || g_log[1] != 'F' || g_log[2] != 'S' || g_n != 3) { std::printf("00a0b590_TEST FAIL\n"); return 1; }
    g_n = 0;
    p = new CRenderManagerCore;                     // font never initialised: no Uninitialise
    delete p;
    if (g_log[0] != 'F' || g_log[1] != 'S' || g_n != 2) { std::printf("00a0b590_TEST FAIL\n"); return 1; }
    std::printf("00a0b590_TEST PASS\n");
    return 0;
}