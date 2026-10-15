// Behaviour test for 0042d6b2 NUISystem::CControllerDisconnect::ChangeState: state 1 swaps the front
// end's current input handler for this screen's handler (remembering the old one) and activates it,
// state 0 restores the remembered handler, and the base ChangeState always runs.
#include <cstdio>

struct CInputHandler { int m_gained; CInputHandler() : m_gained(0) { } virtual ~CInputHandler() { } virtual void OnInputFocusGained() { ++m_gained; } };
struct CFrontEnd {
    CInputHandler* m_stack[8]; int m_n;
    CInputHandler* GetCurrentInputHandler() { return m_n ? m_stack[m_n - 1] : 0; }
    void RemoveInputHandler(CInputHandler* h) { for (int i = 0; i < m_n; ++i) if (m_stack[i] == h) { for (int j = i; j + 1 < m_n; ++j) m_stack[j] = m_stack[j + 1]; --m_n; return; } }
    void AddInputHandler(CInputHandler& h) { m_stack[m_n++] = &h; }
};
static CFrontEnd g_fe;
static CFrontEnd* GetInstance() { return &g_fe; }
static unsigned long g_baseState = 99;
struct CRoot { virtual ~CRoot() { } };
struct CChangingStateComponent : CRoot, CInputHandler { virtual void ChangeState(unsigned long s) { g_baseState = s; } };
struct CControllerDisconnect : CChangingStateComponent {
    CInputHandler* m_pPreviousHandler;
    virtual void ChangeState(unsigned long state)
    {
        if (state == 1) {
            m_pPreviousHandler = GetInstance()->GetCurrentInputHandler();
            GetInstance()->RemoveInputHandler(m_pPreviousHandler);
            GetInstance()->AddInputHandler(*this);
            OnInputFocusGained();
        }
        if (state == 0) {
            GetInstance()->RemoveInputHandler(this);
            GetInstance()->AddInputHandler(*m_pPreviousHandler);
        }
        CChangingStateComponent::ChangeState(state);
    }
};

int main()
{
    CInputHandler game; g_fe.m_n = 0; g_fe.AddInputHandler(game);
    CControllerDisconnect screen; screen.m_pPreviousHandler = 0;
    screen.ChangeState(1);
    bool ok = screen.m_pPreviousHandler == &game && g_fe.m_n == 1 && g_fe.m_stack[0] == static_cast<CInputHandler*>(&screen)
        && screen.m_gained == 1 && g_baseState == 1;
    screen.ChangeState(0);
    ok = ok && g_fe.m_n == 1 && g_fe.m_stack[0] == &game && g_baseState == 0;
    screen.ChangeState(2);
    ok = ok && g_fe.m_n == 1 && g_baseState == 2;
    if (!ok) { std::printf("0042d6b2_TEST FAIL\n"); return 1; }
    std::printf("0042d6b2_TEST PASS\n");
    return 0;
}
