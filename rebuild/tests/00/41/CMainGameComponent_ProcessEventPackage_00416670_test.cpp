#include <cstdio>
class CGameEvent;
class CGameEventPackage {
public:
    const CGameEvent* GetEvent(unsigned long index) const;   // 0x009F16D0
    unsigned long GetNoEvents() const;                   // 0x009F16E0
};
class CMainGameComponent {
protected:
    void ProcessEvent(const CGameEvent* event);          // 0x00415FE0
    void ProcessEventPackage(const CGameEventPackage* package);
    friend struct CMainGameComponent_Test;               // behaviour-test hook (both members are protected)
};
void CMainGameComponent::ProcessEventPackage(const CGameEventPackage* package)
{
    for (unsigned long i = 0; i < package->GetNoEvents(); ++i)
        ProcessEvent(package->GetEvent(i));
}

class CGameEvent { public: int Id; };
static CGameEvent g_events[5]; static unsigned long g_count; static int g_seen[8]; static int g_n;
const CGameEvent* CGameEventPackage::GetEvent(unsigned long i) const { return &g_events[i]; }
unsigned long CGameEventPackage::GetNoEvents() const { return g_count; }
void CMainGameComponent::ProcessEvent(const CGameEvent* e) { g_seen[g_n++] = e->Id; }
struct CMainGameComponent_Test {
    static void Run(CMainGameComponent& c, const CGameEventPackage* p) { c.ProcessEventPackage(p); }
};
int main()
{
    for (int i = 0; i < 5; ++i) g_events[i].Id = 10 + i;
    CMainGameComponent c; CGameEventPackage p;
    g_count = 3; CMainGameComponent_Test::Run(c, &p);
    bool ok = g_n == 3 && g_seen[0] == 10 && g_seen[1] == 11 && g_seen[2] == 12;
    g_count = 0; CMainGameComponent_Test::Run(c, &p);
    ok = ok && g_n == 3;
    std::printf(ok ? "00416670_TEST PASS\n" : "FAIL\n");
    return 0;
}