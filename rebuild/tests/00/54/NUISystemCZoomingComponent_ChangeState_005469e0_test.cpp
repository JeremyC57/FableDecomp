#include <cstdio>
static int g_added, g_removed, g_focus, g_baseState = -1;
static void* g_lastHandler;
struct CInputHandler { int pad; void OnInputFocusGained() { ++g_focus; } };
struct CFrontEndManager
{
    void AddInputHandlerPtr(CInputHandler* h) { ++g_added; g_lastHandler = h; }
    void RemoveInputHandlerPtr(CInputHandler* h) { ++g_removed; g_lastHandler = h; }
    static CFrontEndManager* GetInstance() { static CFrontEndManager m; return &m; }
};
struct CRoot { int vptrStandIn; };
struct CChangingStateComponent : CRoot, CInputHandler
{
    void ChangeState(unsigned long state) { g_baseState = (int)state; }
};
struct CZoomingComponent : CChangingStateComponent { void ChangeState(unsigned long state); };
void CZoomingComponent::ChangeState(unsigned long state)
{
    if (state != 0)
    {
        CFrontEndManager::GetInstance()->AddInputHandlerPtr(this);
        OnInputFocusGained();
    }
    else
        CFrontEndManager::GetInstance()->RemoveInputHandlerPtr(this);
    CChangingStateComponent::ChangeState(state);
}
int main()
{
    CZoomingComponent z;
    CInputHandler* h = &z;
    z.ChangeState(3);
    if (g_added != 1 || g_focus != 1 || g_lastHandler != h || g_baseState != 3) { std::printf("005469e0_TEST FAIL\n"); return 1; }
    z.ChangeState(0);
    if (g_removed != 1 || g_focus != 1 || g_lastHandler != h || g_baseState != 0) { std::printf("005469e0_TEST FAIL\n"); return 1; }
    std::printf("005469e0_TEST PASS\n");
    return 0;
}