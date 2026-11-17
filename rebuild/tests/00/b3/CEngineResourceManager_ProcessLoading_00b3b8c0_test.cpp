// behaviour test for 0x00B3B8C0 CEngineResourceManager::ProcessLoading()
// (model: the CAsyncLoadingProcessCallback override, entered with `this` on the base at +0x0C, adjusts back to the manager and calls
// the non-virtual ProcessLoading(bool, double) with false and 1e30f widened to double).
// Rewritten by agentNF56 on the new name's behaviour (the old test drove the free CEngineResourceManager_ProcessLoading stand-in over
// a local SubObj::Do and an extern g_procLoadTime); the catalog PassPattern is kept.
#include <stdio.h>
namespace model {
static const void* g_this = 0; static int g_flag = -1; static double g_until = 0.0;
struct Callback { virtual void ProcessLoading() = 0; };
struct Component { virtual void Update() {} int A; int B; };
struct Manager : Component, Callback {
    void ProcessLoading(bool flag, double until) { g_this = this; g_flag = flag ? 1 : 0; g_until = until; }
    virtual void ProcessLoading() { ProcessLoading(false, 1e30f); }
};
}
int main()
{
    model::Manager manager;
    model::Callback* callback = &manager;
    callback->ProcessLoading();
    if ((char*)callback - (char*)&manager == 0x0c && model::g_this == (void*)&manager && model::g_flag == 0 &&
        model::g_until == (double)1e30f)
        printf("00b3b8c0_TEST PASS\n");
    else
        printf("FAIL this=%p a=%d d=%g\n", model::g_this, model::g_flag, model::g_until);
    return 0;
}
