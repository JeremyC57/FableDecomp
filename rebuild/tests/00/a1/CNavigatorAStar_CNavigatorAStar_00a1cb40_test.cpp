// Behaviour test: CNavigatorAStar::~CNavigatorAStar (0x00A1CB40) -- the navigator removes every queue entry naming it
// (CIsNavigator predicate, remove_if) from its manager's queue, then its members die (States cleared last).
#include <cstdio>
class CNavigatorBase;
struct Entry { long first; const CNavigatorBase* second; };
class CNavigatorQueueManager { public:
    Entry items[8]; int count;
    CNavigatorQueueManager() : count(0) { }
    template <class P> void remove_if(P pred)
    {
        int o = 0;
        for (int i = 0; i < count; ++i) if (!pred(items[i])) items[o++] = items[i];
        count = o;
    }
};
class CIsNavigator {
public:
    CIsNavigator(const CNavigatorBase& n) : Navigator(&n) { }
    bool operator()(const Entry& e) const { return e.second == Navigator; }
    const CNavigatorBase* Navigator;
};
class CNavigatorManager { public:
    CNavigatorQueueManager QueueManager;
    CNavigatorQueueManager& GetNavigatorQueueManager() { return QueueManager; }
};
static int g_statesCleared = 0;
struct CStates { int count; CStates() : count(3) { } ~CStates() { if (count != 0) { count = 0; ++g_statesCleared; } } };
class CNavigatorBase { public: CNavigatorManager* NavigationManager; virtual ~CNavigatorBase() { } };
class CNavigatorAStar : public CNavigatorBase {
public:
    CStates States;
    virtual ~CNavigatorAStar()
    {
        CNavigatorQueueManager& q = NavigationManager->GetNavigatorQueueManager();
        q.remove_if(CIsNavigator(*this));
    }
};
int main()
{
    CNavigatorManager mgr;
    CNavigatorAStar* a = new CNavigatorAStar; a->NavigationManager = &mgr;
    CNavigatorAStar* b = new CNavigatorAStar; b->NavigationManager = &mgr;
    Entry e0 = { 1, a }, e1 = { 2, b }, e2 = { 3, a };
    mgr.QueueManager.items[0] = e0; mgr.QueueManager.items[1] = e1; mgr.QueueManager.items[2] = e2; mgr.QueueManager.count = 3;
    delete a;
    if (mgr.QueueManager.count != 1 || mgr.QueueManager.items[0].first != 2 || g_statesCleared != 1)
    { std::printf("00a1cb40_TEST FAIL\n"); return 1; }
    delete b;
    if (mgr.QueueManager.count != 0 || g_statesCleared != 2) { std::printf("00a1cb40_TEST FAIL\n"); return 1; }
    std::printf("00a1cb40_TEST PASS\n");
    return 0;
}