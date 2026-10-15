// Behaviour test for 0x00852490 (CScriptModuleSelfTerminateBase constructor): WFDuration is the argument and
// EndFrame the current frame plus that duration.
#include <cstdio>

static long g_frame = 1000;
long GFGetCurrentFrame() { return g_frame; }

static int g_baseCtors = 0;
struct CScriptModuleBase {
    virtual void v00() { }
    virtual ~CScriptModuleBase() { }
    unsigned char m_state[0x48];
    CScriptModuleBase() { ++g_baseCtors; }
};

struct CScriptModuleSelfTerminateBase : CScriptModuleBase {
    long WFDuration;
    long EndFrame;
    virtual void v00() { }
    CScriptModuleSelfTerminateBase(long duration);
};

CScriptModuleSelfTerminateBase::CScriptModuleSelfTerminateBase(long duration)
    : WFDuration(duration)
{
    EndFrame = GFGetCurrentFrame() + WFDuration;
}

int main()
{
    CScriptModuleSelfTerminateBase m(30);
    if (g_baseCtors != 1 || m.WFDuration != 30 || m.EndFrame != 1030) {
        std::printf("00852490_TEST FAIL\n");
        return 1;
    }
    std::printf("00852490_TEST PASS\n");
    return 0;
}