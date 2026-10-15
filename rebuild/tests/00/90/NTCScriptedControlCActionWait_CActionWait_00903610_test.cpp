#include <cstdio>

// NTCScriptedControl::CActionWait::CActionWait(long): inlined CActionBase part cleared, update counter 0,
// the frame count stored.
static int g_baseCtors = 0;
struct CBase {
    virtual ~CBase() { }
    CBase() { ++g_baseCtors; }
};
class CTCScriptedControl;
namespace NTCScriptedControl {
struct CActionBase : CBase {
    CTCScriptedControl* PTCScript;
    bool                Activated;
    bool                Terminated;
    CActionBase() : PTCScript(0), Activated(false), Terminated(false) { }
};
struct CActionWait : CActionBase {
    long NoFramesToWait;
    long UpdateCounter;
    CActionWait(long noFramesToWait);
};
CActionWait::CActionWait(long noFramesToWait)
    : NoFramesToWait(noFramesToWait)
    , UpdateCounter(0)
{
}
}

int main()
{
    NTCScriptedControl::CActionWait w(25);
    if (g_baseCtors != 1 || w.PTCScript != 0 || w.Activated || w.Terminated || w.NoFramesToWait != 25 || w.UpdateCounter != 0) {
        std::printf("00903610_TEST FAIL\n");
        return 1;
    }
    std::printf("00903610_TEST PASS\n");
    return 0;
}