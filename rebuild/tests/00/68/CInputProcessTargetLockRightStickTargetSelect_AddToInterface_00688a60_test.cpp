#include <cstdio>
// Behaviour test for 0x00688A60 CInputProcessTargetLockRightStickTargetSelect::AddToInterface: both flags cleared, then the base call
// with the same manager, its result returned. The manager is only passed through, so it stays an opaque forward-declared class
// (engine/CInputProcessManager.h is the shared model; no definition is needed here).
class CInputProcessManager;
static int g_base = 0; static void* g_mgr = 0;
struct CBase { char pad[0x28]; bool AddToInterface(CInputProcessManager& m) { g_base = 1; g_mgr = &m; return true; } };
struct C : CBase { bool ChangedTargetRight; bool ChangedTargetLeft;
    bool AddToInterface(CInputProcessManager& m) { ChangedTargetRight = false; ChangedTargetLeft = false; return CBase::AddToInterface(m); } };
int main()
{
    static int s_manager_storage[4];
    CInputProcessManager& m = *reinterpret_cast<CInputProcessManager*>(s_manager_storage);
    C o; o.ChangedTargetRight = true; o.ChangedTargetLeft = true;
    bool r = o.AddToInterface(m);
    if (o.ChangedTargetRight || o.ChangedTargetLeft || !g_base || g_mgr != (void*)s_manager_storage || !r) { std::printf("00688a60_TEST FAIL\n"); return 1; }
    std::printf("00688a60_TEST PASS\n");
    return 0;
}
