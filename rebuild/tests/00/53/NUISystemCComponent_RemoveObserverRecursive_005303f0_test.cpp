// Behaviour test for 005303f0 NUISystem::CComponent::RemoveObserverRecursive: every component of the
// tree unregisters its observer interface (passed as the CObserver base) and is notified, parents first.
#include <cstdio>
#include <vector>

struct CObserver { virtual void Notify() { } virtual ~CObserver() { } };
static std::vector<CObserver*> g_removed;
static int g_notified = 0;
struct CFrontEnd { void RemoveObserver(CObserver* o) { g_removed.push_back(o); } };
static CFrontEnd g_frontEnd;
static CFrontEnd* GetInstance() { return &g_frontEnd; }

struct CBase { virtual void OnObserversRemoved() { ++g_notified; } virtual ~CBase() { } };
struct CComponent : CBase, CObserver {
    std::vector<CComponent*> Children;
    void RemoveObserverRecursive()
    {
        GetInstance()->RemoveObserver(this);
        OnObserversRemoved();
        for (unsigned long i = 0; i < Children.size(); ++i) Children[i]->RemoveObserverRecursive();
    }
};

int main()
{
    CComponent root, a, b;
    root.Children.push_back(&a); a.Children.push_back(&b);
    root.RemoveObserverRecursive();
    bool ok = g_removed.size() == 3 && g_removed[0] == static_cast<CObserver*>(&root)
        && g_removed[2] == static_cast<CObserver*>(&b) && g_notified == 3;
    if (!ok) { std::printf("005303f0_TEST FAIL\n"); return 1; }
    std::printf("005303f0_TEST PASS\n");
    return 0;
}