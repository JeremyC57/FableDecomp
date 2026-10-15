// Behaviour test for NUISystem::CRedefinerList::OnDeactivate @ 0x00556580.
#include <cstdio>

static long g_states[4]; static int g_nStates = 0;
struct CListRoot { virtual ~CListRoot() {} };
struct CGuiControl {
    virtual void v000() {} virtual void v001() {} virtual void v002() {}
    virtual void SetMode(long) {}
    virtual void SetState(long state) { g_states[g_nStates++] = state; }
    virtual ~CGuiControl() {}
};
struct CScrollableList : CListRoot, CGuiControl {
    float m_hiddenOver; int m_hideCalls;
    void HideList(float time) { ++m_hideCalls; m_hiddenOver = time; }
};
struct CRedefinerList : CScrollableList {
    bool m_bActive; bool m_bSelecting; float m_fadeTime;
    void OnDeactivate()
    {
        m_bActive = false;
        HideList(0.2f);
        m_fadeTime = 0.2f;
        m_bSelecting = false;
        SetState(0x25);
        SetState(0x24);
    }
};

int main()
{
    CRedefinerList l; l.m_bActive = true; l.m_bSelecting = true; l.m_fadeTime = 1.0f; l.m_hideCalls = 0; l.m_hiddenOver = 0.0f;
    l.OnDeactivate();
    if (l.m_bActive || l.m_bSelecting || l.m_fadeTime != 0.2f || l.m_hideCalls != 1 || l.m_hiddenOver != 0.2f ||
        g_nStates != 2 || g_states[0] != 0x25 || g_states[1] != 0x24) { std::printf("00556580_TEST FAIL\n"); return 1; }
    std::printf("00556580_TEST PASS\n");
    return 0;
}
