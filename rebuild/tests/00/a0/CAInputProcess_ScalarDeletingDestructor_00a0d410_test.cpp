#include <cstdio>
// Model of CAInputProcess's deleting destructor @ 0x00A0D410: notify, unregister from the manager, null it, base.
class CAInputProcess;
static int g_removed, g_notified, g_base; static CAInputProcess* g_removedArg;
class CInputProcessManager { public: void RemoveInputProcess(CAInputProcess* p) { ++g_removed; g_removedArg = p; } };
struct CBaseClassNonCopyable { virtual ~CBaseClassNonCopyable() { ++g_base; } };
class CAInputProcess : public CBaseClassNonCopyable { public:
    CInputProcessManager* PPlayerInterface;
    virtual ~CAInputProcess() { if (PPlayerInterface != 0) { NotifyRemoved(); PPlayerInterface->RemoveInputProcess(this); PPlayerInterface = 0; } }
    virtual void OnRemovedFromManager() { ++g_notified; }
    void NotifyRemoved() { OnRemovedFromManager(); }
};
int main()
{
    CInputProcessManager mgr; CAInputProcess* p = new CAInputProcess; p->PPlayerInterface = &mgr; CBaseClassNonCopyable* b = p;
    delete b;
    if (g_removed != 1 || g_removedArg != p || g_notified != 1 || g_base != 1) { std::printf("00a0d410_TEST FAIL\n"); return 1; }
    { CAInputProcess q; q.PPlayerInterface = 0; }
    if (g_removed != 1 || g_notified != 1 || g_base != 2) { std::printf("00a0d410_TEST FAIL\n"); return 1; }
    std::printf("00a0d410_TEST PASS\n"); return 0;
}