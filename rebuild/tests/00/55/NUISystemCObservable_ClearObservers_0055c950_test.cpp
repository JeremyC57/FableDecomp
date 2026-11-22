// Behaviour test for 0x0055c950 (NF230, q12624) on the shared header engine/NUISystem_CObservable.h: NUISystem::CObservable::ClearObservers, the row's
// body under the row's defines, on the header class (a test-only subclass reads the protected lists; a test observer counts
// IgnoreAllEvents). One case per condition in the row. Stubbed: the other CObservable virtuals (the vtable of the implicit
// constructor needs them) and CObserver's out-of-line constructor / destructor / virtuals. Replaces the HEAD host model.

#include <stdio.h>
#include "engine/NUISystem_CObservable.h"
namespace NUISystem {
void CObservable::ClearObservers()
{
    Observers.clear();
    m_pExclusiveObserver = 0;
    ConcurrentExclusiveObservers.clear();
}
CObserver::CObserver() { }
CObserver::~CObserver() { }
bool CObserver::IsObservingEvent(EEvent) { return false; }
void CObserver::ObserveEvent(EEvent) { }
void CObserver::IgnoreEvent(EEvent) { }
void CObserver::IgnoreAllEvents() { }
void CObserver::ObserveAllEvents() { }
void CObserver::SetPreventObserving(bool) { }
void CObservable::SetEvent(NUISystem::EEvent) { }
void CObservable::SetOwnedEvent(NUISystem::CObserver*, NUISystem::EEvent) { }
void CObservable::AddObserver(NUISystem::CObserver*) { }
void CObservable::AddExclusiveObserver(NUISystem::CObserver*) { }
void CObservable::RemoveExclusiveObserver(NUISystem::CObserver*) { }
void CObservable::RemoveObserver(NUISystem::CObserver*) { }
NUISystem::CObserver* CObservable::GetExclusiveObserver() { return 0; }
bool CObservable::IsExclusivelyObserved() { return false; }
void CObservable::AddConcurrentExclusiveObserver(NUISystem::CObserver*) { }
void CObservable::RemoveConcurrentExclusiveObserver(NUISystem::CObserver*) { }
}
struct CTestObserver : NUISystem::CObserver {
    int Ignored;
    CTestObserver() : Ignored(0) { }
    void ProcessEvent(NUISystem::EEvent) { }
    void IgnoreAllEvents() { ++Ignored; }
};
struct CTestObservable : NUISystem::CObservable {
    std::list<NUISystem::CObserver*>& Obs() { return Observers; }
    std::list<NUISystem::CObserver*>& Conc() { return ConcurrentExclusiveObservers; }
    NUISystem::CObserver*& Excl() { return m_pExclusiveObserver; }
    CTestObservable() { m_pExclusiveObserver = 0; }
};
static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); ++g_fails; } }
int main()
{
    {
        CTestObservable p; CTestObserver x, y;
        NUISystem::CObserver* two[2] = { &x, &y };
        p.ClearObservers();
        Check(p.Obs().empty() && p.Conc().empty() && p.Excl() == 0, "already empty: stays empty");
        p.Obs().insert(p.Obs().end(), two, two + 2); p.Excl() = &x; p.Conc().insert(p.Conc().end(), two + 1, two + 2);
        Check(!p.Obs().empty() && !p.Conc().empty(), "set up: both lists hold observers");
        p.ClearObservers();
        Check(p.Obs().empty() && p.Obs().begin() == p.Obs().end(), "the Observers list is emptied");
        Check(p.Excl() == 0, "the exclusive observer is cleared");
        Check(p.Conc().empty() && p.Conc().begin() == p.Conc().end(), "the concurrent exclusive list is emptied");
        Check(x.Ignored == 0 && y.Ignored == 0, "no observer is told to ignore events");
        p.Obs().insert(p.Obs().end(), two, two + 1);
        Check(!p.Obs().empty() && *p.Obs().begin() == &x, "the lists stay usable");
    }
    printf(g_fails ? "0055c950_TEST FAIL\n" : "0055c950_TEST PASS\n");
    return g_fails ? 1 : 0;
}
