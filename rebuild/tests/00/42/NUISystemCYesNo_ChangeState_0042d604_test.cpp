// Behaviour test for NUISystem::CYesNo::ChangeState (0x0042d604): state 1 registers the dialog's CObserver base with
// the UI manager (AddObserver) and observes all events, state 0 removes it (RemoveObserver), every state reaches the base.
#include <cstdio>
namespace NUISystem {
enum EEvent { EVENT_NONE = 0 };
class CObserver {
public:
    CObserver() : observingAll(0) {}
    virtual ~CObserver() {}
    virtual void Observe(EEvent) = 0;
    virtual void ObserveAllEvents() { ++observingAll; }
    int observingAll;
};
class CObservable {
public:
    CObservable() : added(0), removed(0), addCount(0), removeCount(0) {}
    virtual void AddObserver(CObserver* o) { added = o; ++addCount; }
    virtual void RemoveObserver(CObserver* o) { removed = o; ++removeCount; }
    CObserver* added; CObserver* removed; int addCount; int removeCount;
};
class CManager : public CObservable {
public:
    static CManager* __fastcall GetInstance() { static CManager instance; return &instance; }
};
class CComponentRoot { public: virtual ~CComponentRoot() {} virtual void ChangeState(unsigned long) {} };
class CComponentBase3 { public: virtual ~CComponentBase3() {} };
class CChangingStateComponent : public CComponentRoot, public CObserver, public CComponentBase3 {
public:
    CChangingStateComponent() : lastState(99), baseCalls(0) {}
    virtual void ChangeState(unsigned long state) { lastState = state; ++baseCalls; }
    unsigned long lastState; int baseCalls;
};
class CYesNo : public CChangingStateComponent {
public:
    virtual void ChangeState(unsigned long state);
    virtual void Observe(EEvent) {}
};
void CYesNo::ChangeState(unsigned long state)
{
    if (state == 1) {
        CManager::GetInstance()->AddObserver(this);
        ObserveAllEvents();
    }
    if (state == 0)
        CManager::GetInstance()->RemoveObserver(this);
    CChangingStateComponent::ChangeState(state);
}
}
using namespace NUISystem;
int main()
{
    CYesNo box;
    CManager* m = CManager::GetInstance();
    CObserver* asObserver = &box;
    CComponentRoot* asRoot = &box;
    asRoot->ChangeState(1);
    int bad = 0;
    if (m->added != asObserver || m->addCount != 1 || m->removeCount != 0) bad = 1;
    if (box.observingAll != 1 || box.lastState != 1 || box.baseCalls != 1) bad = 1;
    asRoot->ChangeState(2);
    if (m->addCount != 1 || m->removeCount != 0 || box.lastState != 2 || box.baseCalls != 2) bad = 1;
    asRoot->ChangeState(0);
    if (m->removed != asObserver || m->removeCount != 1 || box.observingAll != 1) bad = 1;
    if (box.lastState != 0 || box.baseCalls != 3) bad = 1;
    if (bad) { std::printf("0042d604_TEST FAIL\n"); return 1; }
    std::printf("0042d604_TEST PASS\n");
    return 0;
}