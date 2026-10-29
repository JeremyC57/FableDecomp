#include <cstdio>
static int g_added, g_removed, g_focus, g_baseState = -1;
static void* g_lastHandler;
struct CInputHandler { int pad; void OnInputFocusGained() { ++g_focus; } };
namespace NUISystem { class CManager { public:
    void AddInputHandlerPtr(CInputHandler* h) { ++g_added; g_lastHandler = h; }
    void RemoveInputHandlerPtr(CInputHandler* h) { ++g_removed; g_lastHandler = h; }
    static CManager* __fastcall GetInstance() { static CManager m; return &m; }
}; }
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
        NUISystem::CManager::GetInstance()->AddInputHandlerPtr(this);
        OnInputFocusGained();
    }
    else
        NUISystem::CManager::GetInstance()->RemoveInputHandlerPtr(this);
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