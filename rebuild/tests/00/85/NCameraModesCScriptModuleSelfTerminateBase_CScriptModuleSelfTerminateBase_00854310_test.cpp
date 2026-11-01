// Behaviour test for NCameraModes::CScriptModuleSelfTerminateBase::~CScriptModuleSelfTerminateBase (0x00854310): the
// compiler-generated destructor adds nothing of its own; the base CScriptModuleBase body clears POwner / PCameraManager and
// destroys PThingOrigin then PCamPoint.
#include <cstdio>
static int g_order[4]; static int g_n = 0; static int g_cleared = 0;
struct IntelligentPointer { int id; ~IntelligentPointer() { g_order[g_n++] = id; } };
struct ScriptModuleBase {
    IntelligentPointer PCamPoint;
    IntelligentPointer PThingOrigin;
    void* POwner;
    void* PCameraManager;
    virtual ~ScriptModuleBase() { POwner = 0; PCameraManager = 0; g_cleared = (POwner == 0 && PCameraManager == 0); }
};
struct ScriptModuleSelfTerminateBase : ScriptModuleBase {
    long WFDuration;
    long EndFrame;
};
int main()
{
    {
        ScriptModuleSelfTerminateBase m; m.PCamPoint.id = 1; m.PThingOrigin.id = 2; m.POwner = &m; m.PCameraManager = &m;
        m.WFDuration = 30; m.EndFrame = 100;
    }
    if (g_n != 2 || g_order[0] != 2 || g_order[1] != 1 || !g_cleared) { std::printf("00854310_TEST FAIL\n"); return 1; }
    std::printf("00854310_TEST PASS\n");
    return 0;
}