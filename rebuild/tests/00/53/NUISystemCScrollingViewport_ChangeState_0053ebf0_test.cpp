#include <cstdio>
struct IViewportPrimary {
    virtual void v00() {}
};
struct CObserver {
    virtual void a00() {} virtual void a01() {} virtual void a02() {}
    virtual void ObserveEvent(long eventType) { events[count++] = eventType; }
    long events[8]; int count;
};
struct IViewportSecondaryB {
    virtual void b00() {}
};
struct CChangingStateComponent : IViewportPrimary, CObserver, IViewportSecondaryB {
    unsigned long baseState; int baseCalls;
    void ChangeState(unsigned long state) { baseState = state; ++baseCalls; }
};
struct CFrontEndManager {
    CObserver* list[8]; int n;
    virtual void AddObserver(CObserver* observer) { list[n++] = observer; }
    virtual void RemoveObserver(CObserver* observer)
    { for (int i = 0; i < n; ++i) if (list[i] == observer) { list[i] = list[--n]; return; } }
    static CFrontEndManager* GetInstance();
};
static CFrontEndManager g_manager;
CFrontEndManager* CFrontEndManager::GetInstance() { return &g_manager; }
struct CScrollBarComponent : IViewportPrimary, CObserver {
};

namespace NUISystem {
class CScrollingViewport;
CScrollingViewport* g_pActiveScrollingViewport;

class CScrollingViewport : public CChangingStateComponent { public:
    CScrollBarComponent* PScrollBar;                     // +0x188
    CScrollBarComponent* PScrollBarOutside;              // +0x18C
    bool  Changed;                                       // +0x190
    float OffsetToUseAbove;                              // +0x194
    float OffsetToUseUnder;                              // +0x198
    void ChangeState(unsigned long state);
};

void CScrollingViewport::ChangeState(unsigned long state)
{
    switch (state) {
    case 0:
        CFrontEndManager::GetInstance()->RemoveObserver(this);
        if (g_pActiveScrollingViewport == this)
            g_pActiveScrollingViewport = 0;
        CChangingStateComponent::ChangeState(state);
        if (PScrollBar)
            CFrontEndManager::GetInstance()->RemoveObserver(PScrollBar);
        if (PScrollBarOutside)
            CFrontEndManager::GetInstance()->RemoveObserver(PScrollBarOutside);
        break;
    case 1:
    case 3:
        CFrontEndManager::GetInstance()->AddObserver(this);
        ObserveEvent(6);
        ObserveEvent(7);
        ObserveEvent(0x19);
        if (g_pActiveScrollingViewport == 0)
            g_pActiveScrollingViewport = this;
        CChangingStateComponent::ChangeState(state);
        if (PScrollBar) {
            CFrontEndManager::GetInstance()->AddObserver(PScrollBar);
            PScrollBar->ObserveEvent(0x19);
        }
        if (PScrollBarOutside) {
            CFrontEndManager::GetInstance()->AddObserver(PScrollBarOutside);
            PScrollBarOutside->ObserveEvent(0x19);
        }
        break;
    default:
        CChangingStateComponent::ChangeState(state);
        break;
    }
}
}

static bool has(CObserver* o) { for (int i = 0; i < g_manager.n; ++i) if (g_manager.list[i] == o) return true; return false; }
int main()
{
    using namespace NUISystem;
    static CScrollingViewport v; static CScrollBarComponent bar, outside;
    v.PScrollBar = &bar; v.PScrollBarOutside = 0; v.count = 0; bar.count = 0; v.baseCalls = 0;
    g_manager.n = 0; g_pActiveScrollingViewport = 0;
    v.ChangeState(1);
    bool ok = g_manager.n == 2 && has(static_cast<CObserver*>(&v)) && has(&bar) && v.count == 3 && v.events[0] == 6
           && v.events[1] == 7 && v.events[2] == 0x19 && bar.count == 1 && bar.events[0] == 0x19
           && g_pActiveScrollingViewport == &v && v.baseState == 1 && v.baseCalls == 1;
    v.ChangeState(2);                                   // other states only reach the base
    ok = ok && g_manager.n == 2 && v.baseState == 2 && v.baseCalls == 2;
    v.PScrollBarOutside = &outside; outside.count = 0;
    g_manager.list[g_manager.n++] = &outside;
    v.ChangeState(0);
    ok = ok && g_manager.n == 0 && g_pActiveScrollingViewport == 0 && v.baseState == 0 && v.baseCalls == 3;
    if (!ok) { std::printf("0053ebf0_TEST FAIL\n"); return 1; }
    std::printf("0053ebf0_TEST PASS\n");
    return 0;
}