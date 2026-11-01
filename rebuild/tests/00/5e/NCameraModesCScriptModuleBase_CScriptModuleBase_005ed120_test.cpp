// Behaviour test for NCameraModes::CScriptModuleBase::~CScriptModuleBase (0x005ED120): clears POwner / PCameraManager, then
// destroys PThingOrigin and PCamPoint (in that order).
#include <cstdio>
static int g_order[4]; static int g_n = 0;
struct IntelligentPointer { int id; ~IntelligentPointer() { g_order[g_n++] = id; } };
struct ScriptModuleBase {
    IntelligentPointer PCamPoint;
    IntelligentPointer PThingOrigin;
    void* POwner;
    void* PCameraManager;
    ~ScriptModuleBase() { POwner = 0; PCameraManager = 0; snapOwner = POwner; snapCamera = PCameraManager; }
    static void* snapOwner; static void* snapCamera;
};
void* ScriptModuleBase::snapOwner = (void*)1; void* ScriptModuleBase::snapCamera = (void*)1;
int main()
{
    {
        ScriptModuleBase m; m.PCamPoint.id = 1; m.PThingOrigin.id = 2; m.POwner = &m; m.PCameraManager = &m;
    }
    if (g_n != 2 || g_order[0] != 2 || g_order[1] != 1 || ScriptModuleBase::snapOwner || ScriptModuleBase::snapCamera) {
        std::printf("005ed120_TEST FAIL\n"); return 1;
    }
    std::printf("005ed120_TEST PASS\n");
    return 0;
}