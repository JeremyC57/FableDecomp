// Behaviour test for NUISystem::CScrollingViewport's deleting destructor (0x0053FC30): unregister from the manager, clear the active global only when it is this viewport, run the base destructor.
#include <cstdio>
struct CObserver { int m_tag; virtual ~CObserver() {} };
struct CFrontEndManager {
    CObserver* m_removed; int m_removes;
    void RemoveObserver(CObserver* o) { m_removed = o; ++m_removes; }
};
static CFrontEndManager g_manager;
static CFrontEndManager* GetInstance() { return &g_manager; }
static int g_baseDestroyed = 0;
struct IPrimary { virtual ~IPrimary() {} };
struct CViewport : IPrimary, CObserver { virtual ~CViewport() { ++g_baseDestroyed; } };
struct CScrollingViewport;
static CScrollingViewport* g_pActiveScrollingViewport = 0;
struct CScrollingViewport : CViewport {
    virtual ~CScrollingViewport()
    {
        GetInstance()->RemoveObserver(this);
        if (g_pActiveScrollingViewport == this)
            g_pActiveScrollingViewport = 0;
    }
};
int main()
{
    g_manager.m_removed = 0; g_manager.m_removes = 0;
    CScrollingViewport* a = new CScrollingViewport;
    CScrollingViewport* b = new CScrollingViewport;
    g_pActiveScrollingViewport = b;
    CObserver* aObserver = a;
    delete a;
    if (g_manager.m_removed != aObserver || g_manager.m_removes != 1 || g_pActiveScrollingViewport != b || g_baseDestroyed != 1) { std::printf("0053fc30_TEST FAIL\n"); return 1; }
    delete b;
    if (g_manager.m_removes != 2 || g_pActiveScrollingViewport != 0 || g_baseDestroyed != 2) { std::printf("0053fc30_TEST FAIL\n"); return 1; }
    std::printf("0053fc30_TEST PASS\n");
    return 0;
}