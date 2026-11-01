// Behaviour test for CAInputProcess::~CAInputProcess (0x00A0D3D0): when registered, Cleanup runs, the manager drops the process and
// the pointer is cleared; an unregistered process touches nothing.
#include <cstdio>
class CAInputProcess;
static int g_cleanups = 0;
class CInputProcessManager {
public:
    CInputProcessManager() : Removed(0), Count(0) { }
    void RemoveInputProcess(CAInputProcess& p) { Removed = &p; ++Count; }
    CAInputProcess* Removed;
    int Count;
};
class CAInputProcess {
public:
    CAInputProcess(CInputProcessManager* m) : PPlayerInterface(m) { }
    virtual ~CAInputProcess() { RemoveFromInterface(); }
    virtual void Cleanup() { ++g_cleanups; }
    void RemoveFromInterface()
    {
        if (PPlayerInterface) {
            Cleanup();
            PPlayerInterface->RemoveInputProcess(*this);
            PPlayerInterface = 0;
        }
    }
    CInputProcessManager* PPlayerInterface;
};
int main()
{
    CInputProcessManager m;
    CAInputProcess* registered = new CAInputProcess(&m);
    CAInputProcess* expected = registered;
    delete registered;
    int ok = g_cleanups == 1 && m.Count == 1 && m.Removed == expected;
    { CAInputProcess loose(0); }
    ok = ok && g_cleanups == 1 && m.Count == 1;
    CAInputProcess twice(&m);
    twice.RemoveFromInterface();
    ok = ok && twice.PPlayerInterface == 0 && m.Count == 2 && g_cleanups == 2;
    if (!ok) { std::printf("00a0d3d0_TEST FAIL\n"); return 1; }
    std::printf("00a0d3d0_TEST PASS\n");
    return 0;
}