// Behaviour test for 0x008FCF10 CAIStateGroupBase::IsRunnableMain: Enabled && IsRunnable().
#include <cstdio>

struct CAIBrain;

struct CAIStateGroupBase {
    CAIBrain*     PBrain;
    unsigned char ScriptAIPriority[4];
    void*         Name_PStringData;
    bool          Terminated;
    unsigned char Priority;
    bool          Enabled;
    int           m_runnableCalls;
    bool          m_runnable;
    virtual bool IsRunnable() { ++m_runnableCalls; return m_runnable; }
    bool IsRunnableMain();
};

bool CAIStateGroupBase::IsRunnableMain()
{
    if (Enabled && IsRunnable())
        return true;
    return false;
}

int main()
{
    int fail = 0;
    CAIStateGroupBase g;
    g.Enabled = false; g.m_runnable = true; g.m_runnableCalls = 0;
    if (g.IsRunnableMain() || g.m_runnableCalls != 0) fail = 1;      // disabled: virtual not consulted
    g.Enabled = true; g.m_runnable = false;
    if (g.IsRunnableMain() || g.m_runnableCalls != 1) fail = 1;
    g.m_runnable = true;
    if (!g.IsRunnableMain() || g.m_runnableCalls != 2) fail = 1;
    if (fail) { std::printf("008fcf10_TEST FAIL\n"); return 1; }
    std::printf("008fcf10_TEST PASS\n");
    return 0;
}