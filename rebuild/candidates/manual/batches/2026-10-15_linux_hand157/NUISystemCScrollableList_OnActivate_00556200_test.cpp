// Behaviour test for 0x00556200 NUISystem::CScrollableList::OnActivate: the call sequence on the list's input
// client base and the front-end manager, with null and non-null arrows.
#include <cstdio>
static int g_log[16]; static int g_n = 0;
struct CInputClient { virtual ~CInputClient() {} virtual void AddInputEvent(long e) { g_log[g_n++] = (int)e; } };
struct CComponentBase { virtual ~CComponentBase() {} };
struct CComponent : CComponentBase, CInputClient {};
struct CFrontEndManager {
    CInputClient* added; CInputClient* nav[2]; int navCount;
    void AddInputClient(CInputClient* c) { added = c; g_log[g_n++] = 100; }
    void SetNavigationClient(CInputClient* c) { nav[navCount++] = c; g_log[g_n++] = 200; }
    static CFrontEndManager* GetInstance() { static CFrontEndManager m; return &m; }
};
struct CScrollableList : CComponent {
    float ScrollTimer; CComponent* PUpArrow; CComponent* PDownArrow; float a, b;
    void UpdateVisibleList(float x, float y) { a = x; b = y; }
    void OnActivate()
    {
        UpdateVisibleList(0.0f, 0.2f);
        ScrollTimer = 0.2f;
        AddInputEvent(0x25); AddInputEvent(0x24); AddInputEvent(4);
        CFrontEndManager::GetInstance()->AddInputClient(this);
        CFrontEndManager::GetInstance()->SetNavigationClient(PUpArrow);
        CFrontEndManager::GetInstance()->SetNavigationClient(PDownArrow);
    }
};
int main()
{
    CComponent down; CScrollableList l; l.PUpArrow = 0; l.PDownArrow = &down;
    CFrontEndManager* m = CFrontEndManager::GetInstance(); m->navCount = 0;
    l.OnActivate();
    bool ok = l.a == 0.0f && l.b == 0.2f && l.ScrollTimer == 0.2f && g_n == 6 && g_log[0] == 0x25 && g_log[1] == 0x24 && g_log[2] == 4
        && g_log[3] == 100 && m->added == static_cast<CInputClient*>(&l) && m->nav[0] == 0 && m->nav[1] == static_cast<CInputClient*>(&down);
    if (!ok) { std::printf("00556200_TEST FAIL\n"); return 1; }
    std::printf("00556200_TEST PASS\n");
    return 0;
}
